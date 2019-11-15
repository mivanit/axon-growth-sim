// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// header for defining common variables and classes

#ifndef CONSTS
#define CONSTS

#include <cstdint>
#include <math.h>

#include "config_ref.h"

struct Coord
{
	double x;
	double y;

	// raw ctor
	Coord(float in_x, float in_y)
		: x(in_x), y(in_y) {}

	// copy ctor
	Coord(const Coord& rhs) 
		: x(rhs.x), y(rhs.y) {}

	double operator [](int i) 
	{
		if (i == 0) return x;
		else return y;
	}

	// rounded getters
	int ix()
	{
		return floor(x + 0.5);
	}

	int iy()
	{
		return floor(y + 0.5);
	}

	/* get vector magnitude */
	double mag()
	{
		return pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
	}

	/* scale vector by some factor */
	Coord scale(float scaling_factor)
	{
		x *= scaling_factor;
		y *= scaling_factor;
		return *this;
	}

	/* normalize vector */
	Coord norm()
	{
		this->scale(1 / this->mag);
		return *this;
	}

	/* add a given vector to this vector */
	Coord add(Coord to_add)
	{
		x += to_add.x;
		y += to_add.y;
		return *this;
	}
};

Coord operator+(const Coord& a, const Coord& b) {
	return Coord(a).add(b);
}


// code for comparing two floats
inline bool comp_f(float a, float b)
{
    return fabs(a - b) <= EPSILON;
}

inline bool zero_f(float a)
{
	return fabs(a) <= EPSILON;
}


// * chem type classes and instances

struct Axon_chemType
{
	public:
	float stepSize;
	// TODO: noise term
	// TODO: neurotrophin attraction
	// TODO: turning rate

};



// misc
int round(float f)
{
	return floor(f + 0.5);
}



#endif