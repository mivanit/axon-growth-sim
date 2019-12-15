// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#include <vector>
#include <cstdlib>
#include <ctime>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../neuro/neuron.h"
#include "../neuro/axon.h"

// load configs from file

int main(int argc, char ** argv)
{
	srand(time(0)); // initialize random number generator
	std::default_random_engine rng;
	std::normal_distribution<float> rdist_timeOn(30, 5); // RNG for time neuron stays on

	//* initialize diffusion grid for each neurotrophic factor
	std::vector<Diffusion> dGrids(1, Diffusion(N_GRIDSIZE, 1, 1, 1, 1));

	// init static ref to dGrids in Axon, Neuron classes
	Axon::dGrids = &dGrids;
	Neuron::dGrids = &dGrids;

	//* initialize neurons (and in turn axons)
	std::vector<Neuron> neurons;
	neurons.reserve(N_NEURONS);
	for (int i = 0; i < N_NEURONS; ++i) {
		// generate random initial coordinates
		
		// REVIEW: clustering of positions, rang gen chem type
		int x_init = rand() % N_GRIDSIZE;	
		int y_init = rand() % N_GRIDSIZE;

		// create new neuron
		Neuron new_neuron(i, 1, Coord(float(x_init), float(y_init)));


		// model start time using uniform random distribution
		new_neuron.NT_start = rand() % N_STEPS;
		// model time neuron stays on using normal distribution
		new_neuron.NT_end = rdist_timeOn(rng) + new_neuron.NT_start;
		new_neuron.NT_amt = 1;

		// model amount to release randomly
		neurons.push_back(new_neuron);
	}


	//* run simulation
	for (TIME; TIME < N_STEPS; ++TIME) {
		// update all grids
		for (auto & g : dGrids) g.adi_step();

		// update all the neurons (which will in turn update axons)
		for (auto & nrn : neurons) nrn.update();
	}

}
