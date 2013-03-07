# File name: __init__.py
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

def build_css_parser():
    tooold = False
    try:
        try:
            mtime = __mktimestamp()
        except OSError:
            mtime = None
        try:
            import GeneratedParser
        except SyntaxError as err:
            raise ImportError(err)
        if not hasattr(GeneratedParser, "__version__"):
            raise TooOld()
        if mtime is not None and GeneratedParser.__version__ != unicode(mtime):
            raise TooOld()
        return True
    except TooOld as err:
        tooold = True
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
            return tooold
        # rebuild the parser
        syntaxfile = open(SYNTAX_FILE, "r")
        parser = pyLRp.Parser(syntaxfile)
        syntax = parser.Parse()
        syntaxfile.close()
        del syntaxfile
        del parser

        graph = pyLRp.LALR1StateTransitionGraph(syntax)
        graph.Construct()

        lexing_nfa = pyLRp.LexingNFA(syntax)
        lexing_nfa.Construct()
        lexing_dfa = lexing_nfa.CreateDFA()
        del lexing_nfa
        lexing_dfa.Optimize()

        lexing_table = lexing_dfa.CreateLexTable()
        del lexing_dfa

        lexing_table.ConstructEquivalenceClasses()
        
        parserfile = open(PARSER_FILE, "w")
        writer = pyLRp.Writer(parserfile, True, False, True, sys.version_info[0] >= 3, version=mtime)
        writer.Write(syntax, graph, lexing_table)

        del graph
        del lexing_table
        del syntax
        del writer
        parserfile.close()
        del parserfile
    finally:
        del sys.path[1]

    try:
        if tooold:
            reload(GeneratedParser)
        else:
            import GeneratedParser
    except ImportError as err:
        warn(ImportWarning("Despite having created the parser: {0}".format(err)))

if not build_css_parser():
    warn(UserWarning("CSS interpreter will be unavailable."))
