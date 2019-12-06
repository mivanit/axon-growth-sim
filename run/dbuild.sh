# Default build script for testing
NAME='DEFAULT_TEST'
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
PDIR=$(echo $SCRIPTPATH | rev | cut -d '/' -f 2- | rev)

[ -f $PDIR/run/run_sim.exe ] && rm $PDIR/run/run_sim.exe
rm -rf $PDIR/data/$NAME

mkdir $PDIR/data/$NAME
mkdir $PDIR/data/$NAME/raw
mkdir $PDIR/data/$NAME/cfg
mkdir $PDIR/data/$NAME/vis
cp $PDIR/run/config_default.c $PDIR/data/$NAME/cfg/config_default.c

g++ -std=c++17 main.cpp $PDIR/diffusion/diffusion.cpp $CONFIG_NAME -o run_sim.exe
