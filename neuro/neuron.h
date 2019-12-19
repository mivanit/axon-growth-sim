// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NEURON
#define NEURON

#include <iostream>
#include <string>

#include "../util/coord.h"
#include "axon.h"
#include "../diffusion/diffusion.h"
// #include "../waveform/waveform.h"

class Neuron
{
public:
	// static pointer to diffusion grid list
	static std::vector<Diffusion> * dGrids;

	// vars
	uint16_t id_neuron;
	uint16_t id_cellType;
	Coord loc;
	Axon axon;
	
	// aggregatve activity (super simple model)
	float avg_activity;	

	// REVIEW: waveforms, dendrites? both RV and references
	// waveform wf_in;
	// waveform wf_out;

	// initialize dGrid ref
	static void init_dGrid_ptr(std::vector<Diffusion> * inRef_dGrids)
	{
		dGrids = inRef_dGrids;
	}

	// ctor
	Neuron(uint16_t in_ID, uint16_t in_cellType, Coord in_coord)
		: id_neuron(in_ID), 
		id_cellType( in_cellType ), 
		loc(in_coord), 
		axon(in_ID, in_cellType, in_coord)
	{
		avg_activity = get_cellType().base_activ;
	}


	// update
	void update()
	{
		// update axons
		axon.update();

		// release NTs
		activ_release();

		// normalize activity to prevent large values
		norm_activ();
	}

	// get cellType class corresponding to ID
	inline cellType & get_cellType()
	{
		return CELLTYPE_ARR[id_cellType];
	}


	// normalize activity with absolute limit
	// UGLY: specify absolute limit in neuron type
	void norm_activ()
	{
		float MAX_ACTIV = 5.0;
		avg_activity = MAX_ACTIV * std::atan( avg_activity / MAX_ACTIV );
	}



	// add activity (called from a different neuron)
	void add_activ(float to_add_activ)
	{
		avg_activity += to_add_activ;
	}


	// get scaled activity
	// scaling by `activ_scale` from cellType
	const float get_activ_scaled()
	{
		return (avg_activity * get_cellType().activ_scale);
	}


	// activity based NT release into grid
	// UGLY: only linear NT release functions
	void activ_release()
	{
		// printf("\t\tactiv = %f\n", avg_activity);

		for (int g_idx = 0; g_idx < MAX_CHEMTYPE; g_idx++)
		{
			// get initial difference between target and actual activity
			float amt_rel = get_cellType().activ_tgt - avg_activity;

			// apply function, in this case linear function with min value of 0
			amt_rel *= get_cellType().activ_rel_coeff[g_idx];
			if (amt_rel < 0.0)
			{
				amt_rel = 0.0;
			}

			// add amount to the correct grid
			if ((*dGrids)[g_idx].Crd_valid(loc))
			{
				(*dGrids)[g_idx].Crd_add(loc, amt_rel);
			}
		}
	}	
};

std::vector<Diffusion> * Neuron::dGrids = nullptr;

#endif
