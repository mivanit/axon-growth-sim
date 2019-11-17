// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NETWORK
#define NETWORK

#include <string>
#include <fstream>

#include "../util/csv_parser.h"
#include "../util/math_util.h"
#include "../util/err_util.h"
#include <dirent.h>
#include "neuron.h"
#include "synapse.h"

class Network
{
public:
	// data
	std::vector<neuron> neurons;

	// ctors
	network(std::string file_net, std::string file_topo, std::string str_dir_stim)
	{
		// reserve space
		neurons.reserve(N_NEURONS);

		// * read neurons + coords
		{
			io::CSVReader<4> csv_net(file_net);	
			csv_net.read_header(io::ignore_extra_column, "ID", "coord_x", "coord_y", "chemType");
			uint16_t ID; uint8_t chemType; uint16_t coord_x; uint16_t coord_y;
			while(csv_net.read_row(ID, chemType, coord_x, coord_y))
			{
				neurons.emplace_back(ID, chemType, coord_x, coord_y);
			}
		}

		// * read connection data
		{
			io::CSVReader<4> csv_topo(file_topo);	
			csv_topo.read_header(io::ignore_extra_column, "pre", "post", "alpha", "delay");
			uint16_t id_pre; uint16_t id_post; float alpha; float delay;
			while(csv_topo.read_row(id_pre, id_post, alpha, delay))
			{
				neurons[id_pre].axon.emplace_back(id_post, alpha, delay);
			}
		}


		// * read stim data
		{
			DIR *dir_stim = NULL;
			struct dirent *entry = NULL;
			dir_stim = opendir(str_dir_stim.c_str());

			// read firs two entries, "." and ".."
			readdir(dir_stim); readdir(dir_stim);
			
			// read all files
			while (entry = readdir(dir_stim))
			{
				// get file name with neuron ID
				CHK_ERROR_FATAL(entry == nullptr, "read file entry was nullptr");
				std::string str = entry->d_name;
				std::cout << str << "\t";
				str.erase(str.end() - std::strlen(CSTR_STIMEXT), str.end());
				str.erase(str.begin(), str.begin() + std::strlen(CSTR_STIMNAME));
				uint16_t neuron_id = std::stoi(str);
				std::cout << str << std::endl;
				
				// read waveform, set
				neurons[neuron_id].wf_in = wf_read(entry->d_name);
			}
			closedir(dir_stim);
		}
	}

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


};




#endif
