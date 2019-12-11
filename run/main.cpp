// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../network/neuron.h"
#include "../axon/axon.h"
#include "driver.h"

void print_info(std::string name){
	printf("==================================================\n");
	printf("simulation initialized with name \t %s\n", name.c_str());
	// REVIEW: print more info here
	printf("starting simulation\n");
	printf("==================================================\n");
}


// load configs from file
int main(int argc, char ** argv)
{
	// TODO: check args, print help msg

	// initialize driver with name
	std::string sim_name = "test";
	int sim_time = 500;
	int sim_save_step = 50;
	
	if (argc > 1)
	{
		sim_name = argv[1];
	}
	if (argc > 2) {
		sim_time = std::stoi(argv[2]);
		sim_save_step = std::stoi(argv[3]);
	}

	Driver simulation(sim_name);
	
	// print some info to console
	print_info(sim_name);

	// run simulation
	simulation.run(sim_time, sim_save_step);
}





