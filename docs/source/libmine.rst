.. _c_api:

C API
=====

This chapter describes the mine C library. These functions and structures are
declared in the header file ``mine.h``, located in the ``libmine/`` folder.
You need to add ``#include "mine.h"`` in your C source files and link your
program with ``mine.c``.

.. c:var:: EST_MIC_APPROX 0

   Original estimator described in DOI: 10.1126/science.1205438.

.. c:var:: EST_MIC_E 1

   Estimator described in DOI: arXiv:1505.02213 and DOI: arXiv:1505.02214.

.. c:var:: char *libmine_version

   The libmine version in the form X.Y.Z (e.g., 1.0.1).


.. c:type:: mine_problem

   The mine_problem structure describes the problem. x and y are the two
   variables of length n.

   .. code-block:: c

      typedef struct mine_problem {
         int n;
  	     double *x;
  	     double *y;
      } mine_problem;


.. c:type:: mine_parameter

   The mine_parameter structure describes the MINE parameters. Alpha is the
   exponent in B(n) = n^alpha and must be in (0,1] and c determines how many
   more clumps there will be than columns in every partition. c = 15 meaning
   that when trying to draw x grid lines on the x-axis, the algorithm will
   start with at most 15*x clumps. c must be > 0. Est is the estimator,
   EST_MIC_APPROX and EST_MIC_E. With est=EST_MIC_APPROX, the mine_*()
   functions return the original statistics, with est=EST_MIC_E the
   equicharacteristic matrix is computed and mine_mic() and mine_tic() will
   return MIC_e and TIC_e respectively.

   .. code-block:: c

      typedef struct mine_parameter {
         double alpha;
         double c;
	       int est;
      } mine_parameter;


.. c:type:: mine_score

   The mine_score structure describes the maximum normalized mutual information
   scores (i.e. the characteristic matrix if est=EST_MIC_APPROX, the
   equicharacteristic matrix instead). M[i][j] contains the score using a grid
   partitioning x-values into i+2 bins and y-values into j+2 bins. m and M are
   of length n and each M[i] is of length m[i].

   .. code-block:: c

      typedef struct mine_score {
          int n;      /* number of rows of M */
          int *m;     /* number of cols of M[i] for each i */
	        double **M; /* the (equi)characteristic matrix */
      } mine_score;


.. c:function:: mine_score *mine_compute_score(mine_problem *prob, mine_parameter *param)

   Computes the (equi)characteristic matrix (i.e. maximum normalized mutual
   information scores and returns a mine_score structure. Returns NULL if an
   error occurs.

.. c:function:: char *mine_check_parameter(mine_parameter *param)

   Checks the parameters. This function should be called before calling
   mine_compute_score(). It returns NULL if the parameters are feasible,
   otherwise an error message is returned.

.. c:function:: double mine_mic(mine_score *score)

   Returns the Maximal Information Coefficient (MIC or MIC_e).

.. c:function:: double mine_mas(mine_score *score)

   Returns the Maximum Asymmetry Score (MAS).

.. c:function:: double mine_mev(mine_score *score)

   Returns the Maximum Edge Value (MEV).

.. c:function:: double mine_mcn(mine_score *score, double eps)

   Returns the Minimum Cell Number (MCN) with :math:`\epsilon \geq 0`.

.. c:function:: double mine_mcn_general(mine_score *score)

   Returns the Minimum Cell Number (MCN) with :math:`\epsilon=1 - \mathrm{MIC}`.

.. c:function:: double mine_gmic(mine_score *score)

   Returns the Generalized Mean Information Coefficient (GMIC).

.. c:function:: double mine_tic(mine_score *score)

   Returns the Total Information Coefficient (TIC or TIC_e).

.. c:function:: void mine_free_score(mine_score **score)

   This function frees the memory used by a :c:type:`mine_score`
   and destroys the score structure.


Example
-------

The example is located in ``examples/c_example.c``.

.. literalinclude:: ../../examples/c_example.c
   :language: c

To compile the example, open a terminal, go into the example
(``examples/``) folder and run:

.. code-block:: sh

   $ gcc c_example.c -Wall ../libmine/mine.c -I../libmine/ -lm

or

.. code-block:: sh

   $ gcc c_example.c -O3 -Wall ../libmine/mine.c -I../libmine/ -lm

for an extensive optimization. Run the example by typing:

.. code-block:: sh

    $ ./a.out
    libmine version 1.1.0
    Elapsed time: 0.011288 seconds

    Some MINE statistics:

    MIC: 1.000
    MEV: 1.000
    MCN (eps=0): 4.585

    Characteristic Matrix:

    0.108 0.146 0.226 0.347 0.434 0.545 0.639 0.740 0.863 0.932 1.000
    0.199 0.138 0.169 0.256 0.298 0.379 0.427
    0.237 0.190 0.217 0.286 0.324
    0.247 0.198 0.191
    0.262 0.213 0.232
    0.272 0.225
    0.286 0.237
    0.296
    0.308
    0.321
    0.333
