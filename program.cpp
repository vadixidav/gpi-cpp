#include "program.h"
#include <cassert>

namespace gpi {
    Scratchpad::Scratchpad() {
        result = NAN;
    }
    
    bool Scratchpad::isSolved() {
        return !std::isnan(result);
    }

    Program::Program(int totalInputs, int totalOutputs, int totalInternalChromosomes,
                     int totalChromosomeSize, std::mt19937 &rand) {
        tInputs = totalInputs;
        tOutputs = totalOutputs;
        tInternalChromosomes = totalInternalChromosomes;
        tChromosomeSize = totalChromosomeSize;
        
        //Inputs and chromosomes cannot total higher than 32768
        assert(totalInputs + totalInternalChromosomes <= 32768);
        assert(totalChromosomeSize <= 32768);
        
        //Reserve enough space in the vector so we can emplace all chromosomes without resizing
        internalChromosomes.reserve(tInternalChromosomes);
        outputChromosomes.reserve(tOutputs);
        scratchValues.reserve(tInternalChromosomes);
        
        randomize(rand);
    }
    
    Program::Program(const Program &parent) {
        *this = parent;
    }
    
    void Program::randomize(std::mt19937 &rand) {
        //Emplace all chromosomes (there is no empty constructor)
        for (unsigned i = 0; i != tInternalChromosomes; i++)
            internalChromosomes.emplace_back(tChromosomeSize);
        //Perform chromosome initialization
        for (unsigned i = 0; i != tInternalChromosomes; i++) {
            Chromosome &c = internalChromosomes[i];
            c.randomize(i + tInputs, rand); //Chromosome position includes inputs
        }
        
        //Perform same operations for output chromosomes
        for (unsigned i = 0; i != tOutputs; i++)
            outputChromosomes.emplace_back(tChromosomeSize);
        for (unsigned i = 0; i != tOutputs; i++) {
            Chromosome &c = outputChromosomes[i];
            c.randomize(tInternalChromosomes + tInputs, rand); //Here we only allow references to internal only
        }
    }
    
    void Program::mutate(std::mt19937 &rand) {
        auto selection = rand() % (tInternalChromosomes + tOutputs);
        if (selection < tInternalChromosomes) {
            internalChromosomes[selection].mutate(selection + tInputs, rand);
        } else {
            outputChromosomes[selection - tInternalChromosomes].mutate(tInternalChromosomes + tInputs, rand);
        }
    }
    
    double solveProgramChromosome(Program &program, const Chromosome &chromosome, double *inputs);
    double solveProgramChromosomeInstruction(Program &program, const Chromosome &chromosome, double *inputs,
                                             std::vector<Scratchpad> &intermediates, uint16_t sel);
    
    double solveProgramSelection(Program &program, const Chromosome &chromosome, double *inputs,
                                 std::vector<Scratchpad> &intermediates, uint16_t sel) {
        if (sel & CHROMOSOME_BIT) {
            uint16_t index = sel & ~CHROMOSOME_BIT;
            if (index < program.tInputs)
                return inputs[index];
            
            index -= program.tInputs;
            
            if (program.scratchValues[index].isSolved())
                return program.scratchValues[index].result;
            
            program.scratchValues[index].result = solveProgramChromosome(program, program.internalChromosomes[index],
                                                                         inputs);
            assert(std::isnormal(program.scratchValues[index].result));
            
            return program.scratchValues[index].result;
        }
        
        if (intermediates[sel].isSolved())
            return intermediates[sel].result;
        
        return solveProgramChromosomeInstruction(program, chromosome, inputs, intermediates, sel);
    }
    
    double solveProgramChromosomeInstruction(Program &program, const Chromosome &chromosome, double *inputs,
                                             std::vector<Scratchpad> &intermediates, uint16_t sel) {
        const MUXInstruction &mins = chromosome.instructions[sel];
        auto choice = mins.choose(solveProgramSelection(program, chromosome, inputs, intermediates,
                                                        mins.params[0]),
                                  solveProgramSelection(program, chromosome, inputs, intermediates,
                                                        mins.params[1]));
        const Instruction &ins = mins.choices[choice];
        intermediates[sel].result = ins.solve(solveProgramSelection(program, chromosome, inputs, intermediates,
                                                           ins.params[0]),
                                     solveProgramSelection(program, chromosome, inputs, intermediates,
                                                           ins.params[1]));
        
        if (!std::isnormal(intermediates[sel].result))
            intermediates[sel].result = 1;
        
        return intermediates[sel].result;
    }
    
    double solveProgramChromosome(Program &program, const Chromosome &chromosome, double *inputs) {
        std::vector<Scratchpad> intermediates(chromosome.instructions.size());
        return solveProgramChromosomeInstruction(program, chromosome, inputs, intermediates,
                                                 chromosome.instructions.size() - 1);
    }
    
    double Program::solveOutput(unsigned output, double *inputs) {
        return solveProgramChromosome(*this, outputChromosomes[output], inputs);
    }
    
    void Program::startSolve() {
        scratchValues.resize(0);
        scratchValues.resize(tInternalChromosomes);
    }
}
