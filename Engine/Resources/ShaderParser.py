# File name: ShaderParser.py
# This file is part of: pyuni
#
# LICENSE
#
# The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and limitations
# under the License.
#
# Alternatively, the contents of this file may be used under the terms
# of the GNU General Public license (the  "GPL License"), in which case
# the provisions of GPL License are applicable instead of those above.
#
# FEEDBACK & QUESTIONS
#
# For feedback and questions about pyuni please e-mail one of the
# authors named in the AUTHORS file.
########################################################################
from __future__ import unicode_literals, print_function, division
from our_future import *

import itertools
import re

import Engine.Utils

from Manager import ResourceManager

class CodeNode(object):
    def __init__(self, **kwargs):
        super(CodeNode, self).__init__(**kwargs)

class ParentNode(CodeNode):
    def __init__(self, nodes, **kwargs):
        super(ParentNode, self).__init__(**kwargs)
        self.Nodes = list(nodes)

    def __call__(self, varDict):
        return itertools.chain.from_iterable((node(varDict) for node in self.Nodes))

    def __repr__(self):
        return "ParentNode({0!r})".format(self.Nodes)

class RootNode(ParentNode):
    def __call__(self, varDict):
        return "\n".join(super(RootNode, self).__call__(varDict))

class StaticNode(CodeNode):
    def __init__(self, lines, **kwargs):
        super(StaticNode, self).__init__(**kwargs)
        self.Lines = list(lines)

    def __call__(self, varDict):
        return iter(self.Lines)

    def __repr__(self):
        return "Code({0!r})".format(self.Lines)

    def append(self, line):
        self.Lines.append(line)

class ConditionalNode(CodeNode):
    def __init__(self):
        self._conditionals = []
        self._else = None

    def __call__(self, varDict):
        for condition, node in self._conditionals:
            if bool(eval(condition, varDict)):
                return node(varDict)
        if self._else is not None:
            return self._else()
        else:
            return ()

    def newCondition(self, expression):
        if self._else is not None:
            raise ValueError("Cannot have another branch after the else branch.")
        node = ParentNode([])
        self._conditionals.append((expression, node))
        return node

    def addElse(self):
        if self._else is not None:
            raise ValueError("Multiple else branches defined.")
        self._else = self.newCondition("True")
        return self._else

    def __repr__(self):
        return "Conditionals["+(", ".join("If({0!r}, {1!r})".format(expr, node) for expr, node in self._conditionals))+"]"

class State(object):
    def __init__(self, parser, transitionTest, commentTest):
        self._parser = parser
        self._transitionTest = transitionTest
        self._commentTest = commentTest

    def preprocessor(self, command, argument):
        return NotImplemented

class DefaultState(State):
    def line(self, line):
        if self._transitionTest(line):
            return
        if self._commentTest(line):
            return
        raise ValueError("{0!r} not allowed outside any block.".format(line))

class VariablesState(State):
    _variableMatch = re.compile("^([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(\S+)\s*$", re.I)

    def line(self, line):
        if self._transitionTest(line):
            return
        if self._commentTest(line):
            return
        variable = self._variableMatch.match(line)
        if variable:
            name, value = variable.groups()[0:2]
            self._parser.variables[name] = eval(value, {})
        else:
            raise ValueError("Invalid variable definition: {0}".format(line))

class ShaderCodeState(State):
    def __init__(self, *args):
        super(ShaderCodeState, self).__init__(*args)
        self._rootNode = RootNode([])
        self._nodeStack = [self._rootNode]

    @property
    def _currentNode(self):
        return self._nodeStack[-1]

    def _pushNode(self, nodeInstance):
        # print(self._nodeStack)
        while isinstance(self._currentNode, StaticNode):
            self._popNode()
        assert isinstance(self._currentNode, ParentNode), repr(self._nodeStack)
        self._currentNode.Nodes.append(nodeInstance)
        self._nodeStack.append(nodeInstance)
        return nodeInstance

    def _popNode(self):
        self._nodeStack.pop()

    def _getInnerConditional(self):
        for i, node in enumerate(reversed(self._nodeStack)):
            if isinstance(node, ConditionalNode):
                return i, node
        return None, None

    def _makeInnerConditionalCurrent(self, errormsg="No inner conditional found"):
        index, conditional = self._getInnerConditional()
        if index is None:
            raise ValueError(errormsg)
        if index > 0:
            self._nodeStack = self._nodeStack[:-index]
        return self._currentNode

    def _pp_if(self, argument):
        conditional = self._pushNode(ConditionalNode())
        self._nodeStack.append(conditional.newCondition(argument))

    def _pp_else(self, argument):
        if argument is not None:
            raise ValueError("else instructions must not have any argument. Got: {0}".format(argument))
        self._makeInnerConditionalCurrent("else not allowed outside if/endif")
        self._nodeStack.append(self._currentNode.addElse())

    def _pp_elif(self, argument):
        self._makeInnerConditionalCurrent("elif not allowed outside if/endif")
        self._nodeStack.append(self._currentNode.newCondition(argument))

    def _pp_endif(self, argument):
        self._makeInnerConditionalCurrent("unmatched endif")
        self._popNode()

    def preprocessor(self, command, argument):
        call = self._preprocessorMap.get(command, None)
        if call is not None:
            return call(self, argument)
        return NotImplemented

    def line(self, line):
        if self._transitionTest(line):
            index, conditional = self._getInnerConditional()
            if index is not None:
                raise ValueError("Unmatched if")
            return
        if not isinstance(self._currentNode, StaticNode):
            assert isinstance(self._currentNode, ParentNode)
            self._pushNode(StaticNode([line]))
        else:
            self._currentNode.append(line)
    
    _preprocessorMap = {
        "if": _pp_if,
        "else": _pp_else,
        "endif": _pp_endif,
        "elif": _pp_elif
    }

class FragmentShaderCodeState(ShaderCodeState):
    def __init__(self, *args):
        super(FragmentShaderCodeState, self).__init__(*args)
        self._parser.fp = self._rootNode

class VertexShaderCodeState(ShaderCodeState):
    def __init__(self, *args):
        super(VertexShaderCodeState, self).__init__(*args)
        self._parser.vp = self._rootNode

class ShaderParser(object):
    _transition = re.compile("^\[([\w\s]+)\]\s*$", re.I)
    _transitionMap = {
        re.compile(r"^(library variables|variables|globals)$", re.I): VariablesState,
        re.compile(r"^(vertex( shader)?|vs)$", re.I): VertexShaderCodeState,
        re.compile(r"^(fragment( shader)?|fs)$", re.I): FragmentShaderCodeState
    }
    _preprocessor = re.compile(r"^\s*#(\w+)(\s+(\S.*))?\s*$")
    _outsideComment = re.compile(r"^\s*//.*$")

    _includeFile = re.compile(r'^"([^\"]+)"$')
    
    def __init__(self, lineIterable):
        self.lineIterableStack = [iter(lineIterable)]
        self.stateStack = [DefaultState(self, self._testTransition, self._testComment)]
        self.variables = {}
        self.fp = None
        self.vp = None

    def _getLine(self):
        while len(self.lineIterableStack) > 0:
            try:
                return next(self.lineIterableStack[-1])
            except StopIteration:
                self.lineIterableStack.pop()
                pass
        else:
            return None

    def _replaceState(self, stateClass):
        self.stateStack[-1] = stateClass(self, self._testTransition, self._testComment)

    def _pp_include(self, argument):
        includeFile = self._includeFile.match(argument.strip())
        if not includeFile:
            raise ValueError("Invalid include command: {0}".format(argument))
        fileName = includeFile.group(1)
        iterable = ResourceManager().require(fileName, resourceType="txt").split("\n")
        self.lineIterableStack.append(iter(iterable))

    def _testTransition(self, line):
        transition = self._transition.match(line)
        if transition:
            targetName = transition.group(1)
            for match, stateClass in self._transitionMap.iteritems():
                if match.match(targetName):
                    self._replaceState(stateClass)
                    return True
            raise KeyError("Unknown section: {0!r}".format(targetName))
        return False

    def _testComment(self, line):
        line = line.strip()
        if len(line) == 0:
            return True
        if self._outsideComment.match(line):
            return True
        return False

    def parse(self):
        line = self._getLine()
        while line is not None:
            line = line.rstrip()
            preprocessor = self._preprocessor.match(line)
            if preprocessor:
                groups = preprocessor.groups()
                command = groups[0]
                argument = groups[2]
                if command in self._preprocessorMap:
                    self._preprocessorMap[command](self, argument)
                else:
                    status = self.stateStack[-1].preprocessor(command, argument)
                    if status is NotImplemented:
                        raise KeyError("Unknown preprcosser instruction: {0}".format(command))
            else:
                self.stateStack[-1].line(line)
            line = self._getLine()
        return self.variables, self.vp, self.fp

    _preprocessorMap = {
        "include": _pp_include,
    }
