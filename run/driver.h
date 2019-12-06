#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../network/network.h"
#include "../network/neuron.h"
#include "../axon/axon.h"

#include "../util/err_util.h"

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
	std::default_random_engine rng;
	std::normal_distribution<float> rdist_timeOn; // RNG for time neuron stays on

	//* name 
	std::string NAME; 

	// array of chem type structs
	std::vector< chemType > CHEMTYPE_ARR;
	// array of cell type structs
	std::vector< cellType > CELLTYPE_ARR;

/*
 ######  ########  #######  ########
##    ##    ##    ##     ## ##     ##
##          ##    ##     ## ##     ##
##          ##    ##     ## ########
##          ##    ##     ## ##   ##
##    ##    ##    ##     ## ##    ##
 ######     ##     #######  ##     ##
*/

	// default ctor, inherits everything from config.h
	Driver(std::string name_in = "Test") {
		TIME = 0;

		//* RNG setup
		RNG_seed = time(0);
		srand(RNG_seed);

		//* name setup
		NAME = name_in;

		//* get arrays of chem, cell types
		CHEMTYPE_ARR = init_chemType_arr();
		CELLTYPE_ARR = init_cellType_arr();

		//* dGrid setup
		for (chemType x : CHEMTYPE_ARR)
		{
			dGrids.emplace_back(
				N_GRIDSIZE, // grid dim
				x.r_diff,
				DIFF_dx, 
				DIFF_dy, 
				DIFF_dt, 
				"LABEL"
			);
			dGrids.back().set_decay(x.r_decay);
		}

		//* neuron setup
		// CRIT: fix neuron/network ctors
		net = Network();
		net.gen_neurons();
		
		// CRIT: print config and initial state to files/console
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

	void run(size_t fin_step = N_STEPS, size_t save_every = 1)
	{
		while (TIME < fin_step) 
		{
			sim_step();
			if (TIME % save_every == 0)
			{
				save_state();
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
		const std::string DIR = "../data/" + NAME + "/raw/";
		// TODO: check for validity of path
		const std::string END = "_" + std::to_string(TIME) + ".tsv";
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
			<neuron.id>, <neuron.cellType>, <neuron.loc>
			[waveform arrays]
		*/
		for (const Neuron& neuron : net.neurons) {
			ofs << "==========\n";
			ofs << neuron.id << "\t"
				<< neuron.cellType << "\t"
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
			<axon.id>, <axon.cellType>, <axon.loc>, <axon.dir>
			[past_loc array]
			[postSyn_id array]
			[postSyn_wgt array]
		*/
		for (const Neuron& neuron : net.neurons) {
			const Axon& axon = neuron.axon;
			ofs << "==========\n";
			ofs << axon.id << "\t"
				<< axon.cellType << "\t"
				<< axon.loc().to_str() << "\t"
				<< axon.dir.to_str() << "\t"
				<< "\n";
			
			// locations
			for (const Coord& c : axon.past_loc) {
				ofs << c.to_str() << "\t";
			}
			ofs << "\n";

			// post stynaptic connection IDs
			for (uint16_t id : axon.postSyn_id) {
				ofs << std::to_string(id) << "\t";
			}
			ofs << "\n";

			// post stynaptic weights
			for (float wgt : axon.postSyn_wgt) {
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
		// CRIT: add grid ID
		for (const Diffusion& d : dGrids) {
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
}; // end driver class

#endif
