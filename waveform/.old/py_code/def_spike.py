import numpy as np


# Start and end time (in milliseconds)
tmin = 0.0
tmax = 1000.0
dt = 0.1

# * spikes

# spike consts
A_default = 110.0
t_r = 0.6
t_e = 3

def wf_spike(t, A=A_default):
	if 0.0 < t < t_r:
		return A * t / t_r
	elif t_r < t < t_e:
		return A - (t-t_r) * A / (t_e - t_r)
	return 0.0


spikeAmp_type = [('time', float), ('amp', float)]


# def Id(t):
# 	A = 110.0
# 	t_r = 1.2
# 	t_f = t_r + 0.15
# 	t_e = 3.6

# 	if 0.0 < t < t_r:
# 		return A * ((t / t_r)**4.0)
# 	elif t_r < t < t_f:
# 		return A
# 	elif t_f < t < t_e:
# 		return A - (t-t_f) * A / (t_e - t_f)
# 	return 0.0



# * misc util
# assuming sorted numpy array, find APPROX index of largest item < elt
# uses binary search
def find_elt_SORTED(arr, elt, lower_bd=True):
	# print("searching for " + str(elt) )
	bd_L = 0
	bd_U = len(arr)
	i = int((bd_U + bd_L)/2)
	
	while True:
		i = int((bd_U + bd_L)/2)
		# print('\t' + str(i) + '\t' + str(bd_L) + '\t' + str(bd_U))
		if arr[i][0] < elt:
			bd_L = i
		else:
			bd_U = i
		
		if (bd_U - bd_L) < 2:
			if lower_bd:
				return bd_L
			else:
				return bd_U


# sort spikes
def get_stim_fctn(spikes, ampGiven = True):
	spikes = np.array(spikes, dtype = spikeAmp_type)
	spikes.sort(order='time')
	# print(spikes)

	# define input stimulus function
	def stim(t):
		val = 0
		# get min and max bound spike indecies
		bd_L = find_elt_SORTED(spikes, t - 2.0 * t_e)
		bd_U = find_elt_SORTED(spikes, t + t_e, lower_bd = False)
		for x_q in spikes[ bd_L:bd_U ]:
			if ampGiven:
				x = x_q[0]
				A = x_q[1]
			# TODO: an "else" here should allow passing of just spikes with implicit default amplitude

			if t > x:
				if t < x + t_e:
					val = val + wf_spike(t-x, A)
		return val

	return stim








def find_range_max(arr_V, r_t = (3.0,10.0), adj_dt = dt * 10):
	t = r_t[0]

	max_V = 0.0
	max_t = 0.0
	
	while t < r_t[1]:
		temp = arr_V[int(round(t/dt))]
		if max_V < temp:
			max_V = temp
			max_t = t

		t = t + adj_dt

	return (max_t, max_V)







# ~ test_arr = np.linspace(0.0, 10000.0, 10000)


# ~ print(find_elt_SORTED(test_arr, 100, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 50, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 20, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 10, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 70, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 90, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 0, lower_bd=False))
# ~ print(find_elt_SORTED(test_arr, 100, lower_bd=False))













