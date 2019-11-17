#%%
import os
# os.chdir(os.path.join(os.getcwd(), 'C:\\Users\\mivan\\Google Drive\\Winter_2019\\Math_563\\finalProj\\alt_nn\\waveform\\py_code\\'))

print("starting")

import numpy as np
import numpy.random as rnd
import matplotlib.pyplot as plt
from scipy.fftpack import fft
# import random
from HH_main import *
from def_spike import *


# # generate randomized waveform
dist_max = 10.0
dist_center = 3.0
dist_width = 10.0
# # SHOULD DIVIDE len(T) 
# num_chunks = 100
# num_repeats = int(len(T) / num_chunks)

# UNCOMMENT ONE:
# ~~~~~~~~~~~~
# rand_wf = dist_max * np.random.random(len(T))
# rand_wf = np.repeat(dist_max * np.random.random(num_repeats),  num_chunks)
# rand_wf = abs(np.random.normal(dist_center,dist_width, len(T)))
# ~~~~~~~~~~~~

# def wf_func(t):
# 	idx = int((t - tmin) / dt - 1)
# 	if idx >= len(rand_wf):
# 		print("tried to access index  %d" % (idx))
# 		return rand_wf[-1]
# 	else:
# 		return rand_wf[idx]

# compute(wf_func, bln_plot = True)

num_spikes = 400
def get_rand_amp():
	# return dist_max * rnd.random()
	return rnd.normal(dist_center, dist_width)

rand_spikes = [ (rnd.random() * tmax, get_rand_amp()) for x in range(num_spikes)]
# rand_spikes = [(3 * x, 15.0) for x in range(num_spikes)]

# plot waveforms
wf_func_rand = get_stim_fctn(rand_spikes)
wf_ap = compute(wf_func_rand, bln_plot=True)[:, 0]

# get array of rand waveform
wf_rand = [wf_func_rand(t) for t in T]

# wf_test = 20 * np.sin(100 * np.pi * T) + 15 * np.sin(15 * np.pi * T)
# wf_test = 20 * np.sin(50 * np.pi * T) + 10

# plt.plot(T, wf_test, 'g')
plt.show()

# plot fft of both
fft_rand = fft(wf_rand)
fft_ap = fft(wf_ap)
# fft_test = fft(wf_test)




N = len(T)

# xf = np.linspace(0.0, 1.0/(2.0*T), N//2)
xf = np.linspace(0.0, tmax/2, N//2)

plt.plot(xf, 2.0 / N * np.abs( fft_rand[0:N//2]))
plt.show()


plt.plot(xf, 2.0 / N * np.abs( fft_ap[0:N//2]))
plt.show()
# plt.plot(xf, 2.0 / N * np.abs( fft_test[0:N//2]))
# plt.show()

#%%









