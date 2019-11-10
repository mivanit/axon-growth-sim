//  generator.cpp

#include "generator.h"

/**********************************************************************/

template<typename T>
void Generator<T>::generate_neurons(unsigned int num) {
    // Uniformly generate neuron locations
    std::uniform_int_distribution<int> n_unif(pad_, dim_ - pad_ - 1);
    // Randomly generate chem types
    std::uniform_int_distribution<int> c_unif(_MINCHEM_, _MAXCHEM_);
    
    // If max is larger than the number of available grid points, or very
    // close to that number, this function might take forever of never
    // terminate. This could probably be done better, but we'll just
    // have the function abort after 100 failed attempts to create a
    // unique neuron.
    unsigned int timeout = 0;
    
    for (unsigned int i = 0; i < num; ++i) {
        // Randomly generate the i-th neuron
        Neuron n = {i, {n_unif(g), n_unif(g)}, c_unif(g)};
        // If a neuron with the same coordinates as n has already been
        // generated, go back and try again
        for (unsigned int j = 0; j < neurons.size(); ++j) {
            if (neurons[j] == n) {
                ++timeout;
                --i;
                goto quit;
            }
        }
        neurons.push_back(n);
        quit:;
        
        if (timeout > 100) {
            std::cerr << "Possible infinite loop in neuron generation, aborting."
                      << std::endl;
            neurons.clear();
            return;
        }
    }
}

/**********************************************************************/

template<typename T>
void Generator<T>::generate_connections(unsigned int max, float mu, float sigma) {
    if (max > neurons.size() - 1 || max == 0) {
        std::cerr << "Invalid value for max, aborting." << std::endl;
        return;
    } else if (mu <= 0) {
        std::cerr << "Invalid value for mu, aborting." << std::endl;
        return;
    } else if (sigma <= 0) {
        std::cerr << "Invalid value for sigma, aborting." << std::endl;
        return;
    }
    
    // Generate connections
    std::normal_distribution<float> normal(mu, sigma);
    for (Neuron n : neurons) {
        // Number of connections
        unsigned int nc =
            std::min((unsigned int) std::max((int) std::ceil(normal(g)), 1),
                 max);
        // Pick neurons to connect to
        std::uniform_int_distribution<int> c_unif(0, (int) neurons.size() - 1);
        
        // Keep track of connections made so there are no duplicates
        std::set<Coord> c_locs;
        for (unsigned int i = 0; i < nc; ++i) {
            Neuron c = neurons.at(c_unif(g));
            // A neuron can't connect with itself, so go back
            if (c == n) {
                --i;
            } else {
                size_t pre = c_locs.size();
                c_locs.insert(c.coord);
                // If a connection was already formed with this neuron, go back
                if (pre == c_locs.size()) {
                    --i;
                } else {
                    connections[n].push_back({c, conn_data()});
                }
            }
        }
    }
    
    // Remove loops (e.g. neuron 1 and neuron 2 both connect to each other
    for (auto itr = connections.begin(); itr != connections.end(); ++itr) {
        Neuron n = itr->first;
        // Look at the connections made with each neuron that connects
        // to neuron n
        for (std::pair<Neuron, conn_data> p : itr->second) {
            Neuron c = p.first;
            std::vector<std::pair<Neuron, conn_data>>& clist = connections[c];
            // If c connects back to n, delete n from c's list of connections
            for (unsigned int i = 0; i < clist.size(); ++i) {
                if (clist[i].first.coord == n.coord) {
                    clist.erase(clist.begin() + i);
                    break;
                }
            }
        }
    }
}

/**********************************************************************/

template<typename T>
void Generator<T>::print_neurons(std::ostream& os) {
    os << "ID, coord_x, coord_y, chemType \n";
    for (Neuron n : neurons) {
        // Neuron #, x coor, y coor, chem type
        os << n.num << ", " << n.coord.first << ", " << n.coord.second
           << ", " << n.chem << "\n";
    }
}

/**********************************************************************/

template<typename T>
void Generator<T>::write_neuron_csv(const std::string& file) {
    std::ofstream ofs(file);
    if (!ofs.is_open()) {
        std::cerr << "Error opening " << file << ", aborting."
                  << std::endl;
    } else {
        print_neurons(ofs);
    }
}

/**********************************************************************/

template<typename T>
void Generator<T>::print_connections(std::ostream& os) {
    // Presynaptic coordinate, postsynaptic coordinate, connection strength
    for (Neuron n : neurons) {
        for (auto pair : connections[n]) {
            Neuron c = pair.first;
            os << c.num << ", " << n.num << ", " << pair.second.alpha << ", " << pair.second.delay << "\n";
        }
    }
}

/**********************************************************************/

template<typename T>
void Generator<T>::write_connections_csv(const std::string& file) {
    std::ofstream ofs(file);
    if (!ofs.is_open()) {
        std::cerr << "Error opening " << file << ", aborting."
                  << std::endl;
    } else {
        ofs << "pre, post, alpha, delay \n";
        print_connections(ofs);
    }
}

/**********************************************************************/

// Explicit template instantiations
// ~ template class Generator<double>;
template class Generator<float>;
// ~ template class Generator<long>;

/**********************************************************************/

int main(int argc, char ** argv)
{
    std::string dir = "";
    if (argc > 1)
    {
        if (argv[1] == "-h" || argv[1] == "--help" )
        {
            std::cout << "generator.cpp \n"
                    << "generates network topology and exports to csv format \n"
                    << "usage: \n"
                    << " ./gen_wf.exe <path_save>    :    saves .csv files into the given folder";
            return 0;
        }

        dir = argv[1];
    }

    // gen
	Generator<float> net(N_GRIDSIZE, N_PADDING);
    net.generate_neurons( N_NEURONS );
    net.generate_connections( MAX_CONN, MU_CONN, SIG_CONN);

    // NOTE: if modifying network topology during operation, then this should be written seperately
    // write
    net.write_neuron_csv(dir + "neurons.csv");
    net.write_connections_csv(dir + "net_topo.csv");
}