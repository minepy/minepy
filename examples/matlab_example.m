% create x = [0, 0.001, 0.002, ..., 0.998, 0.999, 1]
x = linspace(0, 1, 1001);
% y = sin(10 * pi * x) + x
y = sin(10 * pi * x) + x;
% compute the mine statistics
minestats = mine(x, y);

