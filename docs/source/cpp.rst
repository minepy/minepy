.. _cpp_api:

C++ API
=======

This chapter describes the mine C++ wrapper. The class is declared in the header
file ``cppmine.h``, located in the ``libmine/`` folder. You need to add
``#include "cppmine.h"`` in your C++ source files and link your program with
``mine.c`` and ``cppmine.c``.

See the :ref:`c_api` documentation.


.. cpp:class:: MINE

   .. cpp:function:: MINE::MINE(double alpha, double c, int est)

      Constructor. Throws an exception when the parameters are
      invalid.

   .. cpp:function:: MINE::~MINE()

      Destructor.

   .. cpp:function:: void MINE::compute_score(double *x, double *y, int n)

   .. cpp:function:: void MINE::mic()

   .. cpp:function:: void MINE::mas()

   .. cpp:function:: void MINE::mev()

   .. cpp:function:: void MINE::mcn(double eps)

   .. cpp:function:: void MINE::mcn_general()

   .. cpp:function:: void MINE::tic(int norm)


Example
-------

The example is located in ``examples/cpp_example.cpp``.

.. literalinclude:: ../../examples/cpp_example.cpp
   :language: cpp

To compile the example, open a terminal, go into the example (``examples/``)
folder and run:

.. code-block:: sh

   $ g++ -O3 -Wall -Wno-write-strings cpp_example.cpp ../libmine/cppmine.cpp \
     ../libmine/mine.c -I../libmine/

Run the example by typing:

.. code-block:: sh

    MIC: 0.999999
    MAS: 0.728144
    MEV: 0.999999
    MCN (eps=0): 4.58496
    MCN (eps=1-MIC): 4.58496
    TIC: 67.5236

A more simple example
---------------------

The example is located in ``examples/cpp_example2.cpp``.

.. literalinclude:: ../../examples/cpp_example2.cpp
   :language: cpp

To compile the example, open a terminal, go into the example
(``examples/``) folder and run:

.. code-block:: sh

   $ g++ -O3 -Wall -Wno-write-strings cpp_example2.cpp ../libmine/cppmine.cpp \
     ../libmine/mine.c -I../libmine/

Run the example by typing:

.. code-block:: sh

    $ ./a.out
    MIC: 0.291692
