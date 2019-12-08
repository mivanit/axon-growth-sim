#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <sstream>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../network/network.h"
#include "../network/neuron.h"
#include "../axon/axon.h"

#include "../util/err_util.h"
#include "../util/rng_def.h"

#ifndef DRIVER
#define DRIVER


class Driver {
public:
/*
##     ##    ###    ########   ######
##     ##   ## ##   ##     ## ##    ##
##     ##  ##   ##  ##     ## ##
##     ## ##     ## ########   ######
 ##   ##  ######### ##   ##         ##
  ## ##   ##     ## ##    ##  ##    ##
   ###    ##     ## ##     ##  ######
*/
	size_t TIME;

	//* actual data
	std::vector<Diffusion> dGrids;
	Network net;
	// std::vector<Neuron> neurons;

	//* RNG stuff
	unsigned int RNG_seed;
	// std::default_random_engine rng; // declared in config.h
	std::normal_distribution<float> rdist_timeOn; // RNG for time neuron stays on

	//* name 
	std::string NAME; 


/*
 ######  ########  #######  ########
##    ##    ##    ##     ## ##     ##
##          ##    ##     ## ##     ##
##          ##    ##     ## ########
##          ##    ##     ## ##   ##
##    ##    ##    ##     ## ##    ##
 ######     ##     #######  ##     ##
*/

	// main ctor, inherits everything from config.h
	Driver(std::string name_in = "Test") {
		TIME = 0;

		//* RNG setup
		RNG_seed = time(0);
		srand(RNG_seed);

		//* name setup
		NAME = name_in;

		//* dGrid setup

		// NOTE: IF YOU REMOVE THIS IT WILL SEGFAULT
		// no copy ctor/dtor for dGrid
		dGrids.reserve(MAX_CHEMTYPE);

		for (chemType x : CHEMTYPE_ARR)
		{
			dGrids.emplace_back(
				N_GRIDSIZE, // grid dim
				x.r_diff,
				DIFF_dx, 
				DIFF_dy, 
				DIFF_dt,
				x.label
			);
			dGrids.back().set_decay(x.r_decay);
		}

		Axon::init_dGrid_ptr(&dGrids);

		//* neuron setup
		net.gen_neurons();
	}


/*
 ######  ######## ######## ########
##    ##    ##    ##       ##     ##
##          ##    ##       ##     ##
 ######     ##    ######   ########
      ##    ##    ##       ##
##    ##    ##    ##       ##
 ######     ##    ######## ##
*/
	void sim_step() {
		// update all grids
		for (auto & g : dGrids) {
			g.adi_step();
		}

		// update all the neurons (which will in turn update axons)
		for (auto & nrn : net.neurons) {
			nrn.update();
		}

		TIME++;
	}

	/*
	- run the simulation until `fin_step`
	- save when `TIME % save_every == 0`
	- print info when `TIME % print_every == 0`
		- `verbosity == 0`  :  dont print
		- `verbosity == 0`  :  just timestep
		- `verbosity == 1`  :  timestep, number of active axons (TODO)
	*/
	void run(
		size_t fin_step = N_STEPS, 
		size_t save_every = 1,
		int print_every = 1,
		int verbosity = 1
	) {
		while (TIME < fin_step) 
		{
			// run step
			sim_step();
			// save if needed
			if (TIME % save_every == 0)
			{
				save_state();
			}
			
			// print some information
			if (TIME % print_every == 0)
			{
				print_info(verbosity);
			}
		}
	}



/*
 ######     ###    ##     ## ########
##    ##   ## ##   ##     ## ##
##        ##   ##  ##     ## ##
 ######  ##     ## ##     ## ######
      ## #########  ##   ##  ##
##    ## ##     ##   ## ##   ##
 ######  ##     ##    ###    ########
*/
	// REVIEW: move write functions to their respective classes?
	// Write the state of the network at time step t
	void save_state() {
		// relies on build script to create this folder
		// const std::string DIR = "../data/" + NAME + "/raw/";
		const std::string DIR = "raw/";
		// TODO: check for validity of path
		std::ostringstream ss;
		ss << std::setw(5) << std::setfill('0') << std::to_string(TIME);
		const std::string END = "_" + ss.str() + ".tsv";
		neuron_write(DIR + "neur" + END);
		axon_write(DIR + "axon" + END);
		diffusion_write(DIR + "diff" + END);
	}
	
	void neuron_write(const std::string& file) const {
		// init ofstream
		std::ofstream ofs(file, std::ios_base::app);
		CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.");
		ofs.precision(PRECISION);
		

		// write neuron data (without axon data)
		// format:
		// for every neuron in order, write:
		/*
			==========
			<neuron.id_neuron>, <neuron.id_cellType>, <neuron.loc>
			[waveform arrays]
		*/
		// // for (Neuron& neuron : net.neurons) {
		for (auto & neuron : net.neurons) {
			ofs << "==========\n";
			ofs << neuron.id_neuron << "\t"
				<< neuron.id_cellType << "\t"
				<< neuron.loc.to_str() << "\t"
				<< "\n";

			// FIXME: print waveform arrays for neuron write

			// additional newline, flush stream
			ofs << std::endl;
		}
	}
	
	void axon_write(const std::string& file) const {
		// init ofstream
		std::ofstream ofs(file, std::ios_base::app);
		CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.");
		ofs.precision(PRECISION);
		
		// write axon data
		// format:
		// for every axon in order, write:
		/*
			==========
			<axon.id_neuron>, <axon.id_cellType>, <axon.loc>, <axon.dir>
			[past_loc array]
			[postSyn_id array]
			[postSyn_wgt array]
		*/
		// // for (Neuron& neuron : net.neurons) {
		// // for (int n_idx = 0; n_idx < net.neurons.size(); n_idx++) {
		// // Axon& axon = net.get_neuron_ref(n_idx).axon;
		for (auto & nrn : net.neurons) {
			ofs << "==========\n";
			ofs << nrn.axon.id_neuron << "\t"
				<< nrn.axon.id_cellType << "\t"
				<< nrn.axon.loc().to_str() << "\t"
				<< nrn.axon.dir.to_str() << "\t"
				<< "\n";
			
			// locations
			for (const Coord& c : nrn.axon.past_loc) {
				ofs << c.to_str() << "\t";
			}
			ofs << "\n";

			// post stynaptic connection IDs
			for (uint16_t id : nrn.axon.postSyn_id) {
				ofs << std::to_string(id) << "\t";
			}
			ofs << "\n";

			// post stynaptic weights
			for (float wgt : nrn.axon.postSyn_wgt) {
				ofs << std::to_string(wgt) << "\t";
			}
			ofs << "\n";
			
			// additional newline, flush stream
			ofs << std::endl;
		}
	}


	void diffusion_write(const std::string& file) const {
		// init ofstream
		std::ofstream ofs(file, std::ios_base::app);
		CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.");
		ofs.precision(PRECISION);
		
		// write diffusion grid data
		// format:
		// for every diffusion grid, in order:
		/*
			==========
			<grid.label>
			[array]
		*/
		for ( unsigned int g_idx = 0; g_idx < dGrids.size(); g_idx++ ) {

			const Diffusion& d = dGrids[g_idx];

			ofs << "==========\n"; // separator
			ofs << d.label() << "\n"; // label of grid

			// output array
			for (unsigned int i = 0; i < d.dim(); ++i) {
				for (unsigned int j = 0; j < d.dim(); ++j) {
					ofs << std::to_string(d.concentration(j, i)) << "\t";
				}
				ofs << "\n";
			}
			// additional newline, flush stream
			ofs << std::endl;
		}
	}

	/*
	- `verbosity == 0`  :  dont print
	- `verbosity == 0`  :  just timestep
	- `verbosity == 1`  :  timestep, number of active axons (TODO)
	*/
	void print_info(int verbosity = 1) {
		switch (verbosity)
		{
		case (-1):
			break;

		case 0:
			printf("TIME = \t%d\n", TIME);
			break;

		case 1:
			// REVIEW: print more info?
			printf("TIME = \t%d\n", TIME);
			break;
		
		default:
			break;
		}
	}

}; // end driver class

#endif
