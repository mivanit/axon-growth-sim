#include <iostream>
#include "diffusion.h"

int main(int argc, char** argv) {
    Diffusion<double> d(50, 1.9, 1, 1);
    d.set(25, 25, 20);
    d.set_decay(0.1);
    double dt = 0.1;
    int n = 100;
    d.adi(0, dt, n * dt);
    d.write_to_csv("/Users/connorpuritz/Desktop/output.CSV");
    return 0;
}
