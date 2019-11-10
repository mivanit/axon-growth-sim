// Michael Ivanitskiy, John Balis, Connor Puritz 2019
// for support email:
// balisujohn@gmail.com 
// mivanits@umich.edu

#include "network.h"
#include "neuron.h"
#include "synapse.h"
#include "../waveform/waveform_D.h"

// using namespace std;

int main(int argc, char ** argv)
{
	if (argc <= 3 || argv[1] == "-h" || argv[1] == "--help" )
	{
		std::cout << "TODO\n";
		return 0;
	}

	network net(argv[1], argv[2], argv[3]);
	return 0;
}
