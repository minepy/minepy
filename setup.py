from distutils.core import setup, Extension
from distutils.sysconfig import *
from distutils.util import *
import numpy

        
try:
    from distutils.command.build_py import build_py_2to3 as build_py
except ImportError:
    from distutils.command.build_py import build_py

# extension module arguments
if get_platform() == "win32":
   libraries = []
else:
   libraries = ['m']
extra_compile_args = ['-Wall']
include_dirs = [numpy.get_include()]

ext_modules = [
    Extension("minepy.mine",
              ["minepy/mine.c", "libmine/mine.c"],
              libraries=libraries,
              include_dirs=include_dirs,
              extra_compile_args=extra_compile_args)
    ]

# setup arguments
packages=['minepy']
scripts = ["minepy/scripts/mine"]
data_files = []
classifiers = [
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Science/Research',
    'License :: OSI Approved :: GNU General Public License (GPL)',
    'Programming Language :: C',
    'Programming Language :: Cython',
    'Programming Language :: Python',
    'Programming Language :: Python :: 2',
    'Programming Language :: Python :: 3',
    'Operating System :: OS Independent'
    ]

setup(name = 'minepy',
      version='1.0.1',
      description='minepy - Maximal Information-based Nonparametric Exploration',
      long_description=open('README.rst').read(),
      author='Davide Albanese',
      author_email='davide.albanese@gmail.com',
      maintainer='Davide Albanese',
      maintainer_email='davide.albanese@gmail.com',
      url='minepy.sourceforge.net',
      download_url='minepy.sourceforge.net',
      license='GPLv3',
      packages=packages,
      scripts=scripts,
      data_files=data_files,
      classifiers=classifiers,
      ext_modules=ext_modules
    )
