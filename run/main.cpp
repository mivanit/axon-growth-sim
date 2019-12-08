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
#include "../network/neuron.h"
#include "../axon/axon.h"
#include "driver.h"

void print_info(std::string name){
	printf("==================================================");
	printf("simulation initialized with name \t %s", name);
	// REVIEW: print more info here
	printf("starting simulation");
	printf("==================================================");
}


// load configs from file
int main(int argc, char ** argv)
{
	// initialize driver with name
	std::string sim_name = argv[1];
	Driver simulation(sim_name);
	// Axon::init_dGrid_ref(simulation.dGrids);
	
	// print some info to console
	print_info(sim_name);

	// run simulation
	simulation.run();
}





