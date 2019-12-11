# Usage: ./run_and_plot <name of simulation> <MAX_TIMESTEP> <PLOT_EVERY>

# tell the script to stop if any command returns nonzero exit code
set -e

if [ "$#" -ne 3 ]; then 
    echo "Usage: ./run_and_plot <name of simulation> <MAX_TIMESTEP> <PLOT_EVERY>"
    exit 1;
fi

pushd ../data/$1/

echo "+ ./run_sim.exe $1 $2 $3"
./run_sim.exe $1 $2 $3

pushd ../..
echo "+ python vis/split_data.py $1 axon_final.tsv"
python vis/split_data.py $1 "axon_final.tsv"
echo "+ python vis/plot_axons.py $1 gif"
python vis/plot_axons.py $1 gif
popd
popd