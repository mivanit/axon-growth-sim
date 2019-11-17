#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../network/neuron.h"
#include "../axon/axon.h"

#ifndef DRIVER
#define DRIVER


class Driver
{
public:
	//* actual data
	std::vector<Diffusion> dGrids;
	std::vector<Neuron> neurons;

	//* RNG stuff
	unsigned int RNG_seed;
	std::default_random_engine rng;
	std::normal_distribution<float> rdist_timeOn(30, 5); // RNG for time neuron stays on

	// default ctor
	Driver()
		: dGrids(1, Diffusion(N_GRIDSIZE, 1, 1, 1, 1)), RNG_seed(time(0))
	{
		srand(RNG_seed); // initialize random number generator

		// create neurons randomly
		neurons.reserve(N_NEURONS);
		for (int i = 0; i < N_NEURONS; ++i) 
		{	
			// generate random initial coordinates	
			// REVIEW: clustering of positions, rang gen chem type
			int x_init = rand() % N_GRIDSIZE;	
			int y_init = rand() % N_GRIDSIZE;

			// create new neuron
			// TODO: neuron chemtypes
			Neuron new_neuron(i, 1, Coord(float(x_init), float(y_init)));


			// model start time using uniform random distribution
			new_neuron.NT_start = rand() % N_STEPS;
			// model time neuron stays on using normal distribution
			new_neuron.NT_end = rdist_timeOn(rng) + new_neuron.NT_start;
			new_neuron.NT_amt = 1;

			// model amount to release randomly
			neurons.push_back(new_neuron);
		}
	}




}


void sim_step(){
	// update all grids
	for (auto & g : dGrids) g.adi_step();

	// update all the neurons (which will in turn update axons)
	for (auto & nrn : neurons) nrn.update();
}



// Diffusion d(100, 1, 1, 1, 0.1);
//     d.set(25, 25, 20);
//     d.set_decay(0.1);
//     for (unsigned int i = 0; i < 500; ++i) {
//         d.adi_step();
//         if (i % 50 == 0) {
//             d.write_to_csv("outputs/diffusion_" + std::to_string(i) + ".csv");
//         }
//         if (i == 100) {
//             d.set(75, 75, 15);
//         }
//     }
//     d.write_to_csv("outputs/diffusion_" + std::to_string(500) + ".csv");
//     return 0;

#endif