import unittest
import my_path
# from unittest import TestCase

class ProductTestCase(unittest.TestCase):
    def test(self):
        for x in xrange(-10, 10):
            p = my_path.square(x)
            self.failUnless(p == x * x, 'test failed')


if __name__ == '__main__':
    unittest.main()
