# Usage: ./plot.sh <name of simulation>

# tell the script to stop if any command returns nonzero exit code
set -e

if [ "$#" -ne 1 ]; then 
    echo "Usage: ./plot.sh <name of simulation>"
    exit 1;
fi

pushd ../data/$1/
pushd ../..
echo "+ python vis/split_data.py $1 axon_final.tsv"
python vis/split_data.py $1 "axon_final.tsv"
echo "+ python vis/plot_axons.py $1 gif"
python vis/plot_axons.py $1 gif
popd
popd