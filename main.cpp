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
    gpi::Population pop(100, 0.5, 100, 7, 4, 10, 32, rand);
    
    for (int i = 0; ; i++) {
        gpi::Program &prog = pop.next(rand);
        prog.startSolve();
        std::cout << "Trial " << i << std::endl;
        int j;
        int sucess = 0;
        for (j = 0; j != 4; j++) {
            double res = prog.solveOutput(j, inputs);
            if (isClose(res, outputs[j])) {
                sucess++;
            }
        }
        std::cout << sucess << " successes" << std::endl;
        if (sucess == 4) {
            std::cout << "Program correct!" << std::endl;
            break;
        }
        pop.rate(1024 * sucess / 4);
    }
    
    return 0;
}
