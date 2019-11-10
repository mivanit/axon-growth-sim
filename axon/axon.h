#ifndef AXON
#define AXON

#include <vector>
#include <random>

#include "../run/consts.h"
#include "../diffusion/diffusion.h"


// RNG stuff
// TODO: make all of this depend on config file chem types
// TODO: move this all to config file?
std::default_random_engine rng;
std::normal_distribution<float> rdist_dirNoise(0.0, 0.1); // RNG for noise on (normalized) direction vector
std::normal_distribution<float> rdist_move(1.5, 0.1); // RNG for distance travelled


class Axon
{
public:
	// static pointer to diffusion grid list
	static std::vector<Diffusion> * dGrids;

	// vars
	uint16_t id; // id of parent neuron
	uint8_t chemType; // chemType determines step size, etc etc
	Coord dir; // relative direction vector (NOT ABSOLUTE)
	std::vector<Coord> past_loc;
	// TODO: ids of post-synaptic neurons

	Coord loc()
	{
		return past_loc.back();
	}

	// ctor
	Axon(uint16_t in_ID, uint8_t in_chemType, Coord in_coord)
	: id(in_ID), chemType( in_chemType )
	{
		past_loc.push_back(in_coord);
	}
	
	// TODO: assignment/copy

	/*
	* main update function
	*/
	void update()
	{

	}

private:

	/*
	* knowing location and current direction,
	* find a new direction + noise term
	* save to direction vector
	* NOTE: this can be modified later if we choose to update gradient sensing
	*/
	void update_dir()
	{
		// find all points with positive dot product

		// test which grid square has highest concentration

		// add on noise term
		// rdist_dirNoise()

	}

	
	/*
	* knowing location and current direction
	* call update_dir(), and move along that new direction
	* with some noise
	*/
	void update_loc()
	{
		// update and copy direction vector
		update_dir();
		Coord move_new = dir;
		// multiply by some noise term
		move_new.scale(rdist_move(rng));
		// add original
		move_new.add(past_loc.back());

		past_loc.push_back(move_new);
	}

};

std::vector<Diffusion> * Axon::dGrids = nullptr;

#endif