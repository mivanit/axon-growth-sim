// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// template header for defining consts
// there should be a seperate file for every network run

#ifndef CONSTS
#define CONSTS

#include <cstdint>
#include <queue>
#include <vector>
#include <math.h>
#include <cstdint>
#include <random>

// * RNG stuff
std::random_device rd;
std::mt19937 e2(rd());


// * Time-related

// timestep for diffusion, in ms
const float TIMESTEP_DIFF = 1000.0;

// timestep for discrete waveform
const float TIMESTEP_WF = 10.0;

// current timestep count
size_t TIME = 0;



// * network specs

const uint16_t N_NEURONS = 50;
const uint16_t N_GRIDSIZE = 100;
const uint16_t N_PADDING = 10;
const float MU_CONN = 3.0;
const float SIG_CONN = 4.0;
const uint16_t MAX_CONN = 10;


// Range for chem types
const uint8_t _MINCHEM_ = 1;
const uint8_t _MAXCHEM_ = 10;

// rand distribution definitions for synapse stuff
const float MU_DELAY = 200;
const float SIG_DELAY = 150;
const float MU_ALPHA = 0;
const float SIG_ALPHA = 0.05;

// * input stim specs

const float F_STIM = 0.2;

const char * CSTR_STIMNAME = "wf_";
const char * CSTR_STIMEXT = ".csv";







// * neuron specs

// resting voltage
const float V_REST = 0.0;

// default threshold voltage to fire neuron
const float V_THRESHOLD = 5.0;

// default amplitude of spikes, if threshold crossed
const float V_SPIKEAMP = 110.0;

// ignore spikes smaller than this amplitude
const float V_IGNORE = 1.0;


// time to reach spike voltage
const float T_RASING = 0.6;

// time to decay back to 0 (does not consider recovery period)
const float T_DECAY = 2.4;

// minimum delay time between spikes
const float T_MIN_DELAY = 5.1;





// * typedefs

// Typedef for coordinate pair of unsigned ints
// typedef std::pair<unsigned int, unsigned int> Coord;
struct Coord
{
	double x;
	double y;

	double operator [](int i) 
	{
		if (i == 0) return x;
		else return y;
	}

	/* get vector magnitude */
	double mag()
	{
		return pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
	}

	/* scale vector by some factor */
	void scale(float scaling_factor)
	{
		x *= scaling_factor;
		y *= scaling_factor;
	}

	/* normalize vector */
	void norm()
	{
		this.scale(1 / this.mag);
	}
};




// * floating point arithmetic consts

// margin for comparing floats
// #define EPSILON 0.01f;
const float EPSILON = 0.01;


// code for comparing two floats
inline bool comp_f(float a, float b)
{
    return fabs(a - b) <= EPSILON;
}

inline bool zero_f(float a)
{
	return fabs(a) <= EPSILON;
}



// * chem type classes and instances

struct Axon_chemType
{
	public:
	float stepSize;
	// TODO: noise term
	// TODO: neurotrophin attraction
	// TODO: turning rate

};




#endif