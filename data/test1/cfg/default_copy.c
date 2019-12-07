// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// template header for configuring consts

#include <cstdint>
// #include <stdint.h>
#include <math.h>
#include <random>
#include <vector>

#include "config.h"


// * Time-related

// timestep for diffusion, in ms
extern const float TIMESTEP_DIFF = 1000.0;

// timestep for discrete waveform
extern const float TIMESTEP_WF = 10.0;

// current timestep count
// size_t TIME = 0;

// maximum timestep count
size_t N_STEPS = 1000;


// * network specs

extern const uint16_t N_NEURONS = 50;
extern const uint16_t N_GRIDSIZE = 100;
extern const uint16_t N_PADDING = 10;

// Range for chem types, cell types
extern const uint8_t MAX_CELLTYPE = 2;
extern const uint8_t MAX_CHEMTYPE = 2;


// * neuron specs

// resting voltage
extern const float V_REST = 0.0;

// default threshold voltage to fire neuron
extern const float V_THRESHOLD = 5.0;

// default amplitude of spikes, if threshold crossed
extern const float V_SPIKEAMP = 110.0;

// ignore spikes smaller than this amplitude
extern const float V_IGNORE = 1.0;

// time to reach spike voltage
extern const float T_RASING = 0.6;

// time to decay back to 0 (does not consider recovery period)
extern const float T_DECAY = 2.4;

// minimum delay time between spikes
extern const float T_MIN_DELAY = 5.1;

//* common diffusion consts
extern const float DIFF_dx = 1.0;
extern const float DIFF_dy = 1.0;
extern const float DIFF_dt = 1.0;



// * floating point arithmetic consts

// margin for comparing floats
// #define EPSILON 0.01f;
extern const float EPSILON = 0.001;

// Decimal precision for writing to files
extern const int PRECISION = 8;

//* chem/cell type definitions
// UGLY: improve configuration method?


std::vector< chemType > init_chemType_arr()
{
	std::vector< chemType > CHEMTYPE_ARR;
	CHEMTYPE_ARR.reserve(MAX_CHEMTYPE);

	CHEMTYPE_ARR.emplace_back(
		0, // chemType_ID
		0.0, // ambient
		0.1, // r_decay
		1.0, // r_diff
		"EXCITE" // label
	);

	CHEMTYPE_ARR.emplace_back(
		1, // chemType_ID
		0.0, // ambient
		0.1, // r_decay
		1.0, // r_diff
		"INHIBIT" // label
	);

	return CHEMTYPE_ARR;
}



std::vector< cellType > init_cellType_arr()
{
	std::vector< cellType > CELLTYPE_ARR;
	CELLTYPE_ARR.reserve(MAX_CELLTYPE);
	
	CELLTYPE_ARR.emplace_back(
		0, // cellType_ID
		1.0, // stepSize_mu
		0.1, // stepSize_sigma
		0.0, // searchAngle_tau
		0.5, // turningRate_max
		std::vector<float>{0.1, 0.1}, // senseNoise_sigma
		std::vector<float>{1.0, -1.0}, // chemType_affinities
		std::vector<float>{0.1, 1.0} // chemType_release
	);

	CELLTYPE_ARR.emplace_back(
		1, // cellType_ID
		1.0, // stepSize_mu
		0.1, // stepSize_sigma
		0.0, // searchAngle_tau
		0.5, // turningRate_max
		std::vector<float>{0.1, 0.1}, // senseNoise_sigma
		std::vector<float>{-1.0, 1.0}, // chemType_affinities
		std::vector<float>{1.0, 0.1} // chemType_release
	);

	return CELLTYPE_ARR;
}




