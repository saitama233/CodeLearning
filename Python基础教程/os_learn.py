import sys
import os

print __file__
print sys.argv[0]

print os.path.abspath(__file__)
print os.path.dirname(__file__)
print os.path.dirname(os.path.abspath(__file__))
print os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

print os.pardir
print os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir))
os.chdir(os.path.normpath(os.path.join(os.path.abspath(__file__), os.pardir)))
print os.getcwd()
