#include <string>

#include "gen_wf.h"
#include "../run/consts.h"
#include "../util/err_util.h"

int main(int argc, char ** argv)
{
	if (argc <= 1 || argv[1] == "-h" || argv[1] == "--help" )
	{
		std::cout << "gen_wf.cpp \n"
			 << "generates frequency domain waveforms and exports to csv format"
			 << "usage: \n"
			 << " ./gen_wf.exe <path_save>    :    saves all .csv files into the given folder";
		return 0;
	}

	for (uint16_t n = 0; n < (uint16_t) ( F_STIM * (float) N_NEURONS ); n++)
	{
		// get waveform
		waveform wf = gen_rand_wf();
		// save
		// NOTE: neuron locations are random, so there is no problem with simply stimulating the first n neurons (no spatial bias)
		std::string dir = argv[1];
		wf_save(wf, dir + "/" + CSTR_STIMNAME + std::to_string(n) + CSTR_STIMEXT);
	}
}