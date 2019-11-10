#include <iostream>
#include <string>
#include "diffusion.h"

int main(int argc, char** argv) {
    Diffusion d(100, 1, 1, 1, 0.1);
    d.set(25, 25, 20);
    d.set_decay(0.1);
    for (unsigned int i = 0; i < 500; ++i) {
        d.adi_step();
        if (i % 50 == 0) {
            d.write_to_csv("outputs/diffusion_" + std::to_string(i) + ".csv");
        }
        if (i == 100) {
            d.set(75, 75, 15);
        }
    }
    d.write_to_csv("outputs/diffusion_" + std::to_string(500) + ".csv");
    return 0;
}
