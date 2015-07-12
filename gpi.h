#ifndef GPI_H
#define GPI_H

#include <random>

namespace gpi {
    class Program {
        void *d;
        
    public:
        Program(int totalInputs, int totalOutputs, int totalInternalChromosomes,
                int totalChromosomeSize, std::mt19937 &rand);
        ~Program();
        void crossover(const Program &parent, std::mt19937 &rand);
        void randomize(std::mt19937 &rand);
        void mutate(std::mt19937 &rand);
        void startSolve(); //Must be called before solving using new inputs!
        double solveOutput(unsigned output, double *inputs);
    };
}

#endif // GPI_H

