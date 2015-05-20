from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
from minepy import MINE

#The power of a statistical test is the probability that it correctly
#rejects the null hypothesis when the null hypothesis is false.

n_null = 500 # number of null datasets to estimate rejection regions
n_alt = 500 # number of alternative datasets
n_noise = 30
noise = 3
n = 320 # number of data points
mine_alpha = 0.6
mine_c = 15

f_names = ["Linear", "Quadratic", "Cubic", "Sine: period 1/2",
          "Sine: period 1/8", "X^(1/4)", "Circle", "Step function"]

def f1():
    return x + noise * (i/n_noise) * r

def f2():
    return 4 * (x-0.5)**2 + noise * (i/n_noise) * r

def f3():
    return 128 * (x-1/3)**3 - 48 * (x-1/3)**3 - 12 * (x-1/3) \
         + 10 * noise * (i/n_noise) * r

def f4():
    return np.sin(4*np.pi*x) + 2 * noise * (i/n_noise) * r

def f5():
    return np.sin(16*np.pi*x) + noise * (i/n_noise) * r

def f6():
    return x**(1/4) + noise * (i/n_noise) * r

def f7():
    return (2*np.random.binomial(1, 0.5, n)-1) * (np.sqrt(1-(2*x-1)**2)) \
         + (noise/4) * (i/n_noise) * r

def f8():
    return (x > 0.5) + noise * 5 * (i/n_noise) * r


ff = [f1, f2, f3, f4, f5, f6, f7, f8]
mine_approx = MINE(alpha=mine_alpha, c=mine_c, est="mic_approx")
mine_e = MINE(alpha=mine_alpha, c=mine_c, est="mic_e")

mic_approx_power = np.empty((len(ff), n_noise))
mic_e_power = np.empty((len(ff), n_noise))
tic_e_power = np.empty((len(ff), n_noise))
r2_power = np.empty((len(ff), n_noise))

np.random.seed(0)
for i in range(1, n_noise+1):
    for j, f in enumerate(ff):
        print "Noise: %d, function: %d" % (i, j)
        
        mic_approx_null, mic_e_null, tic_e_null, r2_null = [], [], [], []
        mic_approx_alt, mic_e_alt, tic_e_alt, r2_alt = [], [], [], []

        # null hypothesis
        for k in range(1, n_null+1):
            x = np.random.rand(n)
            r = np.random.randn(n)
            y = f()
            
            # resimulate x for the null scenario
            x = np.random.rand(n)

            mine_approx.compute_score(x, y)
            mine_e.compute_score(x, y)
            
            mic_approx_null.append(mine_approx.mic())
            mic_e_null.append(mine_e.mic())
            tic_e_null.append(mine_e.tic())
            r2_null.append(np.corrcoef(x, y)[0][1]**2)

        # alternative hypothesis
        for k in range(1, n_alt+1):
            x = np.random.rand(n)
            r = np.random.randn(n)
            y = f()

            mine_approx.compute_score(x, y)
            mine_e.compute_score(x, y)
            
            mic_approx_alt.append(mine_approx.mic())
            mic_e_alt.append(mine_e.mic())
            tic_e_alt.append(mine_e.tic())
            r2_alt.append(np.corrcoef(x, y)[0][1]**2)

        cut_mic_approx = np.percentile(mic_approx_null, 95)
        cut_mic_e = np.percentile(mic_e_null, 95)
        cut_tic_e = np.percentile(tic_e_null, 95)
        cut_r2 = np.percentile(r2_null, 95)
        
        mic_approx_power[j, i-1] = np.sum(np.array(mic_approx_alt) > cut_mic_approx) / n_alt
        mic_e_power[j, i-1] = np.sum(np.array(mic_e_alt) > cut_mic_e) / n_alt
        tic_e_power[j, i-1] = np.sum(np.array(tic_e_alt) > cut_tic_e) / n_alt
        r2_power[j, i-1] = np.sum(np.array(r2_alt) > cut_r2) / n_alt
        
fig = plt.figure(1, figsize=(14, 12))
x_noise = np.linspace(noise/n_noise, noise, n_noise)
for i in range(len(ff)):
    plt.subplot(4,2,i+1)
    plt.title(f_names[i])
    plt.plot(x_noise, mic_approx_power[i], label="MIC APPROX")
    plt.plot(x_noise, mic_e_power[i], label="MIC_e")
    plt.plot(x_noise, tic_e_power[i], label="TIC_e")
    plt.plot(x_noise, r2_power[i], label="cor")
    plt.xlabel("Noise Level")
    plt.ylabel("Power")
    plt.xlim((0, noise+1))
    plt.ylim((-0.05, 1.05))
    plt.legend(loc='upper right')

plt.tight_layout()
fig.savefig("power.pdf", bbox_inches='tight')
