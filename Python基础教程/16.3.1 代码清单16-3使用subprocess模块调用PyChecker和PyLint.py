# coding=utf-8
# 当square函数有错误时会报错
import unittest,my_path
from subprocess import Popen,PIPE

class ProductTestCase(unittest.TestCase):
    def testWithPyChecker(self):
        cmd = 'pychecker','-Q',my_path.__file__.rstrip('c')
        pychecker = Popen(cmd,stdout=PIPE,stderr=PIPE)
        self.assertEqual(pychecker.stdout.read(),'')
    def testWithPyLint(self):
        cmd = 'pylint','-rn','my_path'
        pylint = Popen(cmd,stdout=PIPE,stderr=PIPE)
        self.assertEqual(pylint.stdout.read(),'')

if __name__ == '__mian__':
    unittest.main()