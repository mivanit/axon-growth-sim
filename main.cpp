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
	std::vector<Diffusion<float>> grids(1, Diffusion<float>(GRID_SIZE));

	// initialize neurons
	std::vector<Neuron> neurons;
	neurons.reserve(N_NEURONS);
	for (int i = 0; i < N_NEURONS; ++i) {
		// generate random initial coordinates
		int x_init = rand() % GRID_SIZE;
		int y_init = rand() % GRID_SIZE;
		neurons.emplace_back(i, _MINCHEM_, x_init, y_init);
	}

	// run simulation for N_STEPS steps
	/*
	for (TIME; TIME < N_STEPS; ++TIME) {
		// update all the grids
	}
	*/

}
