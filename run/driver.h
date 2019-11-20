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
    // TODO: ambient concentrations, baseline stimulation

	//* actual data
	std::vector<Diffusion> dGrids;
	std::vector<Neuron> neurons;

	//* RNG stuff
	unsigned int RNG_seed;
	std::default_random_engine rng;
    std::normal_distribution<float> rdist_timeOn; // RNG for time neuron stays on

    // FIXME: name of driver class?
    const std::string NAME = "Test"; 

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

		//* dGrid setup
        // FIXME: label and correct init for dGrids
		dGrids = std::vector<Diffusion>(1, Diffusion(N_GRIDSIZE, 1, 1, 1, 1, "LABEL"));

		//* neuron setup
		// CRIT: get rid of this, inherit from chemtype
		rdist_timeOn = std::normal_distribution<float>(40.0, 10.0);
		gen_neurons();

		// TODO: print config and initial state to files/console
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
    // Write the state of the network at time step t
	void save_state(unsigned int t) {
        const std::string DIR = "../data/" + NAME + "/";
        const std::string END = "_" + std::to_string(t) + ".csv";
        axon_write(DIR + "axon" + END);
        neuron_write(DIR + "neuron" + END);
        diffusion_write(DIR + "diffusion" + END);
	}
    
    void axon_write(const std::string& file) const {
        // init ofstream
        std::ofstream ofs(file, std::ios_base::app);
        CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.")
        ofs.precision(PRECISION);
        
        // TODO: `axon_write` function
    }

    void neuron_write(const std::string& file) const {
        // init ofstream
        std::ofstream ofs(file, std::ios_base::app);
        CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.")
        ofs.precision(PRECISION);
        
        // TODO: `neuron_write` function
    }

    void diffusion_write(const std::string& file) const {
        // init ofstream
        std::ofstream ofs(file, std::ios_base::app);
        CHK_ERROR(!ofs.is_open(), "Error opening " + file + ", aborting.")
        ofs.precision(PRECISION);
        
        // write diffusion grids
        // UGLY: specify format in comments
        for (const Diffusion& d : dGrids) {
            ofs << d.label() << "\n";
            for (unsigned int i = 0; i < d.dim(); ++i) {
                for (unsigned int j = 0; j < d.dim(); ++j) {
                    ofs << std::to_string(d.concentration(j, i)) << ",";
                }
                ofs << "\n";
            }
            ofs << std::endl;
        }
    }
}

#endif
