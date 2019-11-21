# CRIT: implement setup script

# get config file path, <name> (of simulation) from args

# create output data folders:
# `data/<name>/raw/`  :  raw per-timestep .csv files
# `data/<name>/cfg/`  :  config files
# `data/<name>/vis/`  :  output of visualization

# copy all configuration to output folder `data/<name>/cfg/`  


# compile executable
g++ -std=c++17 main.cpp ../diffusion/diffusion.cpp config_default.c -o run_sim.exe

