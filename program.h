#ifndef PROGRAM_H
#define PROGRAM_H

#include "chromosome.h"

namespace gpi {
    struct Scratchpad {
        double result;
        
        Scratchpad();

        bool isSolved();
    };

    struct Program {
        unsigned tInputs; //Total inputs to network
        unsigned tOutputs; //Total outputs from network
        unsigned tInternalChromosomes; //Extra internal chromosomes
        unsigned tChromosomeSize; //Instructions in a chromosome
        
        std::vector<Chromosome> internalChromosomes;
        std::vector<Chromosome> outputChromosomes;
        std::vector<Scratchpad> scratchValues;
        
        Program(int totalInputs, int totalOutputs, int totalInternalChromosomes,
                int totalChromosomeSize, std::mt19937 &rand);
        
        Program(const Program &parent);
        
        void randomize(std::mt19937 &rand);
        void mutate(std::mt19937 &rand);
        void startSolve();
        double solveOutput(unsigned output, double *inputs);
    };
}

#endif // PROGRAM_H
