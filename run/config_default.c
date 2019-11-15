// * Time-related

// timestep for diffusion, in ms
TIMESTEP_DIFF = 1000.0;

// timestep for discrete waveform
TIMESTEP_WF = 10.0;

// current timestep count
TIME = 0;

// maximum timestep count
N_STEPS = 1000;


// * network specs

N_NEURONS = 50;
N_GRIDSIZE = 100;
N_PADDING = 10;

// Range for chem types
_MINCHEM_ = 1;
_MAXCHEM_ = 10;

// rand distribution definitions for synapse stuff
MU_DELAY = 200;
SIG_DELAY = 150;
MU_ALPHA = 0;
SIG_ALPHA = 0.05;


// * neuron specs

// resting voltage
V_REST = 0.0;

// default threshold voltage to fire neuron
V_THRESHOLD = 5.0;

// default amplitude of spikes, if threshold crossed
V_SPIKEAMP = 110.0;

// ignore spikes smaller than this amplitude
V_IGNORE = 1.0;


// time to reach spike voltage
T_RASING = 0.6;

// time to decay back to 0 (does not consider recovery period)
T_DECAY = 2.4;

// minimum delay time between spikes
T_MIN_DELAY = 5.1;



// * floating point arithmetic consts

// margin for comparing floats
// #define EPSILON 0.01f;
EPSILON = 0.01;
