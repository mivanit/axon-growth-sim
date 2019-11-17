
#define CSV_IO_NO_THREAD

#include <string>
#include <iostream>
#include <fstream>

#include "../waveform_D.h"
#include "../../util/csv_parser.h"
#include "../../util/err_util.h"

int main(int argc, char ** argv)
{
	__DEBUG("starting");
	// * read in waveform
	waveform wf_in = wf_read("../data/wf/wf_0.csv");
	__DEBUG("waveform read in");

	// * process
	waveform wf_out = wf_activation(wf_in);
	__DEBUG("waveform processed");

	// * output
	// processing into spike train etc done in spike_times.py
	wf_save(wf_out, "../data/wf/wf_0_out.csv");
	__DEBUG("waveform written");

	return 0;
}