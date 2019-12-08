// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NEURON
#define NEURON

#include <iostream>
#include <string>

#include "../run/consts.h"
#include "../axon/axon.h"
#include "../diffusion/diffusion.h"
// #include "../waveform/waveform.h"

class Neuron
{
public:
	// static pointer to diffusion grid list
	static std::vector<Diffusion> & dGrids;

	// vars
	uint16_t id_neuron;
	uint16_t id_cellType;
	Coord loc;
	Axon axon;
	
	// UGLY: NT release vars
	size_t NT_start;
	size_t NT_end;
	double NT_amt;

	// REVIEW: waveforms, dendrites? both RV and references
	// waveform wf_in;
	// waveform wf_out;

	// ctor
	Neuron(uint16_t in_ID, uint16_t in_cellType, Coord in_coord)
		: id_neuron(in_ID), 
		id_cellType( in_cellType ), 
		loc(in_coord), 
		axon(in_ID, in_cellType, in_coord) 
	{}

	// update
	void update()
	{
		// REVIEW: update waveforms

		// CRIT: hardcoded NT release
		// if ((TIME > NT_start) && (TIME < NT_end))
		// {
		// 	(*dGrids)[0].Crd_add(loc, NT_amt);
		// }

		// update axons
		axon.update();
	}

	// get cellType class corresponding to ID
	cellType & get_cellType()
	{
		return CELLTYPE_ARR[id_cellType];
	}

};

// std::vector<Diffusion> * Neuron::dGrids = nullptr;

#endif
