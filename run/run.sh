# Usage: ./run.sh <name of simulation> <MAX_TIMESTEP> <PLOT_EVERY>

# tell the script to stop if any command returns nonzero exit code
set -e

if [ "$#" -ne 3 ]; then 
    echo "Usage: ./run.sh <name of simulation> <MAX_TIMESTEP> <PLOT_EVERY>"
    exit 1;
fi

pushd ../data/$1/

echo "+ ./run_sim.exe $1 $2 $3"
./run_sim.exe $1 $2 $3

popd
