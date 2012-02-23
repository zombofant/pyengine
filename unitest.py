#!/usr/bin/python2
# encoding=utf8

from __future__ import unicode_literals, print_function, division
from our_future import *

class Colors(object):
    # HEADER = '\033[95m'
    
    Header = '\033[01m'
    Warning = '\033[93m'
    Success = '\033[01;32m' 
    Skipped = '\033[94m'
    Error = '\033[01;31m'
    Failure = Error
    UnexpectedSuccess = Warning
    ExpectedFailure = Warning
    TestClass = Header
    TestName = Header
    Reset = '\033[0m'

    def disable(self):
        self.Success = ''
        self.Skipped = ''
        self.Failure = ''
        self.Error = ''
        self.UnexpectedSuccess = ''
        self.ExpectedFailure = ''
        self.TestClass = ''
        self.TestName = ''
        self.Reset = ''
        self.Header = ''
        self.Warning = ''
    
    def __call__(self, string, colour):
        return "{0}{2}{1}".format(colour, self.Reset, string)

import unittest
from unittest.util import strclass
import os
import sys
import traceback

STATE_PASS = 0
STATE_SKIP = 1
STATE_ERROR = 2
STATE_FAILURE = 3
STATE_UNEXPECTED_SUCCESS = 4
STATE_EXPECTED_FAILURE = 5

Colors = Colors()
ttyWidth = 80
if not os.isatty(sys.stdout.fileno()):
    Colors.disable()
else:
    rows, cols = os.popen('stty size', 'r').read().split()
    ttyWidth = int(cols)

loader = unittest.TestLoader()

class AwesomeTextResult(unittest.TestResult):
    success = []
    testNoteIndent = "        "
    stateNames = [
        ("ok",                  Colors.Success),
        ("skipped",             Colors.Skipped),
        ("error",               Colors.Error),
        ("failure",             Colors.Failure),
        ("unexpected success",  Colors.UnexpectedSuccess),
        ("expected failure",    Colors.ExpectedFailure)
    ]
    maxStateNameLen = len(stateNames[STATE_UNEXPECTED_SUCCESS][0])
    ttyWidth = 80
    
    def __init__(self, *args, **kwargs):
        super(AwesomeTextResult, self).__init__(*args, **kwargs)
        self._previousPath = None
        
    def _formatTestName(self, name, indent=" "*2, color=Colors.TestName):
        testNameLen = self.ttyWidth - (self.maxStateNameLen + (len(indent+" $")) - (len(color) + len(Colors.Reset)))
        return ("{1}{0:.<"+unicode(testNameLen)+"s} ").format(Colors(name, color)+" ", indent)
    
    def _printTestName(self, test):
        try:
            methodName = test._testMethodName
        except AttributeError:
            methodName = str(test).partition(" ")[0]
        if methodName[:5] == "test_":
            methodName = methodName[5:]
        elif methodName == "runTest":
            methodName = None
        modulePath = type(test).__module__.split(".")
        deepest = modulePath[-1]
        if deepest[:5] == "test_":
            modulePath[-1] = deepest[5:]
        modulePath.append(type(test).__name__)
        if methodName is None:
            print(self._formatTestName(".".join(modulePath), "", Colors.TestClass), end='')
            self.testNoteIndent = " "*4
        else:
            if self._previousPath != modulePath:
                self._previousPath = modulePath
                print("{0}:".format(Colors(".".join(modulePath), Colors.TestClass)))
            print(self._formatTestName(methodName), end='')
            self.testNoteIndent = " "*6
    
    def _printState(self, state):
        print(Colors(*self.stateNames[state]))
    
    def startTest(self, test):
        self._printTestName(test)
        super(AwesomeTextResult, self).startTest(test)

    def _skipBlank(self, s):
        for line in s.split("\n"):
            line = line.rstrip()
            if len(line) > 0:
                yield line

    def _indented(self, s, indent=None):
        indent = indent or self.testNoteIndent
        return indent + (("\n"+indent).join(self._skipBlank(s)))

    def _formatError(self, err):
        s = "".join(traceback.format_exception(*err))
        print(self._indented(s), file=sys.stderr)
    
    def addError(self, test, err):
        super(AwesomeTextResult, self).addError(test, err)
        print(Colors("error", Colors.Error))
        self._formatError(err)

    def addFailure(self, test, err):
        super(AwesomeTextResult, self).addFailure(test, err)
        print(Colors("failure", Colors.Failure))
        self._formatError(err)

    def addSuccess(self, test):
        super(AwesomeTextResult, self).addSuccess(test)
        self.success.append(test)
        print(Colors("ok", Colors.Success))

    def addSkip(self, test, reason):
        super(AwesomeTextResult, self).addSkip(test, reason)
        print(Colors("skip", Colors.Error))
        print(self.testNoteIndent+reason)

    def addExpectedFailure(self, test, err):
        super(AwesomeTextResult, self).addExpectedFailure(test, err)
        print(Colors("expected failure", Colors.ExpectedFailure))
        self._formatError(err)

    def addUnexpectedSuccess(self, test):
        super(AwesomeTextResult, self).addUnexpectedSuccess(test)
        print(Colors("unexpected success", Colors.UnexpectedSuccess))

    def _colouredNumber(self, count, nonZero="", zero=""):
        return Colors(unicode(count), zero if count == 0 else nonZero)

    def printStats(self, suite):
        passedCount = len(self.success)
        errorCount = len(self.errors)
        failureCount = len(self.failures)
        skippedCount = len(self.skipped)
        expectedFailureCount = len(self.expectedFailures)
        unexpectedSuccessCount = len(self.unexpectedSuccesses)

        passedColour = Colors.Warning
        if passedCount == self.testsRun:
            passedColour = Colors.Success
        elif passedCount == 0:
            passedColour = Colors.Failure
        
        testsTotal = suite.countTestCases()
        print("{0} ({1} tests in total):".format(Colors("Statistics", Colors.Header), testsTotal))
        print("  passed                 : {0}".format(Colors(passedCount, passedColour)))
        print("  skipped                : {0}".format(self._colouredNumber(skippedCount, Colors.Skipped, Colors.Success)))
        print("  expected failures      : {0}".format(self._colouredNumber(expectedFailureCount, Colors.ExpectedFailure, Colors.Success)))
        print("  unexpected successes   : {0}".format(self._colouredNumber(unexpectedSuccessCount, Colors.UnexpectedSuccess, Colors.Success)))
        print("  errors                 : {0}".format(self._colouredNumber(errorCount, Colors.Error, Colors.Success)))
        print("  failures               : {0}".format(self._colouredNumber(failureCount, Colors.Failure, Colors.Success)))
        print("  ran                    : {0}".format(Colors(self.testsRun, Colors.Success if self.testsRun == testsTotal else Colors.Warning)))

results = AwesomeTextResult()
results.ttyWidth = ttyWidth
tests = loader.discover(os.getcwd(), "test_*.py")
print("Running {0} unittests (detected from auto-discovery)".format(tests.countTestCases()))
tests.run(results)
results.printStats(tests)
