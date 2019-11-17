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
// #include "../waveform/waveform_D.h"


class Neuron
{
public:
	// static pointer to diffusion grid list
	static std::vector<Diffusion> * dGrids;

	// vars
	uint16_t id;
	uint8_t cellType;
	Coord loc;
	Axon axon;
	
	// UGLY: NT release vars
	size_t NT_start;
	size_t NT_end;
	double NT_amt;

	// TODO: waveforms
	// waveform wf_in;
	// waveform wf_out;
	// TODO: dendrites? both RV and references

	// ctor
	Neuron(uint16_t in_ID, uint8_t in_cellType, Coord in_coord)
		: id(in_ID), cellType( in_cellType ), loc(in_coord) {
		axon = Axon(in_ID, in_cellType, in_coord);
	}

	// update
	void update()
	{
		// TODO: update waveforms

		// UGLY: hardcoded NT release
		if ((TIME > NT_start) && (TIME < NT_end))
		{
			(*dGrids)[0].Crd_add(loc, NT_amt);
		}

		// update axons
		axon.update();
	}

};

std::vector<Diffusion> * Neuron::dGrids = nullptr;

#endif
