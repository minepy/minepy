from setuptools import setup, Extension
from pkg_resources import get_platform
import numpy


if get_platform() == "win32" or get_platform() == "win-amd64":
    libraries = []
else:
    libraries = ['m']

ext_modules = [
    Extension("minepy.mine",
              ["minepy/mine.c", "libmine/mine.c"],
              libraries=libraries,
              include_dirs=[numpy.get_include()],
              extra_compile_args=['-Wall'])
    ]

classifiers = [
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Science/Research',
    'License :: OSI Approved :: GNU General Public License (GPL)',
    'Programming Language :: C',
    'Programming Language :: C++',
    'Programming Language :: Cython',
    'Programming Language :: Python',
    'Programming Language :: Python :: 2',
    'Programming Language :: Python :: 3',
    'Programming Language :: Other',
    'Operating System :: OS Independent'
    ]

setup(name = 'minepy',
      version='1.3.0',
      description='minepy - Maximal Information-based Nonparametric Exploration',
      long_description=open('README.rst').read(),
      author='Davide Albanese',
      author_email='davide.albanese@gmail.com',
      maintainer='Davide Albanese',
      maintainer_email='davide.albanese@gmail.com',
      url='http://minepy.readthedocs.io',
      download_url='http://minepy.readthedocs.io',
      license='GPLv3',
      packages=['minepy'],
      scripts=["minepy/scripts/mine"],
      install_requires = ['numpy >= 1.3.0'],
      classifiers=classifiers,
      ext_modules=ext_modules,
      use_2to3=True}
    )
