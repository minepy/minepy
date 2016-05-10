Download and Install
====================

C and C++ users
---------------
Requirements:

* GCC

Download latest version from https://github.com/minepy/minepy/releases. No
installation is required. See :ref:`c_api` and :ref:`cpp_api` on how to
include the library in your C/C++ software and how to compile.

Python users
------------

Requirements:

* GCC
* Python >= 2.7
* NumPy >= 1.3.0 (with header files)

On Linux
^^^^^^^^

We suggest to install the GCC compiler through the package manager (e.g. on
Ubuntu/Debian):

.. code-block:: sh

    sudo apt-get update
    sudo apt-get install build-essential python-dev

Then, upgrade pip and install setuptools:

.. code-block:: sh

    pip install --upgrade pip
    pip install 'setuptools >=14.0'

Finally, install micca:

.. code-block:: sh

    sudo pip install minepy

On Mac OS X
^^^^^^^^^^^

In Mac OS X, we recommend to install Python from `Homebrew <http://brew.sh/>`_:

   #. Install `Xcode <https://developer.apple.com/xcode/>`_;
   #. Install `Homebrew <http://brew.sh/>`_;
   #. Make sure the environment variable ``PATH`` is properly setted in your
      ``~/.bash_profile`` or ``~/.bashrc``::

      .. code-block:: sh
         export PATH=/usr/local/bin:$PATH

   #. Install Python:

      .. code-block:: sh

         brew update
         brew install python

Install gcc:

.. code-block:: sh

    brew install gcc

Finally, install minepy:

.. code-block:: sh

    sudo pip install minepy

Running the tests:

   .. code-block:: sh

      $ cd tests
      $ python minepy_test.py
      test_const (__main__.TestFunctions) ... ok
      test_exp (__main__.TestFunctions) ... ok
      test_linear (__main__.TestFunctions) ... ok
      test_sine (__main__.TestFunctions) ... ok

      ----------------------------------------------------------------------
      Ran 4 tests in 0.412s

      OK


MATLAB users (Windows, Linux and Mac OS X)
------------------------------------------
The library works with MATLAB >= 7.3 (R2006b) since it supports 64-Bit indexing.
See http://www.mathworks.com/help/matlab/matlab_external/upgrading-mex-files-to-use-64-bit-api.html.
Download latest version from https://github.com/minepy/minepy/releases.

1. Untar the file ``minepy-X.Y.Z.tar.gz`` (where ``X.Y.Z`` is the current
   version of minepy)

2. Open MATLAB

3. In the MATLAB "command window" go into the ``minepy-X.Y.Z`` folder by
   typing:

   .. code-block:: sh

      >> cd path_to_minepy-X.Y.Z/matlab/

4. Build the binary MEX file by typing:

   .. code-block:: sh

      >> mex mine_mex.c ../libmine/mine.c

5. Now you have the binary MEX-file in ``path_to_minepy-X.Y.Z/matlab/``
   (``mine_mex.mex*``, where * can be ``a64``, ``maci64``, ``w32`` or ``w64``)

6. Put your MEX-file (``mine_mex.mex*``) and ``mine.m`` in a folder on the
   MATLAB path. Alternatively, you can add ``path_to_minepy-X.Y.Z/matlab/``
   selecting File > SetPath.

7. Test the MEX-file by typing:

   .. code-block:: sh

      >> minestats = mine([1,2,3,4,5,6], [1,2,3,4,5,6])


OCTAVE users (Windows, Linux and Mac OS X)
------------------------------------------
Download latest version from https://github.com/minepy/minepy/releases.

1. Untar the file ``minepy-X.Y.Z.tar.gz`` (where ``X.Y.Z`` is the current
   version of minepy)

2. Run OCTAVE

3. Go into the ``minepy-X.Y.Z`` folder by typing:

   .. code-block:: sh

      octave:1> cd path_to_minepy-X.Y.Z/matlab/

4. Build the binary MEX-file by typing:

   .. code-block:: sh

      octave:2> mex mine_mex.c ../libmine/mine.c

5. Now, you have the binary MEX-file in
   ``path_to_minepy-X.Y.Z/matlab/`` (``mine_mex.mex``)

6. Put your MEX-file (``mine_mex.mex``) and ``mine.m`` in a folder on the OCTAVE
   path. Alternatively, you can add ``path_to_minepy-X.Y.Z/matlab/`` in the
   OCTAVE startup file
   (http://www.gnu.org/software/octave/doc/interpreter/Startup-Files.html)

7. Test the MEX-file by typing:

   .. code-block:: sh

      octave:3> minestats = mine([1,2,3,4,5,6], [1,2,3,4,5,6])
