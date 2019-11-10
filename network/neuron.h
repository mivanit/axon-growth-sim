// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#ifndef NEURON
#define NEURON

#include <iostream>
#include <string>

#include "../waveform/waveform_D.h"
#include "../diffusion/diffusion.h"
#include "../run/consts.h"
#include "synapse.h"

class Neuron
{
public:

// vars
uint16_t id;
uint8_t chemType;
Coord loc;
waveform wf_in;
waveform wf_out;
std::vector < synapse > axon;

neuron(uint16_t in_ID, uint8_t in_chemType, uint16_t in_coord_x, uint16_t in_coord_y)
	: id(in_ID), chemType( in_chemType )
{
	loc = std::make_pair(in_coord_x, in_coord_y);
}


};

#endif
