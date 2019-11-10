#%%

import os
os.chdir(os.path.join(os.getcwd(), 'C:\\Users\\mivan\\Google Drive\\Winter_2019\\Math_563\\finalProj\\alt_nn\\waveform\\py_code\\'))

import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from HH_main import *
from parse_wf import *

# returns array with spike times for the given file
def get_spike_times(file_in = '../data/test1_data.csv', file_out_D = '../data/test1_data.csv'):
	# consts
	srch_step = 1.0
	spike_thresh = 50.0

	# read, compute
	wf_in = read_wf(file_in)
	wf_out = read_wf(file_out_D)
	print("computing wfa_in")
	stim_in = wf_to_stim(wf_in)
	wfa_in = [stim_in(t) for t in T]
	print("computing wfa_out")
	stim_out = wf_act_to_stim(wf_out)
	wfa_out = [stim_out(t) for t in T]
	print("computing wfa_HH")
	wfa_HH, trash = compute(wf_to_stim(wf_in), bln_plot=False)
	wfa_HH = wfa_HH[:,0]

	print("plotting all")

	# plot next to each other
	plt.plot(T, wfa_in, 'r')
	plt.plot(T, wfa_out, 'g', linewidth=4.0, alpha=0.6)
	plt.plot(T, wfa_HH, 'b')


	# now, calculate the spike times and save in an array
	# we assume that the spike "occurs" at the peak voltage
	# REVIEW: this doesn't match with how the c++ code treats spikes, needs to be fixed
	# spikes = []
	# t = tmin
	# while t < tmax - srch_step:
	# 	# check range of next srch_step seconds
	# 	test_t, test_A = find_range_max(out_wf, r_t=(t, t + srch_step), adj_dt = dt * 50.0 )
	# 	# once the max over that range is found, do a more refined search for the peak
	# 	test_t, test_A = find_range_max(out_wf, r_t=(test_t - t_e/2, test_t + t_e/2), adj_dt = dt * 5.0)
		
	# 	if test_A > spike_thresh:
	# 		spikes.append(test_t)	

	# 	t = t + srch_step

	# for sp_val in spikes:
		# plt.axvline(x=sp_val, color='k')

	# np.savetxt("../data/test1_HH.csv", spikes, delimiter=",")

	plt.show()

get_spike_times("../data/wf/wf_0.csv", "../data/wf/wf_0_out.csv")



def compare_spikes(file_HH = '../data/test1_HH.csv', file_freq = '../data/test1_freq.csv'):
	file_read = pd.read_csv(file_HH, header=None)
	data_HH = file_read.values.flatten()

	# file_read = pd.read_csv(file_freq, header=None)
	# data_freq = file_read.values.flatten()

	print(data_HH)
	# print(data_freq)

# compare_spikes()


#%%
























