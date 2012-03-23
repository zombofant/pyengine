# File name: __init__.py
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

SYNTAX_FILE = "utils/CSSParser/Syntax.pyLRp"
PARSER_FILE = "Engine/UI/CSS/GeneratedParser.py"
LANGUAGE_FILE = "Engine/UI/CSS/Minilanguage.py"

class TooOld(BaseException):
    pass

# make sure the CSS parser exists.
CSS_AVAILABLE = False
import os
from warnings import warn

def __mktimestamp():
    mtime = unicode(os.stat(SYNTAX_FILE).st_mtime)
    mtime += unicode(os.stat(LANGUAGE_FILE).st_mtime)
    return mtime

def buildCSSParser():
    tooOld = False
    try:
        try:
            mtime = __mktimestamp()
        except OSError:
            return False
        try:
            import GeneratedParser
        except SyntaxError as err:
            raise ImportError(err)
        if not hasattr(GeneratedParser, "__version__"):
            raise TooOld()
        if GeneratedParser.__version__ != unicode(mtime):
            raise TooOld()
        return True
    except TooOld as err:
        tooOld = True
    except ImportError as err:
        pass
    
    # try to find the parser generator, otherwise fail
    import sys
    
    path = os.path.join(os.getcwd(), "utils", "pyLR1")
    sys.path.insert(1, path)
    try:
        try:
            import pyLRp
        except ImportError as err:
            warn(ImportWarning("Could not generate CSS parser: {0}".format(err)))
            print("Exception occured while trying to find the parser generator. Did you run a git submodule init && git submodule update?")
            return tooOld
        # rebuild the parser
        syntaxFile = open(SYNTAX_FILE, "r")
        parser = pyLRp.Parser(syntaxFile)
        syntax = parser.Parse()
        syntaxFile.close()
        del syntaxFile
        del parser

        graph = pyLRp.LALR1StateTransitionGraph(syntax)
        graph.Construct()

        lexingNFA = pyLRp.LexingNFA(syntax)
        lexingNFA.Construct()
        lexingDFA = lexingNFA.CreateDFA()
        del lexingNFA
        lexingDFA.Optimize()

        lexingTable = lexingDFA.CreateLexTable()
        del lexingDFA

        lexingTable.ConstructEquivalenceClasses()
        
        parserFile = open(PARSER_FILE, "w")
        writer = pyLRp.Writer(parserFile, True, False, True, sys.version_info[0] >= 3, version=mtime)
        writer.Write(syntax, graph, lexingTable)

        del graph
        del lexingTable
        del syntax
        del writer
        parserFile.close()
        del parserFile
    finally:
        del sys.path[1]

    try:
        if tooOld:
            reload(GeneratedParser)
        else:
            import GeneratedParser
    except ImportError as err:
        warn(ImportWarning("Despite having created the parser: {0}".format(err)))

if not buildCSSParser():
    warn(UserWarning("CSS interpreter will be unavailable."))
