#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <cstdint>
#include <vector>
#include <random>

#define CHROMOSOME_BIT 0x8000

namespace gpi {
    enum class Opcode {
        ADD,
        SUB,
        MUL,
        DIV,
        MAX
    };

    struct Instruction {
        Opcode opcode;
        uint16_t params[2];
        double solve(double a, double b) const;
    };
    
    struct MUXInstruction {
        uint16_t params[2];
        Instruction choices[2];
        
        int_fast8_t choose(double a, double b) const;
        void randomize(unsigned position, unsigned chromosomePosition, std::mt19937 &rand);
        void mutate(unsigned position, unsigned chromosomePosition, std::mt19937 &rand);
    };

    struct Chromosome {
        std::vector<MUXInstruction> instructions;
        
        Chromosome(unsigned length);
        ~Chromosome();
        
        void randomize(unsigned position, std::mt19937 &rand);
        void mutate(unsigned position, std::mt19937 &rand);
        void crossover(const Chromosome &other, unsigned point, bool direction);
    };
}

#endif // CHROMOSOME_H

