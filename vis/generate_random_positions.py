"""
Generate axon positions to use when plotting. 

The initial position is random; the direction is picked randomly every 3 
timesteps and the axon moves in that direction each timestep. The axon type
is selected randomly from AXON_TYPES.

INPUT:  
    - MAX_POS: x, y are bounded by [0, MAX_POS]
    - TIMESTEPS: number of steps to simulate for
    - NUM_AXONS: the number of axons to generate data for
    - OUTPUT_DIR: where to put data
OUTPUT:
    - OUTPUTDIR/axon_<axon_id>_<axon_type>.csv for each generated axon

"""

import random
import numpy as np
import os
import csv

MAX_POS = 100
TIMESTEPS = 500
NUM_AXONS = 5
OUTPUT_DIR = 'processed'

AXON_TYPES = [0, 1, 2]

PATH = __file__.split('/')[0]

# directions in range [-2, 2] with +/- 1 being most likely
dir_vec = [i for i in range(-2, 3)]
dir_probs = [0.1, 0.3, 0.2, 0.3, 0.1]

for axon in range(NUM_AXONS):
    # pick a random type for the axon
    axon_type = random.choice(AXON_TYPES)

    # choose random initial position
    x_vec = [random.randint(0, MAX_POS)]
    y_vec = [random.randint(0, MAX_POS)]

    for i in range(TIMESTEPS - 1):
        # update direction every 3 timesteps
        if i % 3 == 0:
            dx = np.random.choice(dir_vec, p=dir_probs)
            dy = np.random.choice(dir_vec, p=dir_probs)

        # update position, but must stay in range [0, MAX_POS]
        x_vec.append(max(min(MAX_POS, x_vec[-1] + dx), 0))
        y_vec.append(max(min(MAX_POS, y_vec[-1] + dy), 0))

    # pad id with zeros to make it in ### format
    axon_id = f'{axon:03}'
    new_filename = f'axon_{axon_id}_{axon_type}.csv'

    print(f'Generating {new_filename}')
    # save data to file
    with open(os.path.join(PATH, OUTPUT_DIR, new_filename), mode='w') as output_file:
        writer = csv.writer(output_file, delimiter=',')
        writer.writerows([x_vec, y_vec])
