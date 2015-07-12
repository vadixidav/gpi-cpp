#ifndef MEPROGRAM_H
#define MEPROGRAM_H

#include "chromosome.h"

namespace gpi {
    struct Scratchpad {
        double result;
        
        Scratchpad();

        bool isSolved();
    };

    struct MEProgram {
        unsigned tInputs; //Total inputs to network
        unsigned tOutputs; //Total outputs from network
        unsigned tInternalChromosomes; //Extra internal chromosomes
        unsigned tChromosomeSize; //Instructions in a chromosome
        
        std::vector<Chromosome> internalChromosomes;
        std::vector<Chromosome> outputChromosomes;
        std::vector<Scratchpad> scratchValues;
        
        //Construct a new program and randomize it
        MEProgram(int totalInputs, int totalOutputs, int totalInternalChromosomes,
                int totalChromosomeSize, std::mt19937 &rand);
        
        //Construct a new program as a copy of another
        MEProgram(const MEProgram &parent);
        
        void crossover(const MEProgram &parent, std::mt19937 &rand);
        void randomize(std::mt19937 &rand);
        void mutate(std::mt19937 &rand);
        void startSolve(); //Run this every time the program or the inputs change before solving again
        double solveOutput(unsigned output, double *inputs);
    };
}

#endif // MEPROGRAM_H
