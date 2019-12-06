% Get axon and neuron data from simulation and plot the location
% of the neurons and trajectory of the axons

% directory containing simulation data
DIRPATH = '../data/processed/';
DIR = dir(strcat(DIRPATH, 'axon_*.csv'));
files = {DIR.name};

hold on
% load neuron and axon files from directory
for i=1:length(files)
    filename = char(strcat(DIRPATH, files(i)));
    file = csvread(filename);
    plot(file(1,:), file(2,:), 'o-');
end