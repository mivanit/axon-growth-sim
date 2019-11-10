// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#include <vector>
#include <cstdlib>
#include <ctime>

#include "diffusion/diffusion.h"
#include "network/neuron.h"
#include "axon/axon.h"

// load configs from file
#include "run/consts.h"

int main(int argc, char ** argv)
{
	srand(time(0)); // initialize random number generator


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

		neurons.emplace_back(i, 1, Coord(float(x_init), float(y_init)));

		// TODO: hardcoded NT release RNG
		// neurons.back().NT_end
	}


	//* run simulation
	for (TIME; TIME < N_STEPS; ++TIME) {
		// update all the grids
		for (auto & g : dGrids) g.adi_step();

		// update all the neurons (which will in turn update axons)
		for (auto & nrn : neurons) nrn.update();
	}

}
