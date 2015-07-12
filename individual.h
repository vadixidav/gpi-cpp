#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "gpi.h"
#include <cstdint>

namespace gpi {
    struct Individual {
        uint64_t fitness;
        Program program;
        
        Individual(uint64_t fitness, unsigned totalInputs, unsigned totalOutputs, unsigned totalInternalChromosomes,
                   unsigned totalChromosomeSize, std::mt19937 &rand) : fitness(fitness),
                   program(totalInputs, totalOutputs, totalInternalChromosomes, totalChromosomeSize, rand) {
        }
        
        Individual(uint64_t fitness, const Program &parent) : fitness(fitness), program(parent) {
        }
    };
}

#endif // INDIVIDUAL_H

