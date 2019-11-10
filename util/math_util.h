// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

// * useful math functions

#ifndef MATH_UTIL
#define MATH_UTIL

#include <cstdint>

#include "err_util.h"

// below function copied from
// https://stackoverflow.com/questions/12826114/euclids-extended-algorithm-c
int xEuclid_old(int a, int b, int &x, int &y)
{
	if (a == 0)
	{
		x = 1;
		y = 0;
		return 0;
	}

	int x1, y1;
	int gcd = xEuclid_old(b, a % b, x1, y1);
	x = y1;
	y = x1 - (a / b) * y1;
	return gcd;
}


// https://www.sanfoundry.com/cpp-program-implement-extended-eucledian-algorithm/
int xEuclid(int a, int b, int &xLast, int &yLast)
{
	TEST("F00");
    int x = 1, y = 0;
    xLast = 0;
	yLast = 1;
    int q, r, m, n;
    while (a != 0) 
    {
        q = b / a;
        r = b % a;
        m = xLast - q * x;
        n = yLast - q * y;
        xLast = x; 
        yLast = y;
        x = m; 
        y = n;
        b = a; 
        a = r;
    }
    // xLast, yLast have results
	// return gcd
	TEST("F90");
	return b;
}


// below functions copied from 
// https://stackoverflow.com/questions/4229870/c-algorithm-to-calculate-least-common-multiple-for-multiple-numbers#4229930

int GCD(int a, int b)
{
	for (;;)
	{
		if (a == 0) return b;
		b %= a;
		if (b == 0) return a;
		a %= b;
	}
}

int LCM(int a, int b)
{
	int temp = GCD(a, b);

	return temp ? (a / temp * b) : 0;
}

#endif