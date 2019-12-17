#ifndef AXON
#define AXON

#include <vector>
#include <utility>
#include <math.h>
#include <numeric>      // std::iota
#include <algorithm>    // std::sort

#include "../diffusion/diffusion.h"
#include "../util/coord.h"
#include "../util/rng_def.h"


class Axon
{
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
	// vars
	
	// id of parent neuron
	uint16_t id_neuron; 
	// cellType determines step size, etc etc
	uint16_t id_cellType; 
	// relative direction vector (NOT ABSOLUTE)
	Coord dir; 
	// vector of past locations
	std::vector<Coord> past_loc;
	// vector of id's of neurons we are connected to
	std::vector<uint16_t> postSyn_id;
	
	// vector of weights to the id's we are connected to
	// `postSyn_wgt[i]` will contain weight for connection to neuron id `postSyn_id[i]`
	std::vector<float> postSyn_wgt;
	
	// keeping track of when stopped
	bool bln_stopped = false;
	// TODO: keep track of time when axon growth started/stopped?

	// ref to vector of diffusion grids
	static std::vector<Diffusion> * dGrids;


/*
	########  ##     ## ########  ##       ####  ######
	##     ## ##     ## ##     ## ##        ##  ##    ##
	##     ## ##     ## ##     ## ##        ##  ##
	########  ##     ## ########  ##        ##  ##
	##        ##     ## ##     ## ##        ##  ##
	##        ##     ## ##     ## ##        ##  ##    ##
	##         #######  ########  ######## ####  ######
*/

	// std ctor
	Axon(uint16_t in_ID, uint16_t in_cellType, Coord in_coord)
		: id_neuron(in_ID), id_cellType(in_cellType)
	{
		past_loc.push_back(in_coord);
		dir = Coord(1, 1);
	}


	// initialize dGrid ref
	static void init_dGrid_ptr(std::vector<Diffusion> * inRef_dGrids)
	{
		dGrids = inRef_dGrids;
	}


	Coord loc() const
	{
		return past_loc.back();
	}
	
	// implicit copy & assignment are good enough
	

	/*
	* main update (on timestep) function
	*/
	// REVIEW: move multiple times per diffusion timestep?
	void update()
	{
		if (!bln_stopped)
		{
			update_dir();
			move(dir);
		}
	}

	// get cellType class corresponding to ID
	inline cellType & get_cellType()
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

	
	// make connection to a given neuron
	// UGLY: always makes connections weight 1 for now
	void make_conn(int nrn_id)
	{
		postSyn_id.push_back(nrn_id);
		postSyn_wgt.push_back(1.0);
	}

private:

/*
   ########  #### ########
   ##     ##  ##  ##     ##
   ##     ##  ##  ##     ##
   ##     ##  ##  ########
   ##     ##  ##  ##   ##
   ##     ##  ##  ##    ##
   ########  #### ##     ##
*/

	/*
	
	*/
	void update_dir()
	{
		// vector of sensed directions
		std::vector<Coord> sensed_dir(MAX_CHEMTYPE, Coord(0,0));
		// UGLY: total sensed graident
		float total_sensed = 0.0;

		// create vector of new direction
		Coord new_dir = Coord(0, 0);

		// for each diffusion chemtype (where affinity is nonzero)
		for ( int g_idx =0; g_idx < MAX_CHEMTYPE; g_idx++ )
		{
			if ( abs(get_cellType().chemType_affinities[g_idx]) > EPSILON )
			{
				std::pair<Coord,float> mypair = sense(g_idx);
				// store sensed direction
				sensed_dir[g_idx] = mypair.first;
				total_sensed += mypair.second;
				
				// weighted sum of components
				new_dir = new_dir 
					+ sensed_dir[g_idx].scale( 
						get_cellType().chemType_affinities[g_idx] 
					);
			}
		}

		// REVIEW: check for no sensing, kill with probability 1/2
		// if (
		// 	(total_sensed < EPSILON)
		// 	&& (rand() % 20 == 0)	
		// ) {
		// 	// bln_stopped = true;
		// }

		// normalize
		dir.norm();
	}



/*
    ######  ######## ##    ##  ######  ########
   ##    ## ##       ###   ## ##    ## ##
   ##       ##       ####  ## ##       ##
    ######  ######   ## ## ##  ######  ######
         ## ##       ##  ####       ## ##
   ##    ## ##       ##   ### ##    ## ##
    ######  ######## ##    ##  ######  ########
*/

	// two pronged sensing
	// returns pair of (direction coord, gradient strength) for the given grid
	std::pair<Coord,float> sense(int g_idx)
	{
		// get orth to current dir
		Coord prong_L = Coord(dir);
		Coord prong_R = Coord(dir);
		prong_L.orth(true);
		prong_R.orth(false);
		
		// get prong points, given that
		// p[] * v[] = tau
		prong_L = Crd_scale_mult(prong_L, get_cellType().searchAngle_lambda) + dir;
		prong_R = Crd_scale_mult(prong_R, get_cellType().searchAngle_lambda) + dir;
		prong_L.norm();
		prong_R.norm();
		prong_L.scale(get_cellType().searchDist);
		prong_R.scale(get_cellType().searchDist);
		
		// sense at two points
		float c_L = (*dGrids)[g_idx].getC_bilin( loc() + prong_L );
		float c_R = (*dGrids)[g_idx].getC_bilin( loc() + prong_R );
		
		// add noise
		// standard normal distribution around 0 times noise for this chemtype sensing
		c_L += ndist_STD(rng) * (get_cellType().senseNoise_sigma[g_idx]);
		c_R += ndist_STD(rng) * (get_cellType().senseNoise_sigma[g_idx]);
				
		// weighted sum of prong vectors, normalized
		Coord optimal_dir = Crd_scale_mult(prong_L, c_L) + Crd_scale_mult(prong_R, c_R);
		optimal_dir.norm();

		return std::make_pair(optimal_dir, abs(c_L - c_R) );
	}





/*
   ##     ##  #######  ##     ## ########
   ###   ### ##     ## ##     ## ##
   #### #### ##     ## ##     ## ##
   ## ### ## ##     ## ##     ## ######
   ##     ## ##     ##  ##   ##  ##
   ##     ## ##     ##   ## ##   ##
   ##     ##  #######     ###    ########
*/	
	/*
	knowing location and new direction
	move along that new direction with some noise
	*/
	void move(Coord move_new)
	{
		// scale by (randomized) step size
		move_new.scale( get_cellType().stepSize_mu + get_cellType().stepSize_sigma * ndist_STD(rng) );
		
		// find new location, push onto vector
		move_new.add(past_loc.back());
		past_loc.push_back(move_new);

		// check for no sensing, kill with probability 1/2
		if ( !(*dGrids).back().Crd_valid(loc()) ) {
			bln_stopped = true;
		}
	}




/*
       #######  ##       ########
      ##     ## ##       ##     ##
      ##     ## ##       ##     ##
      ##     ## ##       ##     ##
      ##     ## ##       ##     ##
      ##     ## ##       ##     ##
       #######  ######## ########
*/
		// returns pair of (direction coord, max sensed concentration) for the given grid
	std::pair<Coord,float> sense_grid( int g_idx, std::vector<double> & dot_products )
	{	
		// test which grid square has highest concentration
		Coord optimal_dir(1, 0);
		double highest_concentration = 0.0;

		for (int i = 0; i < 8; ++i)
		{
			if (dot_products[i] > get_cellType().searchAngle_tau)
			{
				double concentration = (*dGrids)[g_idx].Crd_getC(past_loc.back() + search_vec[i]);
				if (concentration > highest_concentration) {
					highest_concentration = concentration;
					optimal_dir = Coord(search_vec[i]);
				}
			}
		}


		// add noise term to normalized direction and set as new direction
		// multiply by noise for that type
		Coord noise_vec = Coord(ndist_STD(rng),ndist_STD(rng)).scale(
		 		get_cellType().senseNoise_sigma[g_idx]
		 	);
		optimal_dir = optimal_dir; // + noise_vec;

		optimal_dir.norm();

		return std::make_pair(optimal_dir,highest_concentration);
	}


	/*
	knowing location and current direction, find a new direction + noise term
	save to direction vector
	NOTE: this can be modified later if we choose to update gradient sensing
	*/
	void update_dir_old()
	{	
		// use dot products to determine closest points to direction axon is facing
		std::vector<double> dot_products(8);
		for (int i = 0; i < 8; ++i) {
			Coord chk_dir = search_vec[i];
			dot_products[i] = chk_dir[0]*dir[0] + chk_dir[1]*dir[1];
		}

		// vector of sensed directions
		std::vector<Coord> sensed_dir(MAX_CHEMTYPE, Coord());
		// UGLY: total sensed chems
		float total_sensed = 0.0;

		// create new dir vector
		Coord new_dir = Coord(0, 0);

		// for each diffusion chemtype (where affinity != 0)
		for ( int g_idx =0; g_idx < MAX_CHEMTYPE; g_idx++ )
		{
			if ( abs(get_cellType().chemType_affinities[g_idx]) > EPSILON )
			{
				std::pair<Coord,float> mypair = sense_grid(g_idx, dot_products);
				// store sensed direction
				sensed_dir[g_idx] = mypair.first;
				total_sensed += mypair.second;
				
				// weighted sum of components
				new_dir = new_dir 
					+ sensed_dir[g_idx].scale( 
						get_cellType().chemType_affinities[g_idx] 
					);
			}
			else
			{
				sensed_dir[g_idx] = Coord(0,0);
			}
		}

		// check for no sensing, kill with probability 1/2
		if (
			(total_sensed < EPSILON)
			&& (rand() % 20 == 0)	
		) {
			// bln_stopped = true;
		}

		// CRIT: check for far out of bands, kill


		// based on total detected NT, weigh the old direction and the new
		// dir = dir + new_dir.scale(total_sensed);
		dir = new_dir;

		// normalize
		dir.norm();
	}

}; // end axon class




std::vector<Diffusion> * Axon::dGrids = nullptr;


#endif