// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// * DISCRETE WAVEFORM IMPLEMENTATION

#ifndef WAVEFORM
#define WAVEFORM

#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>

#include "../util/coord.h"
#include "../util/csv_parser.h"
#include "../util/math_util.h"
#include "../util/err_util.h"

// * class def

// wave component
class wave_component
{
public:
	
int16_t phase; // phase
uint16_t period; // period, for frequency of component
float amp; // amplitude coefficient

// on which diffusion timestep to start/end spiking
size_t t_strt; size_t t_end;
float f_phase; // precise phase


// usual ctor
wave_component(uint16_t in_phase, uint16_t in_per, float in_amp, size_t in_tS_s, size_t in_tS_e, float in_fPhase)
	: phase(in_phase), period(in_per), amp(in_amp), t_strt(in_tS_s), t_end(in_tS_e), f_phase(in_fPhase) {}

// raw data ctor
wave_component(float in_phase, float in_per, float in_amp, size_t in_tS_s, size_t in_tS_e) 
	: amp(in_amp), t_strt(in_tS_s), t_end(in_tS_e)
{
	this->phase = (int16_t) (in_phase / TIMESTEP_WF);
	this->period = (int16_t) (in_per / TIMESTEP_WF);
	this->f_phase = std::fmod( phase, TIMESTEP_WF );
}

// empty ctor for first element, only pass the next update time
wave_component(size_t in_step)
	: phase(0), period(0), amp(0.0), t_strt(in_step), t_end(in_step+1), f_phase(0.0) {}

// "copy" ctor that sets amp to V_SPIKEAMP if activ is true
wave_component(const wave_component & cpy, bool activ = false)
{
	this->phase = cpy.phase;
	this->period = cpy.period;

	this->t_strt = cpy.t_strt;
	this->t_end = cpy.t_end;
	this->f_phase = cpy.f_phase;

	if (activ) this->amp = V_SPIKEAMP;
	else this->amp = cpy.amp;
}


// "interference" ctor that returns a waveform created from constructive interference of the two components
wave_component(const wave_component & A, const wave_component & B, bool activ = false)
{
	// TODO: interference ctor
	// solve for phase using exEuclid
	// xEuclid solves the equation a * n_A + b * n_B = 1
	// we start with p_A * n_A + phi_A = p_B * n_B + phi_B
	int phi = abs(A.phase - B.phase);
	if (phi == 0) this->phase = A.phase;
	else
	{
		int n_A, n_B;
		xEuclid(A.period/phi, B.period/phi, n_A, n_B);
		CHK_WARNING( (A.period * n_A + A.phase != B.period * n_B + B.phase), "phi_X calculation broken" );
		this->phase = A.period * n_A + A.phase;
	}

	// solve for period using LCM
	this->period = LCM(A.period, B.period);

	// start time max of both
	this->t_strt = std::max(A.t_strt, B.t_strt);
	// end time min of both
	this->t_end = std::min(A.t_end, B.t_end);

	// REVIEW: f_phase set to avg, change this?
	this->f_phase = (A.f_phase + B.f_phase) / 2;

	if (activ) this->amp = V_SPIKEAMP;
	else this->amp = A.amp + B.amp;
}

};


inline bool wave_component_compare(wave_component & lhs, wave_component & rhs)
{
	// order by start time first, then by period
	if (lhs.t_strt < rhs.t_strt) return true;
	else if (lhs.t_strt > rhs.t_strt) return false;
	else return (lhs.period < rhs.period);	
}

inline bool wave_component_period_compare(wave_component & lhs, wave_component & rhs)
{
	return (lhs.period < rhs.period);
}




// waveform
typedef std::vector < wave_component > waveform;





// * waveform operations

// add contents of one waveform to another
// NOTE: modifies waveform "main"
void wf_concat(waveform & main, waveform & add)
{
	main.reserve(main.size() + add.size());
	main.insert( main.end(), add.begin(), add.end() );
}

// takes in input waveform, sorts, returns activation waveform
waveform wf_activation(waveform & wf, float min_amp = V_THRESHOLD)
{
	__DEBUG("wf_act, input size: " + std::to_string(wf.size()));
	// REVIEW: this sort might not be needed, adding for safety for now
	std::sort(wf.begin(), wf.end(), wave_component_period_compare);
	// update t_strt for all the elements where t_strt < TIME
	size_t idx;
	// iterate until t_strt is past the current time, or end of array
	for ( idx = 0; (wf[idx].t_strt < TIME) && (idx < wf.size()); idx++ )
	{
		wf[idx].t_strt = TIME;
	}

	// resort the first elements of the vector with t_strt = TIME
	// find last such element
	for ( idx; (wf[idx].t_strt < TIME + 1) && (idx < wf.size()); idx++ ) {}
	// REVIEW: check for off by one in idx usage

	// sort only by period
	std::sort(wf.begin(), wf.begin() + idx, wave_component_period_compare);

	__DEBUG("idx: " + std::to_string(idx));

	// ~ wf_compress(wf, i);

	// declare output waveform, idxs of components for analysis
	// OPTIMIZE: reserve space?
	waveform wf_out;
	std::vector<size_t> wf_small;

	for (size_t j = 0; j < idx; j++)
	{
		// copy all spikes with amp > threshold, setting the new amp to spike amplitude
		if (wf[j].amp > min_amp) wf_out.emplace_back(wf[j], true);
		else if (wf[j].amp > V_IGNORE) wf_small.push_back(j);
		TEST( " \t " + std::to_string(j) + "\t" + std::to_string(wf[j].amp));
	}

	__DEBUG("naive wf_out size: " + std::to_string(wf_out.size()));
	__DEBUG("wf_small size: " + std::to_string(wf_small.size()));

	// harmonic analysis for spikes that are too small on their own
	// loop over every pair of components in wf_small
	// and if they sum to > min_amp, use interference ctor
	for (size_t x = 0; x < wf_small.size(); x++)
	{
		size_t i = wf_small[x];
		__DEBUG("\t i = " + std::to_string(i));
		for (size_t y = 0; y < x; y++)
		{
			size_t j = wf_small[y];
			__DEBUG("\t\t j = " + std::to_string(j));
			if (wf[i].amp + wf[j].amp > min_amp)
			{
				TEST("\t\t A0");
				// test for interference using gcd
				// REVIEW: crashes if i==j, figure out why. probably irrelevant but idk
				int gcd = GCD(wf[i].period, wf[j].period);
				TEST("\t\t gcd -- " + std::to_string(gcd));
				int phi = abs(wf[i].phase - wf[j].phase);
				TEST("\t\t phi -- " + std::to_string(phi));

				if ( (phi == 0) || (gcd % phi == 0) )
				{
					TEST("\t\t   B0");
					// add using interference ctor
					// TODO: check for period/phase greater than TIMESTEP_DIFF ?
					wf_out.emplace_back(wf[i], wf[j], true);
				}
				TEST("\t\t   C0");
			}
		}
	}

	__DEBUG("done wf_act, out size: " + std::to_string(wf_out.size()));

	// TODO: kill/adjust spikes that are too close
	
	return wf_out;
}

waveform wf_read(const std::string & filename)
{
	// output
	waveform wf_out;

	// read using csv parser
	io::CSVReader<5> in(filename);	
	in.read_header(io::ignore_extra_column, "phase", "period", "amp", "t_strt", "t_end");
	// NOTE: in csv file, phase and period stored explicitly
	float phase; float period; float amp; size_t t_strt; size_t t_end;
	while(in.read_row(phase, period, amp, t_strt, t_end))
	{
		// ~ we convert the values
		// ~ wf_out.emplace_back( (int16_t) (phase / TIMESTEP_WF), (uint16_t)(period / TIMESTEP_WF), amp, t_strt, t_end, std::fmod( phase, TIMESTEP_WF ) );
		wf_out.emplace_back(phase, period, amp, t_strt, t_end);
	}

	return wf_out;
}

bool wf_save(waveform & wf, const std::string & filename)
{
	std::ofstream fout;
	fout.open(filename);
	if (!fout.is_open()) return false;
	// print first line
	fout << "phase, period, amp, t_strt, t_end\n";

	// print all waveform components
	for (auto x : wf)
	{
		// ~ fout << "%f, %f, %f, %i, %i \n", ((float)(x.phase * TIMESTEP_WF) + x.f_phase, (float)(x.period * TIMESTEP_WF), x.amp, x.t_strt, x.t_end);
		fout << (float)(x.phase * TIMESTEP_WF) + x.f_phase << ", "
			 << (float)(x.period * TIMESTEP_WF) << ", "
			 << x.amp << ", "
			 << x.t_strt << ", "
			 << x.t_end
			 << " \n";
	}

	return true;
}





#endif