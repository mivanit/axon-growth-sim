
# help msg
if [[ ( $1 == "-h" || $1 == "--help" || $1 == "" ) ]]; then
	printf "Compile the network code with a given consts.h file \n"
	printf "usage: \n\n"

	printf "	-h | --help \n" 
	printf "		:	print this message \n\n"

	printf "	<consts_path> <gen_net> <gen_stim> <wf_tests> \n"
	printf "		:	compile with the given consts file -- '<consts_path>/consts.h' \n"
	printf "		:	if <gen_net> == 'y', generate a new network with the params from consts \n"
	printf "		:	if <gen_stim> == 'y', generate new net stim with the params from consts \n"
	printf "		:	if <wf_tests> == 'y', test waveform processing and save results (comparison to HH model) \n"
	exit 0
fi

# copy consts.h
printf "replacing consts.h with  $1/consts.h \n"
cp $1/consts.h consts.h
# recompile network code
# -Ofast does floating point approximations too
printf "compiling everything, saving executables to $1/ \n"
printf "============================================================ \n"
printf "compiling ../network/main.cpp: \n\n"
g++ -std=c++17 "../network/main.cpp" "../diffusion/diffusion.cpp" -o "$1/run_net.exe" -Ofast
printf "============================================================ \n"
printf "compiling ../network/generator.cpp: \n\n"
g++ -std=c++17 "../network/generator.cpp" -o "$1/gen_net.exe" -O0
printf "============================================================ \n"
printf "compiling ../waveform/gen_wf.cpp: \n\n"
g++ -std=c++17 "../waveform/gen_wf.cpp" -o "$1/gen_wf.exe" -O0
printf "============================================================ \n"


if [[ ( $2 == "y" ) ]]; then 
	printf "generating a new network into $1/ \n"
	./$1/gen_net.exe $1/
fi

if [[ ( $3 == "y" ) ]]; then 
	printf "generating new network initial state (stim) into $1/stim/ \n"
	rm -r $1/stim
	mkdir $1/stim
	./$1/gen_wf.exe $1/stim/
fi

if [[ ( $4 == "y" ) ]]; then 
	printf "testing wf accuracy, sending results to $1/wf_test/ \n"
	printf "TODO"
fi
