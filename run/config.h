// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// template header for configuring consts

#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <math.h>

//* Time-related

// timestep for diffusion, in ms
extern const float TIMESTEP_DIFF;

// timestep for discrete waveform
extern const float TIMESTEP_WF;

// current timestep count
extern size_t TIME;

// maximum timestep count
extern size_t N_STEPS;


//* network specs

extern const uint16_t N_NEURONS;
extern const uint16_t N_GRIDSIZE;
extern const uint16_t N_PADDING;

// Range for chem types
extern const uint8_t MAX_CELLTYPE;
extern const uint8_t MAX_CHEMTYPE;


//* neuron specs

// resting voltage
extern const float V_REST;

// default threshold voltage to fire neuron
extern const float V_THRESHOLD;

// default amplitude of spikes, if threshold crossed
extern const float V_SPIKEAMP;

// ignore spikes smaller than this amplitude
extern const float V_IGNORE;

// time to reach spike voltage
extern const float T_RASING;

// time to decay back to 0 (does not consider recovery period)
extern const float T_DECAY;

// minimum delay time between spikes
extern const float T_MIN_DELAY;



//* floating point arithmetic extern consts

// margin for comparing floats
// #define EPSILON 0.01f;
extern const float EPSILON;


// Decimal precision for writing to files
extern const int PRECISION;


//* chem type classes
// TODO: all chemType stuff

struct cellType
{
public:
	// ID of this chemtype instance
	uint16_t cellType_ID; 
	
	// stepsize as normal distribution
	// UGLY: make this poisson/exponential instead?
	float stepSize_mu, stepSize_sigma;


	// search angle
	float searchAngle_tau;

	// maximum turning rate for the axon
	float turningRate_max;	

	// noise term for sensing certain chemtypes
	float * senseNoise_sigma;
	
	// axonal affinities for the various chemical types
	// positive --> turn towards
	// negative --> turn away
	float * chemType_affinities; 


	// emission coefficients for various chem types
	// positive --> proportional to activity
	// negative --> inverse of activity
	float * chemType_release;
};

#endif
