#include "chromosome.h"

#include <cmath>

namespace gpi {
    Chromosome::Chromosome(unsigned length) {
        instructions.resize(length);
    }
    
    Chromosome::~Chromosome() {
    }
    
    void Chromosome::randomize(unsigned position, std::mt19937 &rand) {
        for (unsigned i = 0; i != instructions.size(); i++) {
            MUXInstruction &ins = instructions[i];
            ins.randomize(i, position, rand);
        }
    }
    
    void Chromosome::mutate(unsigned position, std::mt19937 &rand) {
        unsigned selection = rand() % instructions.size();
        instructions[selection].mutate(selection, position, rand);
    }
    
    void Chromosome::crossover(const Chromosome &other, unsigned point, bool direction) {
        if (direction) {
            for (unsigned i = point; i != instructions.size(); i++)
                instructions[i] = other.instructions[i];
        } else {
            for (unsigned i = 0; i != point; i++)
                instructions[i] = other.instructions[i];
        }
    }

    double Instruction::solve(double a, double b) const {
        switch (opcode) {
        case Opcode::ADD:
            return a + b;
        case Opcode::SUB:
            return a - b;
        case Opcode::MUL:
            return a * b;
        case Opcode::DIV:
            if (std::isnormal(b))
                return a / b;
            return 1; //Return 1 in case division would make no sense
        case Opcode::MAX:
            break;
        }
        //Execution should not reach this point
        return NAN;
    }
    
    uint16_t produceParam(unsigned position, unsigned chromosomePosition, std::mt19937 &rand) {
        uint16_t val = position == 0 ? CHROMOSOME_BIT : (uint16_t)rand() & CHROMOSOME_BIT;
        return val | (val ? rand() % chromosomePosition : rand() % position);
    }
    
    int_fast8_t MUXInstruction::choose(double a, double b) const {
        return a < b; //Always perform less than
    }
    
    void MUXInstruction::randomize(unsigned position, unsigned chromosomePosition, std::mt19937 &rand) {
        //TODO: Make it so the choice is not 50% for both but adapts instead
        params[0] = produceParam(position, chromosomePosition, rand);
        params[1] = produceParam(position, chromosomePosition, rand);
        choices[0].opcode = Opcode(rand() % unsigned(Opcode::MAX));
        choices[0].params[0] = produceParam(position, chromosomePosition, rand);
        choices[0].params[1] = produceParam(position, chromosomePosition, rand);
        choices[1].opcode = Opcode(rand() % unsigned(Opcode::MAX));
        choices[1].params[0] = produceParam(position, chromosomePosition, rand);
        choices[1].params[1] = produceParam(position, chromosomePosition, rand);
    }
    
    void MUXInstruction::mutate(unsigned position, unsigned chromosomePosition, std::mt19937 &rand) {
        auto choice = rand() % 8;
        if (choice < 2) {
            params[choice] = produceParam(position, chromosomePosition, rand);
        } else if (choice < 4) {
            choices[choice-2].opcode = Opcode(rand() % unsigned(Opcode::MAX));
        } else if (choice < 6) {
            choices[choice-4].params[0] = produceParam(position, chromosomePosition, rand);
        } else {
            choices[choice-6].params[1] = produceParam(position, chromosomePosition, rand);
        }
    }
}

