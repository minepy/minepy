Download and Install
====================

Download
--------
Download latest version from  http://sourceforge.net/projects/minepy/files/.

C and C++ users
---------------
No installation is required. See :ref:`c_api` and :ref:`cpp_api` on how to
include the library in your C/C++ software and how to compile.

Python users - Installing on \*nix from source
----------------------------------------------
On GNU/Linux, OSX and FreeBSD you need the following requirements:

   * GCC
   * Python >= 2.6 or 3.X
   * NumPy >= 1.3.0 (with header files)

1. Untar ``minepy-X.Y.Z.tar.gz``, creating ``minepy-X.Y.Z`` folder (where
   ``X.Y.Z`` is the current version of minepy)

2. Go into ``minepy-X.Y.Z`` folder and from a terminal run:

   .. code-block:: sh

      $ sudo python setup.py install

   If you have installation problems on MAC OS X try:

   .. code-block:: sh

      $ sudo ARCHFLAGS="-arch i386 -arch x86_64" python setup.py install

3. If you don't have root access, installing minepy in a directory by
   specifying the ``--prefix`` argument. Then you need to set
   ``PYTHONPATH``:

   .. code-block:: sh

      $ python setup.py install --prefix=/path/to/modules
      $ export PYTHONPATH=$PYTHONPATH:/path/to/modules/lib/python{version}/site-packages

4. Running the tests:

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

1. Untar ``minepy-X.Y.Z.tar.gz``, creating ``minepy-X.Y.Z`` folder (where
   ``X.Y.Z`` is the current version of minepy)

2. Open MATLAB

3. In the MATLAB "command window" go into the ``minepy-X.Y.Z`` folder by
   typing:

   .. code-block:: sh

      >> cd path_to_minepy-X.Y.Z/matlab/

4. Build the binary MEX file by typing:

   .. code-block:: sh

      >> mex mine_mex.c ../libmine/mine.c

5. Now, you have the binary MEX-file in ``path_to_minepy-X.Y.Z/matlab/``
   (``mine_mex.mex*``, where * can be ``a64``, ``maci64``, ``w32`` or ``w64``)

6. Put your MEX-file (``mine_mex.mex*``) and ``mine.m`` in a folder on the
   MATLAB path. Alternatively, you can add
   ``path_to_minepy-X.Y.Z/matlab/`` selecting File > SetPath.

7. Test the MEX-file by typing:

   .. code-block:: sh

      >> minestats = mine([1,2,3,4,5,6], [1,2,3,4,5,6])

      minestats =

    	  mic: 1
    	  mas: 0
          mev: 1
          mcn: 2
	  mcn_general: 2

OCTAVE users (Windows, Linux and Mac OS X)
------------------------------------------
1. Untar ``minepy-X.Y.Z.tar.gz``, creating ``minepy-X.Y.Z`` folder
   (where ``X.Y.Z`` is the current version of minepy)

2. Run OCTAVE

3. Go into the ``minepy-X.Y.Z`` folder by typing:

   .. code-block:: sh

      octave:1> cd path_to_minepy-X.Y.Z/matlab/

4. Build the binary MEX-file by typing:

   .. code-block:: sh

      octave:2> mex mine_mex.c ../libmine/mine.c

5. Now, you have the binary MEX-file in
   ``path_to_minepy-X.Y.Z/matlab/`` (``mine_mex.mex``)

6. Put your MEX-file (``mine_mex.mex``) and ``mine.m`` in a folder on
   the OCTAVE path. Alternatively, you can add
   ``path_to_minepy-X.Y.Z/matlab/`` in the OCTAVE startup file
   (http://www.gnu.org/software/octave/doc/interpreter/Startup-Files.html)

7. Test the MEX-file by typing:

   .. code-block:: sh

      octave:3> minestats = mine([1,2,3,4,5,6], [1,2,3,4,5,6])
      minestats =
      {
      mic =  1
      mas = 0
      mev =  1
      mcn =  2
      mcn_general =  2
      }
