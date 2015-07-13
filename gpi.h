#ifndef GPI_H
#define GPI_H

#include <random>

namespace gpi {
    class Program {
        void *d;
        
    public:
        Program(unsigned totalInputs, unsigned totalOutputs, unsigned totalInternalChromosomes,
                unsigned totalChromosomeSize, std::mt19937 &rand); //Generate a new random program
        Program(const Program &parent); //Make this a copy of the parent
        ~Program();
        void crossover(const Program &parent, std::mt19937 &rand); //Crossover this program with another
        void randomize(std::mt19937 &rand); //Completely randomize program
        void mutate(std::mt19937 &rand); //Randomly mutates one piece of the program
        void startSolve(); //Must be called before solving using new inputs!
        double solveOutput(unsigned output, double *inputs); //Solves one output and remembers intermediate results
    };
    
    class Population {
        void *d;
        
    public:
        //Every member of the population will have these constraints after the population is created
        Population(unsigned maxPrograms, double mutationRate, unsigned mutationTries, unsigned totalInputs,
                      unsigned totalOutputs, unsigned totalInternalChromosomes,
                      unsigned totalChromosomeSize, std::mt19937 &rand);
        ~Population();
        Program& next(std::mt19937 &rand); //Call this to get a program to test
        void rate(uint64_t fitness); //Call this to rate the program based on its fitness
    };
}

#endif // GPI_H

