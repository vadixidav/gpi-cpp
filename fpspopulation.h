#ifndef FPSPOPULATION_H
#define FPSPOPULATION_H

#include <list>
#include "individual.h"

namespace gpi {
    struct FPSPopulation {
        std::list<Individual> individuals;
        uint64_t totalFitness;
        uint64_t bestFitness;
        unsigned maxPrograms;
        double mutationRate;
        unsigned mutationTries;
        
        unsigned totalInputs;
        unsigned totalOutputs;
        unsigned totalInternalChromosomes;
        unsigned totalChromosomeSize;
        
        FPSPopulation(unsigned maxPrograms, double mutationRate, unsigned mutationTries, unsigned totalInputs,
                      unsigned totalOutputs, unsigned totalInternalChromosomes,
                      unsigned totalChromosomeSize, std::mt19937 &rand);
        Program& next(std::mt19937 &rand);
        void rate(uint64_t fitness);
    };
}

#endif // FPSPOPULATION_H

