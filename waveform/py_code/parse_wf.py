import numpy as np
import pandas as pd

from def_spike import *
from HH_main import *

class wf_comp(object):
	def __init__(self, use_arr = False, arr=[], period=0.0, phase=0.0, amp=0.0, t_strt=0, t_end=0):
		if use_arr:
			self.phase = arr[0]
			self.period = arr[1]
			self.amp = arr[2]
			self.t_strt = int(round(arr[3]))
			self.t_end = int(round(arr[4]))
		else:
			self.phase = period
			self.period = phase
			self.amp = amp
			self.t_strt = t_strt
			self.t_end = t_end

	def __str__(self):
		return "\t %d \t %d \t %d \t %d \t %d" % (self.period, self.phase, self.amp, self.t_strt, self.t_end )

def read_wf(filename = '../data/test1_data.csv'):
	file_csv = pd.read_csv(filename)
	data = file_csv.values

	# print(data)
	# print(data[0])
	# print(len(data))

	wf = [wf_comp(use_arr=True, arr=row) for row in data ]
	
	print("read file " + filename)
	for x in wf:
		print(x)

	return wf


# given a waveform, generate an stimulation function
def wf_to_stim(wf):
	spikes = []
	
	# for every waveform component, generate spikes
	i = 0
	for x in wf:
		i = 0
		while (x.period * i + x.phase) < tmax:
			spikes.append( ((x.period * i) + x.phase, x.amp) )
			i = i+1

	# return input stim function
	return get_stim_fctn(spikes)



# given a waveform, generate an activation array
def wf_act_to_stim(wf):
	# print("=====================")
	# print("wf_act_to_stim()")
	spikes = []
	
	# for every waveform component, generate spikes
	i = 0
	for x in wf:
		i = 0
		while (x.period * i + x.phase) < tmax:
			spikes.append( ((x.period * i) + x.phase, x.amp) )
			i = i+1

	
	TIMESTEP_WF = 10.0

	# sort by spike timing
	spikes.sort(key = lambda spk : spk[0])
	# print(spikes)

	spikes_act = []

	# find the first spike (if any) in every interval
	i = 0
	t = tmin
	found_spike = False
	while t < tmax:
		# iterate until first spike after t
		while (i < len(spikes) - 1) and (spikes[i][0] < t):
			i = i+1
			found_spike = False

		# append this spike, continue
		if not found_spike:
			print(str(t) + " : " + str(spikes[i][0]))
			spikes_act.append(spikes[i])
			found_spike = True

		t = t + TIMESTEP_WF
	
	# return input stim function
	return get_stim_fctn(spikes_act)

# my_wf = read_wf('../data/test1_data.csv')
# vltg = compute(wf_to_stim(my_wf))[:,0]




































# ~ vltg = compute(wf_to_stim(my_wf))[:,0]
# ~ def new_stim(t):
# ~ 	return vltg[int((t-1)/dt)]
# ~ compute(new_stim)




