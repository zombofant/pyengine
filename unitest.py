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
    def startTest(self, test):
        print("{1}{0}{2}... ".format(test, bcolors.EMPH, bcolors.ENDC), end='')
        super(AwesomeTextResult, self).startTest(test)

    def _indented(self, s, indent):
        return indent + (("\n"+indent).join(s.split("\n")))

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
        super(AwesomeTextResult, self).addExpectedFailure(test)
        print("{0}unexpected success{1}".format(bcolors.WARNING, bcolors.ENDC))

results = AwesomeTextResult()
tests = loader.discover(os.getcwd(), "test_*.py")
tests.run(results)
