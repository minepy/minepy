function [minestats, M] = mine(x, y, alpha, c)
% MINE  Maximal Information-based Nonparametric Exploration
%   
%   Returns a struct containing MIC, MAS, MEV, MCN (eps=0) and
%   MCN_GENERAL (eps=1-MIC).
%
%   MINESTATS = MINE(X, Y, ALPHA, C) computes the MINE statistics
%   between X and Y. X and Y must be row vectors of size n.
%   Alpha is the exponent in B(n) = n^alpha and must be in (0, 1.0].
%   Parameter c determines how many more clumps there will be than 
%   columns in every partition and must be > 0.
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

switch nargin
    case 3
	c = 15;
    case 2
        alpha = 0.6;
        c = 15;
end

if nargout==1
    A = mine_mex(x, y, alpha, c);
else
    [A, M] = mine_mex(x,y,alpha,c);
end
minestats.mic = A(1);
minestats.mas = A(2);
minestats.mev = A(3);
minestats.mcn = A(4);
minestats.mcn_general = A(5);
