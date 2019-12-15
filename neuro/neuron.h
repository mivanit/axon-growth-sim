// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NEURON
#define NEURON

#include <iostream>
#include <string>

#include "../run/consts.h"
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
		avg_activity = CELLTYPE_ARR[id_cellType].base_activ;
	}


	// update
	void update()
	{
		// update axons
		axon.update();

		// REVIEW: reset activity
		// avg_activity = CELLTYPE_ARR[id_cellType].base_activ;
	}

	// get cellType class corresponding to ID
	cellType & get_cellType()
	{
		return CELLTYPE_ARR[id_cellType];
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
		return (avg_activity * CELLTYPE_ARR[id_cellType].activ_scale);
	}


	// activity based NT release into grid
	// UGLY: only works for two NTs, also linear at the moment
	void activ_release()
	{
		float tgt_activ = 1.0;

		// for excitatory
		float amt_excite = 0.0;
		if (avg_activity < tgt_activ)
		{
			amt_excite = tgt_activ - avg_activity;
		}
		(*dGrids)[0].Crd_add(loc, amt_excite);

		// for inhibitory
		float amt_inh = 0.0;
		if (avg_activity > tgt_activ)
		{
			amt_excite = avg_activity - tgt_activ;
		}
		(*dGrids)[1].Crd_add(loc, amt_inh);
	}

};

std::vector<Diffusion> * Neuron::dGrids = nullptr;

#endif
