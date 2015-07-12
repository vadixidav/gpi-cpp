#include "gpi.h"

#include <iostream>
#include <cmath>
#include <limits>

bool isClose(double a, double b) {
    return std::abs(a - b) < 0.0001;
}

int main() {
    std::cerr << "Test" << std::endl;
    double inputs[] = {0, std::numeric_limits<double>::max(), 1, 2, 3, 4, 5};
    double outputs[] = {4, 5, 6, 7};
    
    std::mt19937 rand;
    rand.seed(time(0));
    gpi::Program prog(7, 4, 10, 32, rand);
    
    for (int i = 0; ; i++) {
        prog.randomize(rand);
        prog.startSolve();
        std::cout << "Trial " << i << std::endl;
        int j;
        for (j = 0; j != 4; j++) {
            double res = prog.solveOutput(j, inputs);
            if (isClose(res, outputs[j])) {
                std::cout << "Output match" << std::endl;
            } else {
                std::cout << "Output fail" << std::endl;
                break;
            }
        }
        if (j == 4) {
            std::cout << "Success!" << std::endl;
            break;
        }
    }
    
    return 0;
}
