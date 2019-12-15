//
//  main.cpp
//  generator
//
//  Created by Connor Puritz on 4/16/19.
//  Copyright © 2019 umich. All rights reserved.
//

#include <iostream>
#include "generator.h"

int main(int argc, const char * argv[]) {
    unsigned int num = 10;
    unsigned int dim = 8;
    unsigned int pad = 2;
    Generator<float> g(dim, pad);
}
