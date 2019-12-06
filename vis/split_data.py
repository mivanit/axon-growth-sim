"""
Script to split data from axon output file and create
separate CSV files for each axon that can be read into MATLAB or python
"""

import os
import csv
from glob import glob
from ast import literal_eval
from matplotlib import pyplot as plt

INPUT_DIR = os.path.join('..', 'data', 'raw')
OUTPUT_DIR = os.path.join('..', 'data', 'processed')

# create output directory if it doesn't exist
if not os.path.isdir(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

# get indices of where each axon info begins
for axon_filename in glob(os.path.join(INPUT_DIR, 'axon_*.tsv')):
    with open(axon_filename) as axon_file:
        lines = axon_file.readlines()
        split_indices = [i for i, line in enumerate(lines) if line[0] == '=']

# split the file and get information for each axon
split_indices.append(-1)
axon_list = [lines[split_indices[i]+1:split_indices[i+1]] for i in range(len(split_indices) - 1)]

# save previous coordinate arrays as separate CSVs per axon
try:
    for axon in axon_list:
        metadata = tuple(literal_eval(axon[0].strip()))
        print(metadata)
        axon_id, axon_type, axon_pos, axon_dir = metadata
        
        # read in each coordinate in previous position array as tuple
        prev_coordinates = list(literal_eval(axon[1].strip()))
        prev_x = [coordinate[0] for coordinate in prev_coordinates]
        prev_y = [coordinate[1] for coordinate in prev_coordinates]

        # pad id with zeros to make it in ### format
        axon_id = f'{axon_id:03}'
        new_filename = f'axon_{axon_id}_{axon_type}.csv'

        # save data to file
        with open(os.path.join(OUTPUT_DIR, new_filename), mode='w') as output_file:
            writer = csv.writer(output_file, delimiter=',')
            writer.writerows([prev_x, prev_y])

except IndexError as error:
    print(error)
