#ifndef AXON
#define AXON

#include <vector>
#include <math.h>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort

#include "../run/consts.h"
#include "../diffusion/diffusion.h"
#include "../util/rng_def.h"


class Axon
{
public:
	// vars
	uint16_t id_neuron; // id of parent neuron
	uint16_t id_cellType; // cellType determines step size, etc etc
	Coord dir; // relative direction vector (NOT ABSOLUTE)
	std::vector<Coord> past_loc;
	std::vector<uint16_t> postSyn_id;
	std::vector<float> postSyn_wgt;
	
	// ref to vector of diffusion grids
	static std::vector<Diffusion> * dGrids;

	// initialize dGrid ref
	static void init_dGrid_ptr(std::vector<Diffusion> * inRef_dGrids)
	{
		dGrids = inRef_dGrids;
	}


	Coord loc() const
	{
		return past_loc.back();
	}

	// std ctor
	Axon(uint16_t in_ID, uint16_t in_cellType, Coord in_coord)
		: id_neuron(in_ID), id_cellType(in_cellType)
	{
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

	// get cellType class corresponding to ID
	cellType & get_cellType()
	{
		return CELLTYPE_ARR[id_cellType];
	}

	// get the weight of connection to a given neuron id
	// return 0.0 if no connection
	float get_weight_to(int idx_nrn)
	{
		std::vector<uint16_t>::iterator it = std::find(postSyn_id.begin(), postSyn_id.end(), idx_nrn);
		int idx_inArr = std::distance(postSyn_id.begin(), it);
		return postSyn_wgt[idx_inArr];
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

		// reset dir vector
		dir = Coord(0, 0);

		// for each diffusion chemtype (where affinity != 0)
		for ( int g_idx =0; g_idx < MAX_CHEMTYPE; g_idx++ )
		{
			if ( abs(get_cellType().chemType_affinities[g_idx]) > EPSILON )
			{
				// store sensed direction
				sensed_dir[g_idx] = sense_grid(g_idx, dot_products);
				
				// weighted sum of components
				dir = dir 
					+ sensed_dir[g_idx].scale( 
						get_cellType().chemType_affinities[g_idx] 
					);
			}
			else
			{
				sensed_dir[g_idx] = Coord(0,0);
			}
		}

		// normalize
		dir.norm();
	}


	Coord sense_grid( int g_idx, std::vector<double> & dot_products )
	{	
		// test which grid square has highest concentration
		Coord optimal_dir(0, 0);
		double highest_concentration = 0.0;
		for (int i = 0; i < 8; ++i)
		{
			if (dot_products[i] > get_cellType().searchAngle_tau)
			{
				double concentration = (*dGrids)[g_idx].Crd_getC(past_loc.back() + search_vec[i]);
				if (concentration > highest_concentration) {
					highest_concentration = concentration;
					optimal_dir = search_vec[i];
				}
			}
		}

		// add noise term to normalized direction and set as new direction
		// multiply by noise for that type
		optimal_dir = optimal_dir 
			+ Coord(ndist_STD(rng),ndist_STD(rng)).scale(
				get_cellType().senseNoise_sigma[g_idx]
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
		move_new.scale( get_cellType().rdist_move(rng) );
		// add original
		move_new.add(past_loc.back());

		past_loc.push_back(move_new);
	}

};

std::vector<Diffusion> * Axon::dGrids = nullptr;


#endif