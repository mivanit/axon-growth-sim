// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// header for defining common variables and classes

#ifndef COORD
#define COORD

#include <cstdint>
#include <math.h>
#include <string>

struct Coord
{
	double x;
	double y;

	// default (0,0) ctor
	Coord() : x(0.0), y(0.0) {}

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

	// floored getters
	int bx() const
	{
		return floor(x);
	}

	int by() const
	{
		return floor(y);
	}

	// rounded getters
	int ix() const
	{
		return floor(x + 0.5);
	}

	int iy() const
	{
		return floor(y + 0.5);
	}

	/* get vector magnitude */
	double mag() const
	{
		return pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
	}

	// return coordinate pair as string
	std::string to_str() const
	{
		return ( "(" 
			+ std::to_string(x) + "," 
			+ std::to_string(y) + ")" );
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
		if (this->mag() <= 0.0000001) {
			this->x = 0.0;
			this->y = 0.0;
		}
		else {
			this->scale(1 / this->mag());
		}
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


inline Coord operator+(const Coord& a, const Coord& b) {
	Coord x(a);
	return Coord(x).add(b);
}


/*
 linearly interpolate the two given values a, b
 at a point proportionally x between them 
 ( positional distance between a,b is 1, we want value distance x from a and (1-x) from b )
*/
inline float lin_interp(float a, float b, float x)
{
	return (a + x * (b-a));
}


// REVIEW: deprecate search_vec for new search method
// TODO: make search_vec more extensible
const Coord search_vec[8] = {
	Coord(0, 1),			/* up */
	Coord(1, 1).norm(),		/* up right */
	Coord(1, 0),			/* right */
	Coord(1, -1).norm(),	/* down right */
	Coord(0, -1),			/* down */
	Coord(-1, -1).norm(),	/* down left */
	Coord(-1, 0),			/* left*/
	Coord(-1, 1).norm()		/* up left */
};


#endif