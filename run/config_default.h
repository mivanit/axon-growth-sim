// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// template header for configuring consts
// there should be a seperate file for every network run

// 

#ifndef CONFIG_VALS
#define CONFIG_VALS

#include <cstdint>
#include <math.h>
#include <random>


// CRIT: keep defns in header, set values in .c file. This gets rid of the linker errors, and also makes it easier to switch out config files

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

// maximum timestep count
size_t N_STEPS = 1000;


// * network specs

const uint16_t N_NEURONS = 50;
const uint16_t N_GRIDSIZE = 100;
const uint16_t N_PADDING = 10;

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



// * floating point arithmetic consts

// margin for comparing floats
// #define EPSILON 0.01f;
const float EPSILON = 0.01;

#endif