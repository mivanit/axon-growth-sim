#ifndef RNG_HEADER
#define RNG_HEADER

#include <random>

// RNG stuff

std::default_random_engine rng;

// standard normal distribution with mean of 0 and sigma of 1
std::normal_distribution<float> ndist_STD(0.0, 1.0);

#endif