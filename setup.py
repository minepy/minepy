from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from pkg_resources import get_platform


# import numpy only when it is needed
class build_ext_custom(build_ext):
    def run(self):
        import numpy
        self.include_dirs.append(numpy.get_include())
        build_ext.run(self)

if get_platform() == "win32" or get_platform() == "win-amd64":
    libraries = []
else:
    libraries = ['m']

ext_modules = [
    Extension("minepy.mine",
              ["minepy/mine.c", "libmine/mine.c"],
              libraries=libraries,
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
      version='1.2.5',
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
      install_requires = ['numpy >= 1.3.0'],
      classifiers=classifiers,
      ext_modules=ext_modules,
      use_2to3=False,
      cmdclass = {'build_ext': build_ext_custom}
    )
