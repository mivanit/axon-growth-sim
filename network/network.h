// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NETWORK
#define NETWORK

#include <random>

#include "../run/consts.h"
#include "../run/config.h"

#include "neuron.h"


class Network
{
public:
	
	// data
	std::vector<Neuron> neurons;
	
	// ctors
	Network()
	{
		gen_neurons();
	}

	
	// pass along activity, update every neuron individually
	void update()
	{
		// FIXME: network update function
		// first pass: forward activity
		

		// second pass: update each neuron (and by extension, axons)



	}



	// generates a set of neurons with uniformly random coordinates
	void gen_neurons()
	{
		// create neurons randomly
		neurons.reserve(N_NEURONS);
		for (int i = 0; i < N_NEURONS; ++i)
		{
			// generate random initial coordinates	
			// REVIEW: clustering of positions, range gen chem type, padding
			
			// create new neuron
			Neuron new_neuron(
				i, // neuron ID
				(rand() % MAX_CELLTYPE), // cellType
				Coord( //coordinate
					float(rand() % N_GRIDSIZE), 
					float(rand() % N_GRIDSIZE)
				)
			); 
			
			// // model start time using uniform random distribution
			// new_neuron.NT_start = rand() % N_STEPS;
			// // model time neuron stays on using normal distribution
			// new_neuron.NT_end = rdist_timeOn(rng) + new_neuron.NT_start;
			// model amount to release randomly
			// new_neuron.NT_amt = 1;

			neurons.push_back(new_neuron);
		}
	}

	// FIXME: state saving to stringstream


};




#endif
