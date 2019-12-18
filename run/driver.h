#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "../util/coord.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../neuro/network.h"
#include "../neuro/neuron.h"
#include "../neuro/axon.h"

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

		Neuron::init_dGrid_ptr(&dGrids);
		Axon::init_dGrid_ptr(&dGrids);

		//* neuron setup
		/*
		std::vector<int> celltypes;
		celltypes.reserve(50);
		std::vector<Coord> input_coords;
		input_coords.reserve(50);

		for (int i = 0; i < 25; ++i) {
			celltypes.push_back(0);
			int x = (rand() % 40) + 5;
			int y = (rand() % 40) + 5;
			input_coords.push_back(Coord(x, y));
		}
		for (int i = 0; i < 25; ++i) {
			celltypes.push_back(1);
			int x = 100 - (rand() % 40) - 5;
			int y = 100 - (rand() % 40) - 5;
			input_coords.push_back(Coord(x, y));
		}
		net.gen_neurons(celltypes, input_coords); // use params defined above
		*/

		net.gen_neurons(); // random init
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
	void sim_step()
	{
		net.pass_activity();
		net.try_conn();

		// update all the neurons (which will in turn update axons)
		for (auto & nrn : net.neurons)
		{
			nrn.update();
		}

		TIME++;

		// update all grids
		int g_idx = 0;		
		for (auto & g : dGrids)
		{
			g.adi_step();
			
			// HARDCODED_RELEASE_TEMP(g_idx);
			g_idx++;
		}

	}


	void HARDCODED_RELEASE_TEMP(int g_idx, bool releaseNT = true)
	{
		auto g = dGrids[g_idx];
		if (releaseNT) {
			// grid release
			int chemType = CHEMTYPE_ARR[g_idx].chemType_ID;
			for (int i = N_GRIDSIZE / 10; i < N_GRIDSIZE; i += N_GRIDSIZE / 10) {
				for (int j = N_GRIDSIZE / 10; j < N_GRIDSIZE; j += N_GRIDSIZE / 10) {
					double concentration_to_add = i * j;
					Coord location(i, j);
					if (chemType == 1) {
						concentration_to_add = (N_GRIDSIZE - i) * (N_GRIDSIZE - j);
						location = Coord(i + 5, j + 5);
					}
					g.Crd_add(location, concentration_to_add / 100);
				}
			}
		}
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
		int verbosity = 0
	) {
		while (TIME < fin_step) 
		{
			// run step
			sim_step();
			// save if needed
			if (TIME % save_every == 0)
			{
				save_state(TIME == fin_step);
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
	void save_state(bool print_final = false) {
		// relies on build script to create this folder
		// const std::string DIR = "../data/" + NAME + "/raw/";
		const std::string DIR = "raw/";
		// TODO: check for validity of path
		std::ostringstream ss;
		ss << std::setw(5) << std::setfill('0') << std::to_string(TIME);
		const std::string END = "_" + ss.str() + ".tsv";

		// REVIEW: allow printing axon/neuron stuff at intermediate timesteps

		// UGLY: move connection matrix generation?

		// generate connection matrix
		net.generate_conn_matrix();

		if (print_final) {
			const std::string END = "_final.tsv";
			neuron_write(DIR + "neur" + END);
			axon_write(DIR + "axon" + END);
			conn_write(DIR + "conn" + END);
		}
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

			// // post stynaptic connection IDs
			// for (uint16_t id : nrn.axon.postSyn_id) {
			// 	ofs << std::to_string(id) << "\t";
			// }
			// ofs << "\n";

			// // post stynaptic weights
			// for (float wgt : nrn.axon.postSyn_wgt) {
			// 	ofs << std::to_string(wgt) << "\t";
			// }
			// ofs << "\n";
			
			// additional newline, flush stream
			ofs << std::endl;
		}
	}

	void conn_write(const std::string& file) const {
		// init ofstream
		std::ofstream ofs(file, std::ios_base::app);
		CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.");
		ofs.precision(PRECISION);
		
		// write connection matrix
		// format:
		/*
			[array]
		*/
		
		// generate connection matrix
		std::vector<std::vector<float>> mat = net.conn_matrix;
		
		// output array
		for (unsigned int i = 0; i < mat.size(); ++i) {
			for (unsigned int j = 0; j < mat.back().size(); ++j) {
				ofs << std::to_string(mat[i][j]) << "\t";
			}
			ofs << "\n";
		}
		// additional newline, flush stream
		ofs << std::endl;
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
	- `verbosity == -1`  :  dont print
	- `verbosity == 0`  :  every 50th timestep
	- `verbosity == 1`  :  timestep
	*/
	void print_info(int verbosity = 0) {
		switch (verbosity)
		{
		case (-1):
			break;

		case 0:
			printf("\tTIME = \t%d\n", (int)TIME);
			break;

		case 1:
			// REVIEW: print more info?
			printf("\tTIME = \t%d\n", (int)TIME);
			break;
		
		default:
			break;
		}
	}

}; // end driver class

#endif
