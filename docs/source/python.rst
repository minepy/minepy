	.. py:module:: minepy

Python API
==========

.. class:: MINE(alpha=0.6, c=15, est="mic_approx")

	Maximal Information-based Nonparametric Exploration.

	:Parameters:

		alpha : float (0, 1.0]
				the exponent in B(n) = n^alpha.
		c : float (> 0)
				determines how many more clumps there will be than columns in
				every partition. Default value is 15, meaning that when trying to
				draw x grid lines on the x-axis, the algorithm will start with at
				most 15*x clumps.
		est : str ("mic_approx", "mic_e")
				estimator. With est="mic_approx" the original MINE statistics will
				be computed, with est="mic_e" the equicharacteristic matrix is
				is evaluated and the mic() and tic() methods will return MIC_e and
				TIC_e values respectively.

  .. automethod:: compute_score(x, y)
  .. automethod:: mic()
  .. automethod:: mas()
  .. automethod:: mev()
  .. automethod:: mcn(eps=0)
  .. automethod:: mcn_general()
  .. automethod:: gmic()
  .. automethod:: tic()
  .. automethod:: get_score()
  .. automethod:: computed()

First Example
-------------
The example is located in ``examples/python_example.py``.

.. literalinclude:: ../../examples/python_example.py
   :language: python

Run the example:

.. code-block:: sh

   $ python python_example.py
   Without noise:
   MIC 1.0
   MAS 0.726071574374
   MEV 1.0
   MCN (eps=0) 4.58496250072
   MCN (eps=1-MIC) 4.58496250072
   GMIC 0.779360251901
   TIC 67.6612295532

   With noise:
   MIC 0.505716693417
   MAS 0.365399904262
   MEV 0.505716693417
   MCN (eps=0) 5.95419631039
   MCN (eps=1-MIC) 3.80735492206
   GMIC 0.359475501353
   TIC 28.7498326953

Second Example
--------------
The example is located in ``examples/relationships.py``.

.. warning::
   	Requires the `matplotlib <http://matplotlib.org>`_ library.

.. literalinclude:: ../../examples/relationships.py
   	:language: python

.. figure:: images/relationships.png
    :align: center
