// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// template header for configuring consts

#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <math.h>
#include <vector>
#include <string>
#include <random>

//* Time-related

// timestep for diffusion, in ms
extern const float TIMESTEP_DIFF;

// timestep for discrete waveform
extern const float TIMESTEP_WF;

// current timestep count
// extern size_t TIME;

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


//* common diffusion consts
// UGLY: should be able to specify common diffusion consts per-grid, but need to rework concentration updating as well as location specifying

extern const float DIFF_dx;
extern const float DIFF_dy;
extern const float DIFF_dt;



//* floating point arithmetic extern consts

// margin for comparing floats
// #define EPSILON 0.01f;
extern const float EPSILON;


// Decimal precision for writing to files
extern const int PRECISION;


//* chem/cell type stuff

// stores various data about how a given chem type behaves
struct chemType
{
public:
	// ID of this chemtype instance
	uint16_t chemType_ID; 
	
	// ambient concentration
	float ambient;

	// decay rate
	float r_decay;

	// diffusion rate
	float r_diff;

	// label of chemical
	std::string label;

	// default empty ctor
	chemType() {}

	// ctor
	chemType(
		uint16_t in_chemType_ID,
		float in_ambient, 
		float in_r_decay,
		float in_r_diff,
		std::string in_label
	) : 
		chemType_ID(in_chemType_ID ),
		ambient(in_ambient),
		r_decay(in_r_decay),
		r_diff(in_r_diff),
		label(in_label)
	{}
};

// stores various data about how a given cell type behaves
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

	// coefficient to multiply orthogonal vector by to get prong vector
	// lambda * w + v = p
	// where v is facing direction, w is orthogonal vec, p is prong vector
	float searchAngle_lambda;


	// search distance (prong length)
	float searchDist;

	// maximum turning rate for the axon
	float turningRate_max;	

	// base activation
	float base_activ;

	// activation multiplier (positive --> excitatory, negative --> inhibitory)
	float activ_scale;

	// noise term for sensing certain chemtypes
	std::vector<float> senseNoise_sigma;


	// axonal affinities for the various chemical types
	// positive --> turn towards
	// negative --> turn away
	std::vector<float> chemType_affinities;


	// emission coefficients for various chem types
	// positive --> proportional to activity
	// negative --> inverse of activity
	std::vector<float> chemType_release;

	// RNG for distance travelled
	std::normal_distribution<float> rdist_move;

	// default empty ctor
	cellType() {}

	// ctor
	cellType(
		uint16_t in_cellType_ID,
		float in_stepSize_mu, 
		float in_stepSize_sigma,
		float in_searchAngle_tau,
		float in_searchDist,
		float in_turningRate_max,
		float in_base_activ,
		float in_activ_scale,
		std::vector<float> in_senseNoise_sigma,
		std::vector<float> in_chemType_affinities,
		std::vector<float> in_chemType_release
	) : 
		cellType_ID(in_cellType_ID ),
		stepSize_mu(in_stepSize_mu),
		stepSize_sigma(in_stepSize_sigma),
		searchAngle_tau(in_searchAngle_tau),
		searchDist(in_searchDist),
		turningRate_max(in_turningRate_max),
		base_activ(in_base_activ),
		activ_scale(in_activ_scale),
		senseNoise_sigma(in_senseNoise_sigma),
		chemType_affinities(in_chemType_affinities),
		chemType_release(in_chemType_release),
		rdist_move(in_stepSize_mu, in_stepSize_sigma)
	{
		searchAngle_lambda = pow( (- 1 + pow( searchAngle_tau, -2 )), 0.5 );
	}
};



extern std::vector< chemType > CHEMTYPE_ARR;
extern std::vector< cellType > CELLTYPE_ARR;

// // std::vector< chemType > init_chemType_arr();
// // std::vector< cellType > init_cellType_arr();
// // hardcoded release
// class Hard_release {
// public:
// 	size_t t_start;
// 	size_t t_end;
// 	float mag;

// 	float get_release(size_t t)
// 	{
// 		if (
// 			(t >= t_start)
// 			&& (t < t_end)
// 		){
// 			return mag;
// 		}
// 		else
// 		{
// 			return 0.0;
// 		}
// 	}

// 	Hard_release() {}

// 	Hard_release(size_t in_t_start, size_t in_t_end, float in_mag)
// 		: t_start(in_t_start), t_end(in_t_end), mag(in_mag) {}

// }; // end class defn `Hard_release`


// std::vector< Hard_release > RELEASE_ARR({
// 	Hard_release(
// 		0,
// 	),
// });


#endif
