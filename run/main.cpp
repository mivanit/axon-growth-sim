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

// load configs from file
int main(int argc, char ** argv)
{
	Driver simulation;
	
	for (TIME; TIME < N_STEPS; ++TIME) 
	{
		simulation.sim_step();
	}
}
