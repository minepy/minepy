MATLAB and OCTAVE API
=====================

.. code-block:: matlab

    function [minestats, M] = mine(x, y, alpha, c, est)
    % MINE  Maximal Information-based Nonparametric Exploration
    %
    %   Returns a struct containing MIC, MAS, MEV, MCN (eps=0) MCN_GENERAL
    %   (eps=1-MIC) and TIC.
    %
    %   MINESTATS = MINE(X, Y, ALPHA, C, EST) computes the MINE statistics
    %   between X and Y. X and Y must be row vectors of size n.
    %   Alpha is the exponent in B(n) = n^alpha and must be in (0, 1.0].
    %   Parameter c determines how many more clumps there will be than
    %   columns in every partition and must be > 0.
    %   Est is a string defining the estimation method.
    %
    %   MINESTATS = MINE(X, Y, ALPHA, C) computes the MINE statistics
    %   between X and Y. Default value of EST is 'mic_approx'.
    %
    %   MINESTATS = MINE(X, Y, ALPHA) computes the MINE statistics
    %   between X and Y. Default value of c is 15.
    %
    %   MINESTATS = MINE(X, Y) computes the MINE statistics
    %   between X and Y. Default value of alpha is 0.6 and default value
    %   of c is 15.

   
Example
-------

The example is located in ``examples/matlab_example.m``.

.. literalinclude:: ../../examples/matlab_example.m
   :language: matlab

.. code-block:: matlab

   % print the minestats structure
   minestats

   minestats =

           mic: 1
           mas: 0.7261
           mev: 1
           mcn: 4.5850
   mcn_general: 4.5850
           tic: 67.661
