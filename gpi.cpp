#include "gpi.h"
#include "meprogram.h"
#include "fpspopulation.h"

namespace gpi {
    Program::Program(unsigned totalInputs, unsigned totalOutputs, unsigned totalInternalChromosomes,
                     unsigned totalChromosomeSize, std::mt19937 &rand) {
        d = (void*)new MEProgram(totalInputs, totalOutputs, totalInternalChromosomes, totalChromosomeSize, rand);
    }
    
    Program::Program(const Program &parent) {
        d = (void*)new MEProgram(*(MEProgram*)parent.d);
    }
    
    Program::~Program() {
        delete (MEProgram*)d;
    }
    
    void Program::crossover(const Program &parent, std::mt19937 &rand) {
        ((MEProgram*)d)->crossover(*(MEProgram*)parent.d, rand);
    }
    
    void Program::randomize(std::mt19937 &rand) {
        ((MEProgram*)d)->randomize(rand);
    }
    
    void Program::mutate(std::mt19937 &rand) {
        ((MEProgram*)d)->mutate(rand);
    }
    
    void Program::startSolve() {
        ((MEProgram*)d)->startSolve();
    }
    
    double Program::solveOutput(unsigned output, double *inputs) {
        return ((MEProgram*)d)->solveOutput(output, inputs);
    }
    
    Population::Population(unsigned maxPrograms, double mutationRate, unsigned mutationTries, unsigned totalInputs,
                           unsigned totalOutputs, unsigned totalInternalChromosomes, unsigned totalChromosomeSize,
                           std::mt19937 &rand) {
        d = (void*)new FPSPopulation(maxPrograms, mutationRate, mutationTries, totalInputs, totalOutputs,
                                     totalInternalChromosomes, totalChromosomeSize, rand);
    }
    
    Population::~Population() {
        delete (FPSPopulation*)d;
    }
    
    Program& Population::next(std::mt19937 &rand) {
        return ((FPSPopulation*)d)->next(rand);
    }
    
    void Population::rate(uint64_t fitness) {
        ((FPSPopulation*)d)->rate(fitness);
    }
}

