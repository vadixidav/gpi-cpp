#include "fpspopulation.h"
#include <assert.h>

namespace gpi {
    FPSPopulation::FPSPopulation(unsigned maxPrograms, double mutationRate, unsigned mutationTries,
                                 unsigned totalInputs, unsigned totalOutputs, unsigned totalInternalChromosomes,
                                 unsigned totalChromosomeSize, std::mt19937 &rand) : maxPrograms(maxPrograms),
                                 mutationRate(mutationRate), mutationTries(mutationTries) {
        //Generate seed program with 1 fitness
        individuals.emplace_back(1, totalInputs, totalOutputs, totalInternalChromosomes, totalChromosomeSize, rand);
        totalFitness = 1; //Total fitness is now 1
        bestFitness = 1;
        
        assert(maxPrograms > 0); //Max programs must be at least 1 or it wont work
    }
    
    Program& FPSPopulation::next(std::mt19937 &rand) {
        //Select two programs to breed proportional to fitness
        uint64_t selection1 = (uint64_t(rand()) | (uint64_t(rand()) << 32)) % totalFitness;
        uint64_t selection2 = (uint64_t(rand()) | (uint64_t(rand()) << 32)) % totalFitness;
        
        //Ensure selection1 is smaller or equal to selection2
        if (selection1 > selection2)
            std::swap(selection1, selection2);
        
        auto i = individuals.begin();
        
        //Find selection1 and copy it onto the back
        for (; ; i++) {
            if (selection1 < i->fitness) {
                individuals.emplace_back(1, i->program); //Initialize with 1 fitness
                totalFitness++; //Increment fitness since we just added 1
                break;
            }
            selection1 -= i->fitness;
            selection2 -= i->fitness;
        }
        
        //Find selection2 and perform a crossover with the back
        for (; ; i++) {
            if (selection2 < i->fitness) {
                individuals.back().program.crossover(i->program, rand);
                break;
            }
            selection2 -= i->fitness;
        }
        
        for (unsigned i = 0; i != mutationTries; i++)
            if (double(rand())/rand.max() < mutationRate)
                individuals.back().program.mutate(rand);
        
        if (individuals.size() > maxPrograms) {
            //Avoid removing best individual; even if there are multiple best, removing the second element does no harm
            if (individuals.front().fitness == bestFitness) {
                auto i = individuals.begin();
                i++; //Advance to second element
                totalFitness -= i->fitness;
                individuals.erase(i);
            } else {
                totalFitness -= individuals.front().fitness;
                individuals.pop_front();
            }
        }
        
        return individuals.back().program;
    }
    
    void FPSPopulation::rate(uint64_t fitness) {
        totalFitness -= individuals.back().fitness;
        individuals.back().fitness = fitness + 1; //Add 1 so we never get 0 fitness (theoretically)
        totalFitness += individuals.back().fitness;
        if (individuals.back().fitness > bestFitness)
            bestFitness = individuals.back().fitness;
    }
}
