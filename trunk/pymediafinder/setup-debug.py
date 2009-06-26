#!/usr/bin/python
from distutils.core import setup, Extension

module1 = Extension('pymediafinder',
                    define_macros = [('MAJOR_VERSION', '0'),
                                     ('MINOR_VERSION', '1')],
                    include_dirs = ['../imediafinder','../gnucoreutils'],
                    libraries = ['imediafinder','gnucoreutils'],
                    library_dirs = ['../imediafinder/Debug','../gnucoreutils/Debug'],
                    sources = ['wrapper.cpp'])

setup (name = 'PyMediaFinder',
       version = '0.1',
       description = 'Media finder',
       author = 'Santiago Iturriaga',
       author_email = 'santiago@ituland.com',
       url = 'http://www.ituland.com',
       long_description = '''
Media finder.
''',
       ext_modules = [module1])
