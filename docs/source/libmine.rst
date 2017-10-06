.. _c_api:

C API
=====

This chapter describes the mine C library. These functions and structures are
declared in the header file ``mine.h``, located in the ``libmine/`` folder.
You need to add ``#include "mine.h"`` in your C source files and link your
program with ``mine.c``.

Defines
-------

.. c:var:: EST_MIC_APPROX 0

   Original estimator described in DOI: 10.1126/science.1205438.

.. c:var:: EST_MIC_E 1

   Estimator described in DOI: arXiv:1505.02213 and DOI: arXiv:1505.02214.

.. c:var:: FALSE 0

   False value.

.. c:var:: TRUE 1

   True value.

.. c:var:: char *libmine_version

   The libmine version in the form X.Y.Z (e.g., 1.0.1).

Structures
----------

.. c:type:: mine_problem

  The mine_problem structure describes the problem. x and y are the two
  variables of length n.

  .. code-block:: c

    typedef struct mine_problem
      {
        int n;
        double *x;
        double *y;
      } mine_problem

.. c:type:: mine_parameter

  MINE parameters.

  alpha : (0, 1.0] or >=4
          if alpha is in (0,1] then B will be max(n^alpha, 4) where n is the
          number of samples. If alpha is >=4 then alpha defines directly the B
          parameter. If alpha is higher than the number of samples (n) it will
          be limited to be n, so B = min(alpha, n).
  c : (> 0)
          determines how many more clumps there will be than columns in
          every partition. Default value is 15, meaning that when trying to
          draw x grid lines on the x-axis, the algorithm will start with at
          most 15*x clumps.
  est : (EST_MIC_APPROX, EST_MIC_E)
          estimator. With est=EST_MIC_APPROX the original MINE statistics will
          be computed, with est=EST_MIC_E the equicharacteristic matrix is
          is evaluated and the mine_mic() and mine_tic() functions will return
          MIC_e and TIC_e values respectively.

  .. code-block:: c

    typedef struct mine_parameter
      {
        double alpha;
        double c;
        int est;
      } mine_parameter


.. c:type:: mine_score

  The mine_score structure contains the maximum normalized mutual information
  scores (i.e. the characteristic matrix if est=EST_MIC_APPROX, the
  equicharacteristic matrix instead). M[i][j] contains the score using a grid
  partitioning x-values into i+2 bins and y-values into j+2 bins. m and M are
  of length n and each M[i] is of length m[i].

  .. code-block:: c

    typedef struct mine_score
      {
        int n;      /* number of rows of M */
        int *m;     /* number of cols of M[i] for each i */
        double **M; /* the (equi)characteristic matrix */
      } mine_score

Functions
---------

.. c:function:: mine_score *mine_compute_score(mine_problem *prob, mine_parameter *param)

   Computes the (equi)characteristic matrix (i.e. maximum normalized mutual
   information scores) and returns a mine_score structure. Returns NULL if an
   error occurs.

.. c:function:: char *mine_check_parameter(mine_parameter *param)

   Checks the parameters. This function should be called before calling
   mine_compute_score(). It returns NULL if the parameters are feasible,
   otherwise an error message is returned. See the mine_parameter documentation.

.. c:function:: double mine_mic(mine_score *score)

   Returns the Maximal Information Coefficient (MIC or MIC_e).

.. c:function:: double mine_mas(mine_score *score)

   Returns the Maximum Asymmetry Score (MAS).

.. c:function:: double mine_mev(mine_score *score)

   Returns the Maximum Edge Value (MEV).

.. c:function:: double mine_mcn(mine_score *score, double eps)

   Returns the Minimum Cell Number (MCN) with \epsilon >= 0.

.. c:function:: double mine_mcn_general(mine_score *score)

   Returns the Minimum Cell Number (MCN) with eps=1 - MIC.

.. c:function:: double mine_tic(mine_score *score, int norm)

   Returns the Total Information Coefficient (TIC or TIC_e). norm=TRUE
   normalizes the Total Information Coefficient, returning values in [0, 1].

.. c:function:: double mine_gmic(mine_score *score)

   Returns the Generalized Mean Information Coefficient (GMIC).

.. c:function:: void mine_free_score(mine_score **score)

   This function frees the memory used by a mine_score and destroys the score
   structure.


Convenience structures and functions
------------------------------------

.. c:type:: mine_matrix

  Mine matrix, variables x samples.

  .. code-block:: c

    typedef struct mine_matrix
      {
        double *data; /* matrix in row-major order */
        int n;        /* number of rows */
        int m;        /* number of cols */
      } mine_matrix


.. c:type:: mine_pstats

  For each statistic, the upper triangle of the matrix is stored by row
  (condensed matrix). If m is the number of variables, then for i<j<m, the
  statistic between (row) i and j is stored in k = m*i - i*(i+1)/2 - i - 1 + j.
  The length of the vectors is n = m*(m-1)/2.

  .. code-block:: c

    typedef struct mine_pstats
      {
        double *mic; /* condensed matrix */
        double *tic; /* condensed matrix */
        int n;       /* number of elements */
      } mine_pstats

.. c:type:: mine_cstats

  For each statistic, the matrix is stored by row. If n and m are the number of
  variables in X and Y respectively, then the statistic between the (row) i
  (for X) and j (for Y) is stored in k = i*m + j. The length of the vector is
  d = n*m.

    .. code-block:: c

      typedef struct mine_cstats
        {
          double *mic; /* matrix in row-major order */
          double *tic; /* matrix in row-major order */
          int n;       /* number of rows */
          int m;       /* number of cols */
        } mine_cstats


.. c:function:: mine_pstats *mine_compute_pstats(mine_matrix *X, mine_parameter *param)

  Compute pairwise statistics (MIC and normalized TIC) between variables.

.. c:function:: mine_cstats *mine_compute_cstats(mine_matrix *X, mine_matrix *Y, mine_parameter *param)

  Compute statistics (MIC and normalized TIC) between each pair of the two
  collections of variables.


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

Example (convenience functions)
-------------------------------

The example is located in ``examples/c_conv_example.c``.

.. literalinclude:: ../../examples/c_conv_example.c
   :language: c

To compile the example, open a terminal, go into the example
(``examples/``) folder and run:

.. code-block:: sh

   $ gcc c_conv_example.c -O3 -Wall ../libmine/mine.c -I../libmine/ -lm

Run the example by typing:

.. code-block:: sh

    $ ./a.out
    libmine version 1.2.0

         X[1]  X[2]  X[3]  X[4]  X[5]  X[6]  X[7]
    X[0] 0.008 0.011 0.011 0.013 0.011 0.007 0.010
    X[1]       0.010 0.012 0.015 0.020 0.010 0.011
    X[2]             0.015 0.016 0.013 0.012 0.019
    X[3]                   0.016 0.016 0.012 0.017
    X[4]                         0.010 0.013 0.016
    X[5]                               0.010 0.015
    X[6]                                     0.018

         Y[0]  Y[1]  Y[2]  Y[3]
    X[0] 0.012 0.012 0.016 0.010
    X[1] 0.010 0.011 0.013 0.014
    X[2] 0.013 0.011 0.012 0.016
    X[3] 0.016 0.009 0.014 0.011
    X[4] 0.007 0.022 0.018 0.017
    X[5] 0.011 0.015 0.015 0.009
    X[6] 0.012 0.007 0.015 0.013
    X[7] 0.017 0.010 0.010 0.009
