#!/usr/bin/python2
# encoding=utf8

from __future__ import unicode_literals, print_function, division
from our_future import *

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    PASSED = '\033[01;32m'
    WARNING = '\033[93m'
    FAILED = '\033[01;31m'
    EMPH = '\033[01m'
    ENDC = '\033[0m'

    def disable(self):
        self.EMPH = ''
        self.HEADER = ''
        self.OKBLUE = ''
        self.PASSED = ''
        self.WARNING = ''
        self.CANNOT_PASS = ''
        self.ENDC = ''

import unittest
import os
import sys
import traceback

if not os.isatty(sys.stdin.fileno()):
    bcolors.disable()

loader = unittest.TestLoader()

class AwesomeTextResult(unittest.TestResult):
    success = []
    
    def startTest(self, test):
        print("{1}{0}{2}... ".format(test, bcolors.EMPH, bcolors.ENDC), end='')
        super(AwesomeTextResult, self).startTest(test)

    def _skipBlank(self, s):
        for line in s.split("\n"):
            line = line.rstrip()
            if len(line) > 0:
                yield line

    def _indented(self, s, indent):
        return indent + (("\n"+indent).join(self._skipBlank(s)))

    def _formatError(self, err):
        s = "".join(traceback.format_exception(*err))
        print(self._indented(s, "    "))
    
    def addError(self, test, err):
        super(AwesomeTextResult, self).addError(test, err)
        print("{0}error{1}".format(bcolors.FAILED, bcolors.ENDC))
        self._formatError(err)

    def addFailure(self, test, err):
        super(AwesomeTextResult, self).addFailure(test, err)
        print("{0}failure{1}".format(bcolors.FAILED, bcolors.ENDC))
        self._formatError(err)

    def addSuccess(self, test):
        super(AwesomeTextResult, self).addSuccess(test)
        self.success.append(test)
        print("{0}ok{1}".format(bcolors.PASSED, bcolors.ENDC))

    def addSkip(self, test, reason):
        super(AwesomeTextResult, self).addSkip(test, reason)
        print("{0}skipped{1}".format(bcolors.OKBLUE, bcolors.ENDC))
        print("    "+reason)

    def addExpectedFailure(self, test, err):
        super(AwesomeTextResult, self).addExpectedFailure(test, err)
        print("{0}expected failure{1}".format(bcolors.WARNING, bcolors.ENDC))
        self._formatError(err)

    def addUnexpectedSuccess(self, test):
        super(AwesomeTextResult, self).addUnexpectedSuccess(test)
        print("{0}unexpected success{1}".format(bcolors.WARNING, bcolors.ENDC))

    def _colouredNumber(self, count, nonZero="", zero=""):
        return "{0}{2}{1}".format(zero if count == 0 else nonZero, bcolors.ENDC, count)

    def printStats(self, suite):
        passedCount = len(self.success)
        errorCount = len(self.errors)
        failureCount = len(self.failures)
        skippedCount = len(self.skipped)
        expectedFailureCount = len(self.expectedFailures)
        unexpectedSuccessCount = len(self.unexpectedSuccesses)

        passedColour = bcolors.WARNING
        if passedCount == self.testsRun:
            passedColour = bcolors.PASSED
        elif passedCount == 0:
            passedColour = bcolors.FAILED
        
        print("{0}Statistics:{1}".format(bcolors.EMPH, bcolors.ENDC))
        print("  passed                 : {0}{2}{1}".format(passedColour, bcolors.ENDC, passedCount))
        print("  skipped                : {0}".format(self._colouredNumber(skippedCount, bcolors.OKBLUE, bcolors.PASSED)))
        print("  expected failures      : {0}".format(self._colouredNumber(expectedFailureCount, bcolors.WARNING, bcolors.PASSED)))
        print("  unexpected successes   : {0}".format(self._colouredNumber(unexpectedSuccessCount, bcolors.WARNING, bcolors.PASSED)))
        print("  errors                 : {0}".format(self._colouredNumber(errorCount, bcolors.FAILED, bcolors.PASSED)))
        print("  failures               : {0}".format(self._colouredNumber(failureCount, bcolors.FAILED, bcolors.PASSED)))
        print("  total                  : {0}".format(self.testsRun))

results = AwesomeTextResult()
tests = loader.discover(os.getcwd(), "test_*.py")
tests.run(results)
results.printStats(tests)
