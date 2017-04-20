"""
myfact:setup.py
"""

from distutils.core import setup,Extension
import py2exe

myfact_module = Extension('_myfact',sources=['myfact_wrap.c','myfact.c'])

setup(name = 'myfact',
      version = '0.1',
      author = 'cx',
      description = """Simple swig example for factorial""",
      ext_modules = [myfact_module],
      py_modules = ["myfact"])
