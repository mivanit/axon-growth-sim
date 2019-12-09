# Usage: ./run_and_plot <name of simulation>

# tell the script to stop if any command returns nonzero exit code
set -e

echo 'Running simulation...' 
pushd ../data/$1/
./run_sim.exe

echo 'Generating plots'
pushd ../..
python vis/split_data.py $1 axon_00020.tsv
python vis/plot_axons.py $1
popd
popd