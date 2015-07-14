#include "gpi.h"

#include <iostream>
#include <cmath>
#include <limits>

bool isClose(double a, double b) {
    return std::abs(a - b) < 0.0001;
}

int main() {
    std::cerr << "Test" << std::endl;
    //It is recommended to provide certain constant values (particularly 0, 1, and 2) even when solving simple formulas.
    //This allows the program to take the same branching path every time and compute constants easier.
    double inputs[] = {0, 1, 2, 1, 2, 3};
    
    std::mt19937 rand;
    rand.seed(time(0));
    //Play around with the parameters based on your needs; higher mutation rates will not mutate the fittest individual.
    //If using 0 chromosomes, the output layer has one chromosome per output, but there are no internal chromosomes.
    /*                  populationSize, mutationRate,   mutationTries,  inputs, outputs,    chromosomes,    chromSize*/
    gpi::Population pop(100,            0.5,            20,            6,      2,          8,              32, rand);
    
    //A single program can be initialized similarly and manual mutation and crossover is available (see header)
    
    for (int i = 0; ; i++) {
        //Use this function to generate a new program using crossover from two parents automatically with mutations.
        gpi::Program &prog = pop.next(rand);
        std::cout << "Trial " << i << std::endl;
        int success = 0;
        for (int test = 0; test != 32; test++) {
            //Generate random inputs
            inputs[3] = rand()%1000 + 1;
            inputs[4] = rand()%1000 + 1;
            inputs[5] = rand()%1000 + 1;
            //Always call this after inputs are modified once before solving!
            prog.startSolve();
            
            //Increase program fitness if it can solve an intermediate result
            if (isClose(prog.solveOutput(1, inputs), inputs[3] * inputs[4]))
                success++;
            //Increase program fitness if it can solve the whole result
            if (isClose(prog.solveOutput(0, inputs), inputs[2] + inputs[3] * inputs[4]))
                success++;
        }
        std::cout << success << "/64 successes" << std::endl;
        if (success == 64) {
            //Program was sucessful in every attempt
            std::cout << "Program correct!" << std::endl;
            break;
        }
        //Rate the programs fitness as its sucesses * 1024; a 0 value becomes 1 internally.
        //Ensure that sucessful individuals breed over completely unsucessful individuals by increasing their fitness.
        pop.rate(1024 * success);
    }
    
    return 0;
}
