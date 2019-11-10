// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#include <vector>
#include <cstdlib>
#include <ctime>

#include "diffusion/diffusion.h"
#include "network/neuron.h"

// load configs from file
#include "run/consts.h"

int main(int argc, char ** argv)
{
	srand(time(0)); // initialize random number generator

	// initialize diffusion grid for each neurotrophic factor
	std::vector<Diffusion> grids(1, Diffusion(N_GRIDSIZE, 1, 1, 1, 1));

	// initialize neurons
	std::vector<Neuron> neurons;
	neurons.reserve(N_NEURONS);
	for (int i = 0; i < N_NEURONS; ++i) {
		// generate random initial coordinates
		
		// REVIEW: clustering of positions, rang gen chem type
		int x_init = rand() % N_GRIDSIZE;
		int y_init = rand() % N_GRIDSIZE;

		neurons.emplace_back(i, 1, Coord(float(x_init), float(y_init));
	}

	// run simulation for N_STEPS steps
	/*
	for (TIME; TIME < N_STEPS; ++TIME) {
		// update all the grids
	}
	*/

}
