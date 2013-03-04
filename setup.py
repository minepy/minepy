from distutils.core import setup, Extension
from distutils.sysconfig import *
from distutils.util import *
import numpy

        
try:
    from distutils.command.build_py import build_py_2to3 as build_py
except ImportError:
    from distutils.command.build_py import build_py

if get_platform() == "win32":
   libraries = []
else:
   libraries = ['m']
    
extra_compile_args = ['-Wall']
include_dirs = [get_python_inc(), numpy.get_include()]
ext_modules = [
    Extension("minepy.mine",
              ["minepy/mine.c", "libmine/mine.c"],
              libraries=libraries, include_dirs=include_dirs,
              extra_compile_args=extra_compile_args)
    ]

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
      version='1.0.0',
      description='minepy - Maximal Information-based Nonparametric Exploration',
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
      ext_modules=ext_modules,
      requires=['numpy (>=1.3.0)']
      )
