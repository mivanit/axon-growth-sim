import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random

df = pd.read_csv("neurons.csv")
data_coord = df.values
df = pd.read_csv("net_topo.csv")
data_topo = df.values

# print(data_coord)
# print(data_topo)

# draw dots
dots_x = [x[1] for x in data_coord]
dots_y = [x[2] for x in data_coord]

plt.plot(dots_x, dots_y, 'bo')

# draw connections

for c in data_topo:
	if random.random() < 0.5:
		id_a = int(c[0])
		id_b = int(c[1])
		plt.arrow(dots_x[id_a], dots_y[id_a], dots_x[id_b] - dots_x[id_a], dots_y[id_b] - dots_y[id_a])






plt.show()