#include "gpi.h"

#include <iostream>
#include <cmath>
#include <limits>

bool isClose(double a, double b) {
    return std::abs(a - b) < 0.0001;
}

int main() {
    std::cerr << "Test" << std::endl;
    double inputs[] = {0, std::numeric_limits<double>::max(), 1, 2, 3};
    
    std::mt19937 rand;
    rand.seed(time(0));
    gpi::Population pop(100, 1.0, 500, 5, 2, 10, 32, rand);
    
    for (int i = 0; ; i++) {
        gpi::Program &prog = pop.next(rand);
        prog.startSolve();
        std::cout << "Trial " << i << std::endl;
        int success = 0;
        for (int test = 0; test != 32; test++) {
            inputs[2] = rand()%1000 + 1;
            inputs[3] = rand()%1000 + 1;
            inputs[4] = rand()%1000 + 1;
            if (isClose(prog.solveOutput(0, inputs), inputs[2] + inputs[3] * inputs[4])) {
                success++;
            }
            if (isClose(prog.solveOutput(1, inputs), inputs[3] * inputs[4])) {
                success++;
            }
        }
        std::cout << success << "/64 successes" << std::endl;
        if (success == 64) {
            std::cout << "Program correct!" << std::endl;
            break;
        }
        pop.rate(1024 * success);
    }
    
    return 0;
}
