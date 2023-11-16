import numpy as np
from minepy import pstats, cstats
import time

np.random.seed(0)

# build the X matrix, 8 variables, 320 samples
X = np.random.rand(8, 320)

# build the Y matrix, 4 variables, 320 samples
Y = np.random.rand(4, 320)

# compute pairwise statistics MIC_e and normalized TIC_e between samples in X,
# B=9, c=5
mic_p, tic_p =  pstats(X, alpha=9, c=5, est="mic_e")

# compute statistics between each pair of samples in X and Y
mic_c, tic_c =  cstats(X, Y, alpha=9, c=5, est="mic_e")

print("normalized TIC_e (X):")
print(tic_p)
print("MIC_e (X vs. Y):")
print(mic_c)
