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

// ctor
Neuron(uint16_t in_ID, uint8_t in_chemType, Coord in_coord)
	: id(in_ID), chemType( in_chemType ), loc(in_coord) {}

};

#endif
