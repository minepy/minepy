from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
from minepy import MINE


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
mine = MINE(alpha=mine_alpha, c=mine_c)
mic_power = np.empty((len(ff), n_noise))
gmic_power = np.empty((len(ff), n_noise))
r2_power = np.empty((len(ff), n_noise))
np.random.seed(0)
for i in range(1, n_noise+1):
    for j, f in enumerate(ff):
        mic_null, gmic_null, r2_null = [], [], []
        mic_alt, gmic_alt, r2_alt = [], [], []

        # null hypothesis
        for k in range(1, n_null+1):
            print i, j, k
            x = np.random.rand(n)
            r = np.random.randn(n)
            y = f()
            
            # resimulate x for the null scenario
            x = np.random.rand(n)

            mine.compute_score(x, y)
            mic_null.append(mine.mic())
            gmic_null.append(mine.gmic(p=-1))
            r2_null.append(np.corrcoef(x, y)[0][1]**2)

        # alternative hypothesis
        for k in range(1, n_alt+1):
            x = np.random.rand(n)
            r = np.random.randn(n)
            y = f()

            mine.compute_score(x, y)
            mic_alt.append(mine.mic())
            gmic_alt.append(mine.gmic(p=-1))
            r2_alt.append(np.corrcoef(x, y)[0][1]**2)

        cut_mic = np.percentile(mic_null, 95)
        cut_gmic = np.percentile(gmic_null, 95)
        cut_r2 = np.percentile(r2_null, 95)
        
        mic_power[j, i-1] = np.sum(np.array(mic_alt) > cut_mic) / n_alt
        gmic_power[j, i-1] = np.sum(np.array(gmic_alt) > cut_gmic) / n_alt
        r2_power[j, i-1] = np.sum(np.array(r2_alt) > cut_r2) / n_alt
        
fig = plt.figure(1, figsize=(14, 12))
x_noise = np.linspace(noise/n_noise, noise, n_noise)
for i in range(len(ff)):
    plt.subplot(4,2,i+1)
    plt.title(f_names[i])
    plt.plot(x_noise, mic_power[i], label="MIC")
    plt.plot(x_noise, gmic_power[i], label="GMIC")
    plt.plot(x_noise, r2_power[i], label="cor")
    plt.xlabel("Noise Level")
    plt.ylabel("Power")
    plt.xlim((0, noise))
    plt.ylim((0, 1))
    plt.legend(loc='upper right')

plt.tight_layout()
fig.savefig("power.pdf", bbox_inches='tight')
