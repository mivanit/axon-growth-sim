# Usage: config file path (relative to 568_Project), name of simulation

# tell the script to stop if any command returns nonzero exit code
set -e


if [ $# < 2 ]; then
	echo "USAGE: ./build_alt.sh <CONFIG_PATH> <SIM_NAME> [OPTIONS]"
	echo "options:"
	echo " -g  :  build for gdb debugger"
	echo "to be launched from 'run/' directory"
	exit 1
fi


DATA_DIR="../data/"
EXEC_NAME="run_sim.exe"
DIFFUSION_CODE="../diffusion/diffusion.cpp"


CONFIG_PATH=$1
CONFIG_NAME="$(basename $CONFIG_PATH)"
SIM_NAME=$2
SIM_PATH=$DATA_DIR$SIM_NAME/


echo "========================="
echo "	config path:				$CONFIG_PATH"
echo "	extracted config name:      $CONFIG_NAME"
echo "	simulation name:			$SIM_NAME"
echo "	simulation path:			$SIM_PATH"
echo "========================="


# TODO: improve option selection
OPTIONS="-std=c++17"
if [ "$3" == "-g" ]; then
	echo "!!!	building GDB version"
	OPTIONS+=" -g"
fi

# clear old data in directory
echo "	clearing old data"
rm -rf $SIM_PATH


# create output data folders:
# `data/<name>/raw/`  :  raw per-timestep .csv files
# `data/<name>/cfg/`  :  config files
# `data/<name>/vis/`  :  output of visualization

echo "	creating output folders"
mkdir $SIM_PATH
mkdir $SIM_PATH/raw
mkdir $SIM_PATH/cfg
mkdir $SIM_PATH/vis


# copy all configuration to output folder `data/<name>/cfg/`  
echo "	copying config"
cp $CONFIG_PATH $SIM_PATH/cfg/$CONFIG_NAME

# compile executable
echo "========================="
echo "compiling executable:"
echo " "
g++ $OPTIONS main.cpp $DIFFUSION_CODE $CONFIG_PATH -o $SIM_PATH$EXEC_NAME
echo "========================="
echo "executable compiled successfuly and saved to $SIM_PATH$EXEC_NAME"

