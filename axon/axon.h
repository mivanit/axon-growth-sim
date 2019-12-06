#ifndef AXON
#define AXON

#include <vector>
#include <math.h>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort

#include "../run/consts.h"
#include "../diffusion/diffusion.h"




class Axon
{
public:
	// vars
	uint16_t id_neuron; // id of parent neuron
	cellType * id_celltype; // cellType determines step size, etc etc
	Coord dir; // relative direction vector (NOT ABSOLUTE)
	std::vector<Coord> past_loc;
	std::vector<uint16_t> postSyn_id;
	std::vector<float> postSyn_wgt;
	
	// pointer to vector of diffusion grids
	static std::vector<Diffusion> * dGrids;


	Coord loc() const
	{
		return past_loc.back();
	}

	Axon() : id_neuron(-1) {}

	// std ctor
	Axon(uint16_t in_ID, cellType * in_cellType, Coord in_coord)
		: id_neuron(in_ID)
	{
		id_celltype = in_cellType;
		past_loc.push_back(in_coord);
	}
	
	// implicit copy & assignment are good enough
	

	/*
	* main update (on timestep) function
	*/
	void update()
	{
		update_dir();
		// REVIEW: move multiple times per diffusion timestep?
		move(dir);
		// CRIT: stop movement and write to synapse when close to neuron
	}

private:

	/*
	* knowing location and current direction, find a new direction + noise term
	* save to direction vector
	* NOTE: this can be modified later if we choose to update gradient sensing
	*/
	void update_dir()
	{
		// use dot products to determine closest points to direction axon is facing
		std::vector<double> dot_products(8);
		for (int i = 0; i < 8; ++i) {
			Coord chk_dir = search_vec[i];
			dot_products[i] = chk_dir[0]*dir[0] + chk_dir[1]*dir[1];
		}

		// vector of sensed directions
		std::vector<Coord> sensed_dir(MAX_CHEMTYPE, Coord());


		// REVIEW: combine these two loops for sensing and weighted sum?

		// for each diffusion chemtype (where affinity != 0)
		for ( int g_idx =0; g_idx < MAX_CHEMTYPE; g_idx++ )
		{
			if ( abs(id_celltype->chemType_affinities[g_idx]) > EPSILON )
			{
				// store sensed direction
				sensed_dir[g_idx] = sense_grid(g_idx, dot_products);
			}
			else
			{
				sensed_dir[g_idx] = Coord(0,0);
			}
		}
		
		// reset dir vector, weighted sum of components
		dir = Coord(0, 0);
		for ( int g_idx =0; g_idx < MAX_CHEMTYPE; g_idx++ )
		{
			dir = dir 
				+ sensed_dir[g_idx].scale( 
					id_celltype->chemType_affinities[g_idx] 
				);
		}

		// normalize
		dir.norm();
	}


	Coord sense_grid( int g_idx, std::vector<double> & dot_products )
	{
		// get grid
		Diffusion& d = (*dGrids)[g_idx];
	
		// test which grid square has highest concentration
		Coord optimal_dir(0, 0);
		double highest_concentration = 0.0;
		for (int i = 0; i < 8; ++i)
		{
			if (dot_products[i] > id_celltype->searchAngle_tau)
			{
				double concentration = d.Crd_getC(past_loc.back() + search_vec[i]);
				if (concentration > highest_concentration) {
					highest_concentration = concentration;
					optimal_dir = search_vec[i];
				}
			}
		}

		// add noise term to normalized direction and set as new direction
		// multiply by noise for that type
		optimal_dir = optimal_dir + Coord(
			ndist_STD(rng) * id_celltype->senseNoise_sigma[g_idx],
			ndist_STD(rng) * id_celltype->senseNoise_sigma[g_idx]
		);

		optimal_dir.norm();

		return optimal_dir;
	}

	
	/*
	* knowing location and new direction
	* move along that new direction with some noise
	*/
	void move(Coord move_new)
	{
		// multiply by some noise term
		move_new.scale(rdist_move(rng));
		// add original
		move_new.add(past_loc.back());

		past_loc.push_back(move_new);
	}

};

std::vector<Diffusion> * Axon::dGrids = nullptr;


#endif