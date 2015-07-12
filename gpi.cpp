#include "gpi.h"
#include "meprogram.h"

namespace gpi {
    Program::Program(int totalInputs, int totalOutputs, int totalInternalChromosomes, int totalChromosomeSize,
                     std::mt19937 &rand) {
        d = (void*)new MEProgram(totalInputs, totalOutputs, totalInternalChromosomes, totalChromosomeSize, rand);
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
}

