printf "compiling\n"
g++ -std=c++17 "../../network/main.cpp" "../../diffusion/diffusion.cpp" -o "run_net.exe" -O0

printf "running\n"

./run_net.exe neurons.csv net_topo.csv stim/