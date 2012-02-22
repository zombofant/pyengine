import unittest

class TestUnittest(unittest.TestCase):
    @unittest.expectedFailure
    def test_expectedFailure(self):
        self.assertTrue(False)

    @unittest.skip("Because we can")
    def test_skip(self):
        pass

    @unittest.expectedFailure
    def test_unexpectedSuccess(self):
        pass

    def test_pass(self):
        pass

    def test_error(self):
        raise Exception()

    def test_failure(self):
        self.assertTrue(False)

# Only for testing the unittest framework output. Remove del to test
# output for all cases.
del TestUnittest
