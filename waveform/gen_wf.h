// waveform generator

// * DISCRETE WAVEFORM GENERATOR

#ifndef WF_GEN
#define WF_GEN

#include <random>
#include <cstdint>
#include <vector>
#include <chrono>

#include "waveform_D.h"

// * random generators definitions
std::default_random_engine re{(unsigned int) std::chrono::system_clock::now().time_since_epoch().count()};

// std::binomial_distribution<int> rnd_count(0, 5);
std::uniform_int_distribution<int> rnd_count{10, 20};

std::uniform_real_distribution<float> rnd_phase{0.0, 250.0};
std::uniform_real_distribution<float> rnd_period{5.0, 250.0};
std::normal_distribution<float> rnd_amp{4.0, 5.0}; // (mu, sigma)
std::binomial_distribution<int> rnd_time{0, 10};

// get a random wf
waveform gen_rand_wf()
{
	waveform wf_out;

	uint8_t num_comps = (uint8_t) ( abs(rnd_count(re)) );

	for (uint8_t i = 0; i < num_comps; i++)
	{
		wf_out.emplace_back( rnd_phase(re), rnd_period(re), fabs(rnd_amp(re)), 0, abs(rnd_time(re)) + 1 );
		// ~ wf_out.emplace_back( rnd_phase(re), rnd_period(re), fabs(rnd_amp(re)), 0, abs(rnd_count(re)) );
	}

	return wf_out;
}


#endif