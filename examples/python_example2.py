import numpy as np
from minepy import MINE
import time

alpha = 0.6
c = 15
n = 5000

x = np.linspace(0, 1, n)
y = np.sin(10 * np.pi * x) + x


print "Without noise"
print "-------------"

mine = MINE(alpha=alpha, c=c, est="mic_approx")
start = time.time()
mine.compute_score(x, y)
print "MIC APPROX: %fs" % (time.time() - start)

# mine = MINE(alpha=alpha, c=c, est="mic_e")
# start = time.time()
# mine.compute_score(x, y)
# print "MIC_e: %fs" % (time.time() - start)

# print "With noise"
# print "----------"

# np.random.seed(0)
# y +=np.random.uniform(-1, 1, x.shape[0]) # add some noise

# mine = MINE(alpha=alpha, c=c, est="mic_approx")
# start = time.time()
# mine.compute_score(x, y)
# print "MIC APPROX: %fs" % (time.time() - start)

# mine = MINE(alpha=alpha, c=c, est="mic_e")
# start = time.time()
# mine.compute_score(x, y)
# print "MIC_e: %fs" % (time.time() - start)

