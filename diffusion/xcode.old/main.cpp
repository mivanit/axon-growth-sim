#include <iostream>
#include "diffusion.h"

int main(int argc, char** argv) {
    Diffusion d(50, 1, 1, 1, 0.1);
    d.set(25, 25, 20);
    d.set_decay(0.1);
    for (unsigned int i = 0; i < 100; ++i) {
        d.adi_step();
    }
    d.write_to_csv("/Users/connorpuritz/Desktop/output.CSV");
    return 0;
}
