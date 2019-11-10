#%%
import os
os.chdir(os.path.join(os.getcwd(), 'C:\\Users\\mivan\\Google Drive\\Winter_2019\\Math_563\\finalProj\\alt_nn\\waveform\\py_code\\'))

print("starting")

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import collections  as mc
from HH_main import *
from def_spike import *


# generate spikes
spikes_A = []
spikes_B = []

P_A, phi_A = (50, 10)
P_B, phi_B = (20, 20)

for n in range(20):
	spikes_A.append( ( P_A * n + phi_A, 3.0 ) )
	spikes_B.append( ( P_B * n + phi_B, 4.0 ) )

spikes = spikes_A + spikes_B

# get waveforms
spikes_func = get_stim_fctn(spikes)
wf_ap, wf_stim = compute(spikes_func, bln_plot=False)
wf_ap = wf_ap[:,0]


# plot waveform
# plt.plot(T, wf_stim, 'r')
plt.figure(num=None, figsize=(7.5, 4.5), dpi=100, facecolor='w', edgecolor='k')

plt.plot(T, [get_stim_fctn(spikes_B)(t) for t in T], 'r', linewidth=3.0)
plt.plot(T, [get_stim_fctn(spikes_A)(t) for t in T], color='orange', linewidth=3.0)

plt.plot(T, wf_ap, 'b', linewidth=0.7)
plt.ylim(-10,25)



# def labels
# plt.rc('text', usetex=True)
labels = [
	[ [0, phi_A], 4 * np.ones(2), r'$\phi_A$' ],
	[ [0, phi_B], 7 * np.ones(2), r'$\phi_B$' ],
	[ [0, phi_A + P_A], 10 * np.ones(2), r'$\phi_X$' ],
	[ [phi_A, phi_A + P_A], -6 * np.ones(2), r'$P_A$' ],
	[ [phi_B, phi_B + P_B], -3.5 * np.ones(2), r'$P_B$'  ],
	[ [phi_A + P_A, phi_A + P_A + 100], 14 * np.ones(2), r'$P_X$' ]
]

lines = [
	[ [0, 0], [0,10] ],
	[ [phi_A, phi_A], [4,-6] ],
	[ [phi_B, phi_B], [7,-3.5] ],
	[ [phi_A + P_A, phi_A + P_A], [14,-6] ],
	[ [phi_B + P_B, phi_B + P_B], [4,-3.5] ],
	[ [160, 160], [14,0] ],
]

# plot labels
for L in labels:
	plt.plot(L[0], L[1],'k|-', alpha=0.6)
	plt.text(sum(L[0])/2, L[1][0], L[2], horizontalalignment='center', verticalalignment='bottom')

for L in lines:
	plt.plot(L[0], L[1], 'k--', alpha=0.4)


plt.xlabel('time (ms)')
plt.ylabel('Voltage (mV)')

plt.savefig('../data/img/comp_inter.png', dpi=500, bbox_inches='tight')

plt.show()

#%%