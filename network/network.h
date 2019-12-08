// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NETWORK
#define NETWORK

#include <random>

#include "../run/consts.h"
#include "../run/config.h"

#include "neuron.h"
#include "../util/rng_def.h"

// std::vector< cellType > create_empty_celltype_vec() 
// { return std::vector< cellType >(); }


class Network {
public:
	
	// neurons vector
	std::vector<Neuron> neurons;

	// connectivity matrix
	// `conn_matrix[i][j]` holds the weight of the connection from neuron i --> j
	std::vector<std::vector<float>> conn_matrix;

	// ctors
	Network(bool gen = false)  {
		if (gen) {
			gen_neurons();
		}
	}

	// TODO: default assign operator sufficient?
	Network operator=(const Network &) {}



	Neuron & get_neuron_ref(int idx) {
		return neurons[idx];
	}

	
	// pass along activity, update every neuron individually
	void update()
	{
		// first pass: forward activity
		for ( Neuron n : neurons )
		{
			// for every connection
			for ( int i : n.axon.postSyn_id )
			{
				// pass along activity after multiplying by scaling factor
				neurons[i].add_activ(n.get_activ_scaled());
			}
		}

		// second pass: update each neuron (and by extension, axons)
		for (Neuron n : neurons) {
			n.update();
		}
	}

	// generates a set of neurons with uniformly random coordinates
	void gen_neurons() {
		// create neurons randomly
		neurons.clear();
		neurons.reserve(N_NEURONS);
		for (int i = 0; i < N_NEURONS; ++i) {
			// generate random initial coordinates	
			// REVIEW: clustering of positions, range gen chem type, padding
			
			// create new neuron
			Neuron new_neuron(
				i, // neuron ID
				(rand() % MAX_CELLTYPE), // cellType
				Coord( //coordinate
					float(rand() % N_GRIDSIZE), 
					float(rand() % N_GRIDSIZE)
				)
			); 

			neurons.push_back(new_neuron);
		}
	}

	/*
	test each axon if it is close enough 
	to form a connection to any neuron
	REVIEW: this is not super realistic
	*/
	void try_conn() {
		// CRIT: proximity testing, synapse formation
	
		// loop over all axons

		// try distance to every neuron

		// if distance less than threshold, make connections

		// OPTIMIZE: splitting into subgrids for faster detection

		const double DIST_THRESHOLD = 2.0;
		const double CONN_PROB = 0.8;

		for (unsigned int i = 0; i < N_NEURONS; ++i) {
			Axon& axon = neurons[i].axon;
			if (axon.bln_stopped) {
				continue;
			}
			Coord al = axon.loc();
			for (unsigned int j = 0; j < N_NEURONS; ++j) {
				if (i != j) {
					Coord nl = neurons[j].loc;
					double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
					if (std::sqrt(pow(nl.x - al.x, 2) + pow(nl.y - al.y, 2)) <=
						DIST_THRESHOLD && r < CONN_PROB) {
						axon.make_conn(neurons[j].id_neuron);
					}
				}			

			}
		}

	}


	// ask the axons for their data and generate a connectivity matrix
	// `conn_matrix[i][j]` holds the weight of the connection from neuron i --> j
	// OPTIMIZE: do we really need to clear it??
	void generate_conn_matrix() {
		conn_matrix.clear();
		conn_matrix.reserve(neurons.size());
		for (int i = 0; i < neurons.size(); ++i) {
			for (int j = 0; j < neurons.size(); ++j) {
				float weight = neurons[i].axon.get_weight_to(j);
				conn_matrix[i][j] = weight;
			}
		}
	}
};


#endif
