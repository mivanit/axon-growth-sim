# CRIT: implement setup script

# Usage: config file path (relative to 568_Project), name of simulation

if [ $# -ne 2 ]; then
	echo "USAGE: build CONFIG_PATH NAME"
	exit 1
fi

CONFIG_PATH=$1
CONFIG_NAME=$(echo $CONFIG_PATH | rev | cut -d '/' -f 1 | rev)
NAME=$2


# create output data folders:
# `data/<name>/raw/`  :  raw per-timestep .csv files
# `data/<name>/cfg/`  :  config files
# `data/<name>/vis/`  :  output of visualization

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
PDIR=$(echo $SCRIPTPATH | rev | cut -d '/' -f 2- | rev)

mkdir $PDIR/data/$NAME
mkdir $PDIR/data/$NAME/raw
mkdir $PDIR/data/$NAME/cfg
mkdir $PDIR/data/$NAME/vis

# copy all configuration to output folder `data/<name>/cfg/`  
cp $PDIR/$CONFIG_PATH $PDIR/data/$NAME/cfg/$CONFIG_NAME

# compile executable
#g++ -std=c++17 main.cpp $PDIR/diffusion/diffusion.cpp config_default.c -o run_sim.exe
