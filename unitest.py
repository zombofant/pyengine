#!/usr/bin/python2
# encoding=utf8

from __future__ import unicode_literals, print_function, division
from our_future import *

import unittest
import os

loader = unittest.TestLoader()
result = unittest.TestResult()
tests = loader.discover(os.getcwd(), "test_*.py")
tests.run(result)
print(result)
