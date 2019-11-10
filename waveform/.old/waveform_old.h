// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef WAVEFORM
#define WAVEFORM

#include <vector>
#include <algorithm>

#include "consts.h"

// * class def

// wave component
class wave_component
{
public:
	
float phase;	// phase, spiking start 
float period;		// period, for frequency of component
float amp;		// amplitude coefficient

// uint16_t num_steps;
size_t num_steps;

// usual ctor
wave_component(float in_phase, float in_per, float in_amp, size_t in_k)
	: phase(in_phase), period(in_per), amp(in_amp), num_steps(in_k) {}

// empty ctor for first element, only pass the phase
wave_component(float in_phase)
	: phase(in_phase), period(0.0), amp(0.0), num_steps(0) {}

};

// waveform
typedef std::vector < wave_component > waveform;
// NOTE: wf[0] is special, contains valid data only in "phase" -- this determines offset for *ALL* wave components, helps simplify computations and improve floating point accuracy


// * waveform operations

void wf_activation(waveform & wf)
{
	wf_compress(wf);
	// TODO: lots of work to be done here

	


}

// get total phase shift of waveform
float wf_get_totalShift(waveform & input)
{
	return input[0].phase;
}

// computes new total shift by getting minimum shift of all elements, subtracting that from all elements, and adding to totalShift var
float wf_comp_totalShift(waveform & input)
{
	// exit if no elements
	if (input.size() < 2) return 0.0;
	
	// find min shift
	float min_shift = input[1].phase;
	for (wave_component x : input)
	{
		if (x.phase < min_shift) 
		{
			min_shift = x.phase;
		}
	}

	// update
	input[0].phase += min_shift;
	for (wave_component x : input)
	{
		x.phase -= min_shift;
	}
}

// mov some value of total shift from var to components
void wf_mov_totalShift(waveform & input, float modValue)
{
	input[0].phase -= modValue;
	for (wave_component x : input)
	{
		x.phase += modValue;
	}
}

// add contents of one waveform to another
void wf_concat(waveform & main, waveform & add)
{
	// check which has smaller totalShift, update other to match
	// WARNING: this can modify either of the passed waveforms
	// 			data is unchanged, but representation can change. This can be expensive
	// if ()

	// TODO: reserve space
	main.insert( main.end(), add.begin(), add.end() );
}

bool wave_component_compare(wave_component & lhs, wave_component & rhs)
{
	// want to return true if lhs should be ordered before rhs
	return (lhs.period < rhs.period);
}


// waveform compression function
void wf_compress(waveform & wf_c)
{
	// TODO: this whole mess

	// sort by wavelength first
	std::sort(wf_c.begin(), wf_c.end(), wave_component_compare);

	// same freq, same phase (to within some epsilon) ==> remove later one


	// phase = per/2 (to within some epsilon), same freq ==> remove one, halve per

		// version for more than one wave combination


	// wavelength too small ==> increase


	// same freq, similar phase (to within refractory period, should define in consts.h) 
		// ==> remove later spike


	// any freq, same phase ==> some sort of analysis?

	// TODO: other ways to compress?

}


#endif