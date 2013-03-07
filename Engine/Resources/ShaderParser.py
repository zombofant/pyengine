# File name: ShaderParser.py
# This file is part of: pyengine
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
# For feedback and questions about pyengine please e-mail one of the
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

    def __call__(self, vardict):
        return itertools.chain.from_iterable((node(vardict) for node in self.Nodes))

    def __repr__(self):
        return "ParentNode({0!r})".format(self.Nodes)

class RootNode(ParentNode):
    def __call__(self, vardict):
        return "\n".join(super(RootNode, self).__call__(vardict))

class StaticNode(CodeNode):
    def __init__(self, lines, **kwargs):
        super(StaticNode, self).__init__(**kwargs)
        self.Lines = list(lines)

    def __call__(self, vardict):
        return iter(self.Lines)

    def __repr__(self):
        return "Code({0!r})".format(self.Lines)

    def append(self, line):
        self.Lines.append(line)

class ConditionalNode(CodeNode):
    def __init__(self):
        self._conditionals = []
        self._else = None

    def __call__(self, vardict):
        for condition, node in self._conditionals:
            if bool(eval(condition, vardict)):
                return node(vardict)
        if self._else is not None:
            return self._else()
        else:
            return ()

    def new_condition(self, expression):
        if self._else is not None:
            raise ValueError("Cannot have another branch after the else branch.")
        node = ParentNode([])
        self._conditionals.append((expression, node))
        return node

    def add_else(self):
        if self._else is not None:
            raise ValueError("Multiple else branches defined.")
        self._else = self.new_condition("True")
        return self._else

    def __repr__(self):
        return "Conditionals["+(", ".join("If({0!r}, {1!r})".format(expr, node) for expr, node in self._conditionals))+"]"

class State(object):
    def __init__(self, parser, transition_test, comment_test):
        self._parser = parser
        self._transition_test = transition_test
        self._comment_test = comment_test

    def preprocessor(self, command, argument):
        return NotImplemented

class DefaultState(State):
    def line(self, line):
        if self._transition_test(line):
            return
        if self._comment_test(line):
            return
        raise ValueError("{0!r} not allowed outside any block.".format(line))

class VariablesState(State):
    _variable_match = re.compile("^([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(\S+)\s*$", re.I)

    def line(self, line):
        if self._transition_test(line):
            return
        if self._comment_test(line):
            return
        variable = self._variable_match.match(line)
        if variable:
            name, value = variable.groups()[0:2]
            self._parser.variables[name] = eval(value, {})
        else:
            raise ValueError("Invalid variable definition: {0}".format(line))

class ShaderCodeState(State):
    def __init__(self, *args):
        super(ShaderCodeState, self).__init__(*args)
        self._root_node = RootNode([])
        self._node_stack = [self._root_node]

    @property
    def _current_node(self):
        return self._node_stack[-1]

    def _push_node(self, node_instance):
        # print(self._node_stack)
        while isinstance(self._current_node, StaticNode):
            self._pop_node()
        assert isinstance(self._current_node, ParentNode), repr(self._node_stack)
        self._current_node.Nodes.append(node_instance)
        self._node_stack.append(node_instance)
        return node_instance

    def _pop_node(self):
        self._node_stack.pop()

    def _get_inner_conditional(self):
        for i, node in enumerate(reversed(self._node_stack)):
            if isinstance(node, ConditionalNode):
                return i, node
        return None, None

    def _make_inner_conditional_current(self, errormsg="No inner conditional found"):
        index, conditional = self._get_inner_conditional()
        if index is None:
            raise ValueError(errormsg)
        if index > 0:
            self._node_stack = self._node_stack[:-index]
        return self._current_node

    def _pp_if(self, argument):
        conditional = self._push_node(ConditionalNode())
        self._node_stack.append(conditional.new_condition(argument))

    def _pp_else(self, argument):
        if argument is not None:
            raise ValueError("else instructions must not have any argument. Got: {0}".format(argument))
        self._make_inner_conditional_current("else not allowed outside if/endif")
        self._node_stack.append(self._current_node.add_else())

    def _pp_elif(self, argument):
        self._make_inner_conditional_current("elif not allowed outside if/endif")
        self._node_stack.append(self._current_node.new_condition(argument))

    def _pp_endif(self, argument):
        self._make_inner_conditional_current("unmatched endif")
        self._pop_node()

    def preprocessor(self, command, argument):
        call = self._preprocessor_map.get(command, None)
        if call is not None:
            return call(self, argument)
        return NotImplemented

    def line(self, line):
        if self._transition_test(line):
            index, conditional = self._get_inner_conditional()
            if index is not None:
                raise ValueError("Unmatched if")
            return
        if not isinstance(self._current_node, StaticNode):
            assert isinstance(self._current_node, ParentNode)
            self._push_node(StaticNode([line]))
        else:
            self._current_node.append(line)
    
    _preprocessor_map = {
        "if": _pp_if,
        "else": _pp_else,
        "endif": _pp_endif,
        "elif": _pp_elif
    }

class FragmentShaderCodeState(ShaderCodeState):
    def __init__(self, *args):
        super(FragmentShaderCodeState, self).__init__(*args)
        self._parser.fp = self._root_node

class VertexShaderCodeState(ShaderCodeState):
    def __init__(self, *args):
        super(VertexShaderCodeState, self).__init__(*args)
        self._parser.vp = self._root_node

class ShaderParser(object):
    _transition = re.compile("^\[([\w\s]+)\]\s*$", re.I)
    _transition_map = {
        re.compile(r"^(library variables|variables|globals)$", re.I): VariablesState,
        re.compile(r"^(vertex( shader)?|vs)$", re.I): VertexShaderCodeState,
        re.compile(r"^(fragment( shader)?|fs)$", re.I): FragmentShaderCodeState
    }
    _preprocessor = re.compile(r"^\s*#(\w+)(\s+(\S.*))?\s*$")
    _outside_comment = re.compile(r"^\s*//.*$")

    _include_file = re.compile(r'^"([^\"]+)"$')
    
    def __init__(self, line_iterable):
        self.line_iterable_stack = [iter(line_iterable)]
        self.state_stack = [DefaultState(self, self._test_transition, self._test_comment)]
        self.variables = {}
        self.fp = None
        self.vp = None

    def _get_line(self):
        while len(self.line_iterable_stack) > 0:
            try:
                return next(self.line_iterable_stack[-1])
            except StopIteration:
                self.line_iterable_stack.pop()
                pass
        else:
            return None

    def _replace_state(self, state_class):
        self.state_stack[-1] = state_class(self, self._test_transition, self._test_comment)

    def _pp_include(self, argument):
        include_file = self._include_file.match(argument.strip())
        if not include_file:
            raise ValueError("Invalid include command: {0}".format(argument))
        file_name = include_file.group(1)
        iterable = ResourceManager().require(file_name, resourcetype="txt").split("\n")
        self.line_iterable_stack.append(iter(iterable))

    def _test_transition(self, line):
        transition = self._transition.match(line)
        if transition:
            target_name = transition.group(1)
            for match, state_class in self._transition_map.iteritems():
                if match.match(target_name):
                    self._replace_state(state_class)
                    return True
            raise KeyError("Unknown section: {0!r}".format(target_name))
        return False

    def _test_comment(self, line):
        line = line.strip()
        if len(line) == 0:
            return True
        if self._outside_comment.match(line):
            return True
        return False

    def parse(self):
        line = self._get_line()
        while line is not None:
            line = line.rstrip()
            preprocessor = self._preprocessor.match(line)
            if preprocessor:
                groups = preprocessor.groups()
                command = groups[0]
                argument = groups[2]
                if command in self._preprocessor_map:
                    self._preprocessor_map[command](self, argument)
                else:
                    status = self.state_stack[-1].preprocessor(command, argument)
                    if status is NotImplemented:
                        raise KeyError("Unknown preprcosser instruction: {0}".format(command))
            else:
                self.state_stack[-1].line(line)
            line = self._get_line()
        return self.variables, self.vp, self.fp

    _preprocessor_map = {
        "include": _pp_include,
    }
