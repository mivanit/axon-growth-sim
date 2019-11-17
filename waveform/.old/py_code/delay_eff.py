
#%%
from scipy.optimize import curve_fit
import sympy as sym

from py_code.HH_main import *

# * find relationship btwn distance and spike delay/amp

def find_delay_effect():
	T_delay = np.linspace(5.0, 5.3, 10)
	max_ts = []
	max_Vs = []

	for test_t in T_delay:
		pair = find_range_max(compute(None, bln_plot=False, use_spikes = True, spikes=[(0.0, A_default), (test_t, A_default)])[:,0], r_t=(max(3.5, test_t), test_t + 6) )

		max_ts.append(pair[0] - test_t)
		max_Vs.append(pair[1])

	# max_Vs = np.array(max_Vs) / 30.0

	fig, ax1 = plt.subplots(figsize=(12, 7))
	plt.grid()
	ax1.plot(T_delay, max_ts, 'r-')
	
	ax2 = ax1.twinx()
	ax2.plot(T_delay, max_Vs, 'b-')
	
	ax1.set_xlabel('second spike delay (ms)')
	ax1.set_ylabel('2nd spike initiation to peak delay', color='r')
	ax2.set_ylabel('peak voltage of 2nd spike', color='b')
	# ax.set_ylabel('blue ')

	plt.show()

# find_delay_effect()

#%%





# * find minimum amplitude of second stim to get a spike

def find_min_Vreq(thresh = 2.5, spike_thresh = 50.0):
	T_delay = np.linspace(4.5, 15.0, 20)
	min_V = []

	# for each time delay, find to within "thresh" 
	# the minimum input spike voltage required for a spike to happen
	for test_t in T_delay:
		print("\t" + str(test_t))
		Abd_L = 0.0
		Abd_U = 250.0
		bln_loop = True
		A_test = (Abd_U + Abd_L) / 2

		while bln_loop:
			A_test = (Abd_U + Abd_L) / 2

			test_spike = [(0.0, A_default), (test_t, A_test)]
			
			pair = find_range_max(compute(None, bln_plot=False, use_spikes = True, spikes=test_spike)[:,0], r_t=(max(3.5, test_t), test_t + 6), adj_dt = dt * 250.0 )

			if pair[1] < spike_thresh:
				Abd_L = A_test
			else:
				Abd_U = A_test
			
			if (Abd_U - Abd_L) < thresh:
				bln_loop = False
		
		min_V.append(A_test)

	# max_Vs = np.array(max_Vs) / 30.0
	min_V = [ (x - 5.5) for x in min_V ]

	fig, ax1 = plt.subplots(figsize=(12, 7))
	plt.grid()
	ax1.plot(T_delay, min_V, 'r-')
		
	ax1.set_xlabel('second spike delay (ms)')
	ax1.set_ylabel('second spike additional min stimulation voltage', color='r')

	def func(t, a, b, c):
		return a * np.exp(b * t + c)

	popt, pcov = curve_fit(func, T_delay, min_V)

	# xs = sym.Symbol('x')
	# tex = sym.latex(func(xs,*popt)).replace('$', '')
	# plt.title(r'$f(x)= %s$' %(tex),fontsize=16)
	plt.plot(T_delay, func(T_delay, *popt))
	plt.title("Fitted Curve a * exp(b * t + c) with a = %d, b = %d, c = %d" % (popt[0], popt[1], popt[2]))

	plt.show()
	
find_min_Vreq()

#%%
