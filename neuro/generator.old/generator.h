#ifndef generator_h
#define generator_h

#include <random>
#include <chrono>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <iostream>

#include "../run/consts.h"

// Create and seed a RNG
std::default_random_engine g((unsigned int) std::chrono::system_clock::now().time_since_epoch().count());

std::normal_distribution<float> gen_delay{MU_DELAY, SIG_DELAY};
std::normal_distribution<float> gen_alpha{MU_ALPHA, SIG_ALPHA};

struct Neuron {
    unsigned int num;
    Coord coord;
    int chem;
    
    bool operator<(const Neuron& n) const { return coord < n.coord; }
    bool operator==(const Neuron& n) const { return coord == n.coord; }
};

struct conn_data {
    float alpha;
    float delay;

    // default ctor
    conn_data() : alpha(fabs(gen_alpha(g))), delay(fabs(gen_delay(g))) {}
};

template<typename T>
class Generator {
private:
    // Dimensions of the square grid
    const unsigned int dim_;
    // Number of grid points to pad boundary
    const unsigned int pad_;
    
    // List of all neurons
    std::vector<Neuron> neurons;
    
    // Key is postsynaptic neuron. Value is list of presynaptic neurons
    // with the strengths of connections.
        // connection data struct

    std::map<Neuron, std::vector<std::pair<Neuron, conn_data>>> connections;
    
public:
    // Constructor
    Generator(unsigned int dim, unsigned int pad) : dim_{ dim }, pad_{ pad } {};
    
    // Generate num neurons uniformly on the grid inside the padding.
    // Axons are assumed to be very long relative to dendrites. That is,
    // we assume that the axons are as long as the distance between neurons,
    // and that the dendrite lengths are negligible.
    //
    // If num is too large, this function may time out.
    void generate_neurons(unsigned int num);
    
    // Each neuron is connected to a number of other neurons. The number is
    // determined by the ceiling of a normal distribution with mean mu and
    // standard deviation sigma.
    //
    // The probability that a neuron forms a connection with another neuron
    // is uniform. The strength of each connection is chosen uniformly
    // between 0 and 1. Each neuron can form a maximum of max connections.
    //
    // Requirements: max > # neurons and 0, mu > 0, sigma > 0
    void generate_connections(unsigned int max, float mu, float sigma);
    
    // Prints list of neurons to os. Does not check state of os.
    // Format: #, x_coor, y_coor, chem type
    void print_neurons(std::ostream& os);
    
    // Calls print_neurons to write to CSV file
    void write_neuron_csv(const std::string& file);
    
    // Prints list of connections to os. Does not check state of os.
    // Format: presynaptic coords, postsynaptic coords, connection strength
    void print_connections(std::ostream& os);
    
    // Calls print_connections to write to CSV file
    void write_connections_csv(const std::string& file);
};

#endif
