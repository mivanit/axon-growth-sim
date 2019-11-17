#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

#include "consts.h"
#include "config.h"

#include "../diffusion/diffusion.h"
#include "../network/neuron.h"
#include "../axon/axon.h"

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
	//* actual data
	std::vector<Diffusion> dGrids;
	std::vector<Neuron> neurons;

	//* RNG stuff
	unsigned int RNG_seed;
	std::default_random_engine rng;
	std::normal_distribution<float> rdist_timeOn; // RNG for time neuron stays on

    const std::string NAME = "Test"; // FIXME

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
	Driver() {
		//* RNG setup
		RNG_seed = time(0);
		srand(RNG_seed);
		// TODO: get rid of this, inherit from chemtype
		rdist_timeOn = std::normal_distribution<float>(40.0, 10.0);

		//* dGrid setup
        //FIXME label
		dGrids = std::vector<Diffusion>(1, Diffusion(N_GRIDSIZE, 1, 1, 1, 1, "LABEL"));

		//* neuron setup
		gen_neurons();

		// TODO: print config and initial state to files/console
	}



/*
 ######   ######## ##    ##
##    ##  ##       ###   ##
##        ##       ####  ##
##   #### ######   ## ## ##
##    ##  ##       ##  ####
##    ##  ##       ##   ###
 ######   ######## ##    ##
*/

	// TODO: gen_dGrids() that inherits from chemtype classes

	void gen_neurons() {
		// create neurons randomly
		neurons.reserve(N_NEURONS);
		for (int i = 0; i < N_NEURONS; ++i)  {
			// generate random initial coordinates	
			// REVIEW: clustering of positions, range gen chem type, padding
			int x_init = rand() % N_GRIDSIZE;	
			int y_init = rand() % N_GRIDSIZE;

			// create new neuron
			Neuron new_neuron(i, 1, Coord(float(x_init), float(y_init)));

			// TODO: neuron chemtypes for this
			// model start time using uniform random distribution
			new_neuron.NT_start = rand() % N_STEPS;
			// model time neuron stays on using normal distribution
			new_neuron.NT_end = rdist_timeOn(rng) + new_neuron.NT_start;
			new_neuron.NT_amt = 1;

			// model amount to release randomly
			neurons.push_back(new_neuron);
		}
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
        for (auto & nrn : neurons) {
            nrn.update();
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
	void save_state() {
		// TODO: call `Diffusion::write_to_csv`, write and call similar functions for axons and neurons. need to figure out how it will be split into files.
	}
    
    void axon_write(const std::string& file) const {
        
    }

    void neuron_write(const std::string& file) const {
        
    }

    void diffusion_write(const std::string& file) const {
        std::ofstream ofs(file, std::ios_base::app);
        if (!ofs.is_open()) {
            std::cerr << "Error opening " << file << ", aborting."
                      << std::endl;
            return;
        }
        ofs.precision(PRECISION);
        
        for (Diffusion& d : dGrids) {
            ofs << d.label() << "\n";
            for (unsigned int i = 0; i < dim_; ++i) {
                for (unsigned int j = 0; j < dim_; ++j) {
                    ofs << std::to_string(u_[j][i]) << ",";
                }
                ofs << "\n";
            }
            ofs << std::endl;
        }
    }
}

#endif
