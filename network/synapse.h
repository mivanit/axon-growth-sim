// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef SYNAPSE
#define SYNAPSE

#include "../waveform/waveform_D.h"

class synapse
{
public:

uint16_t target;		// index of target neuron
float alpha;		// modifying coefficient of amplitude
float delay;		// time delay of whole connection

// target-only ctor does a constant delay, const alpha
synapse(size_t in_tgt)
{
	target = in_tgt;

	delay = 10.0;
	alpha = 0.1;
}

// real ctor
synapse(size_t in_tgt, float in_dly, float in_ampCoef)
	: target(in_tgt), delay(in_dly), alpha(in_ampCoef) {}

};

#endif
