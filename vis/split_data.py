"""
Script to split data from axon output file and create
separate CSV files for each axon that can be read into MATLAB or python
"""

import os
import csv
import sys
from glob import glob
from ast import literal_eval

if len(sys.argv) != 3:
    print('Usage: split_data.py <TEST_NAME> <AXON_FILENAME>')
    sys.exit(1)

TEST_NAME = sys.argv[1]
AXON_FILENAME = sys.argv[2]

INPUT_DIR = os.path.join('data', TEST_NAME, 'raw')
OUTPUT_DIR = os.path.join('data', TEST_NAME, 'processed')

# check if input directory exists
if not os.path.isdir(INPUT_DIR):
    print(f'Error: {INPUT_DIR} is not a directory')
    sys.exit(1)

# create output directory if it doesn't exist
if not os.path.isdir(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

INPUT_FILE = os.path.join(INPUT_DIR, AXON_FILENAME)
# get indices of where each axon info begins
with open(INPUT_FILE) as axon_file:
    lines = axon_file.readlines()
    split_indices = [i for i, line in enumerate(lines) if line[0] == '=']

# split the file and get information for each axon
split_indices.append(-1)
axon_list = [lines[split_indices[i]+1:split_indices[i+1]] for i in range(len(split_indices) - 1)]

# save previous coordinate arrays as separate CSVs per axon
try:
    for axon in axon_list:
        metadata = tuple(literal_eval(','.join(axon[0].strip().split('\t'))))
        #print(metadata)
        axon_id, axon_type, axon_pos, axon_dir = metadata

        
        # read in each coordinate in previous position array as tuple
        prev_coordinates = list(axon[1].strip().split('\t'))
        prev_x = []
        prev_y = []
        for i, coordinate in enumerate(prev_coordinates):
            coordinate = literal_eval(coordinate)
            if i == 100:
                # sanity check
                #print(coordinate)
                pass
            prev_x.append(coordinate[0])
            prev_y.append(coordinate[1])

        # pad id with zeros to make it in ### format
        axon_id = f'{axon_id:03}'
        new_filename = f'axon_{axon_id}_{axon_type}.csv'

        # save data to file
        with open(os.path.join(OUTPUT_DIR, new_filename), mode='w') as output_file:
            writer = csv.writer(output_file, delimiter=',')
            writer.writerows([prev_x, prev_y])

except IndexError as error:
    print(error)
