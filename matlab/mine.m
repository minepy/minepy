function [minestats, M] = mine(x, y, alpha, c, est)
% MINE  Maximal Information-based Nonparametric Exploration
%
%   Returns a struct containing MIC, MAS, MEV, MCN (eps=0) MCN_GENERAL
%   (eps=1-MIC) and TIC.
%
%   MINESTATS = MINE(X, Y, ALPHA, C, EST) computes the MINE statistics
%   between X and Y. X and Y must be row vectors of size n.
%
%   If alpha is in (0,1] then B will be max(n^alpha, 4) where n is the number of
%   samples. If alpha is >=4 then alpha defines directly the B parameter. If B
%   is higher than the number of samples (n) it will be limited to n, so
%   B = min(alpha, n).
%
%   c determines how many more clumps there will be than columns in every
%   partition, c=15 meaning that when trying to draw x grid lines on the x-axis,
%   the algorithm will start with at most 15*x clumps. c must be > 0.
%
%   estimator. With est="mic_approx" the original MINE statistics will be
%   computed, with est="mic_e" the equicharacteristic matrix is is evaluated and
%   the mic() and tic() methods will return MIC_e and TIC_e values respectively.
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
%
%   Example:
%   >> x = linspace(0, 1, 1000);
%   >> y = sin(10 * pi * x) + x;
%   >> minestats = mine(x, y)
%
%   minestats =
%
%           mic: 1
%           mas: 0.7261
%           mev: 1
%           mcn: 4.5850
%   mcn_general: 4.5850
%           tic: 67.661

if nargin<5
    est = 'mic_approx';
end

if nargin<4
    c = 15;
end

if nargin<3
    alpha = 0.6;
end

if strcmpi(est, 'mic_approx')
    e = 0;
else
    e = 1;
end

if nargout==1
    A = mine_mex(x, y, alpha, c, e);
else
    [A, M] = mine_mex(x, y, alpha, c, e);
end

minestats.mic = A(1);
minestats.mas = A(2);
minestats.mev = A(3);
minestats.mcn = A(4);
minestats.mcn_general = A(5);
minestats.tic = A(6);
