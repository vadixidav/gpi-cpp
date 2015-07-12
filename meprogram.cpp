#include "meprogram.h"
#include <cassert>

namespace gpi {
    Scratchpad::Scratchpad() {
        result = NAN;
    }
    
    bool Scratchpad::isSolved() {
        return !std::isnan(result);
    }

    MEProgram::MEProgram(unsigned totalInputs, unsigned totalOutputs, unsigned totalInternalChromosomes,
                     unsigned totalChromosomeSize, std::mt19937 &rand) : tInputs(totalInputs), tOutputs(totalOutputs),
                     tInternalChromosomes(totalInternalChromosomes), tChromosomeSize(totalChromosomeSize) {
        //Inputs and chromosomes cannot total higher than 32768
        assert(totalInputs + totalInternalChromosomes <= 32768);
        assert(totalChromosomeSize <= 32768);
        
        //Reserve enough space in the vector so we can emplace all chromosomes without resizing
        internalChromosomes.reserve(tInternalChromosomes);
        outputChromosomes.reserve(tOutputs);
        scratchValues.reserve(tInternalChromosomes);
        
        randomize(rand);
    }
    
    MEProgram::MEProgram(const MEProgram &parent) {
        *this = parent;
    }
    
    void MEProgram::crossover(const MEProgram &parent, std::mt19937 &rand) {
        assert(tInputs == parent.tInputs);
        assert(tOutputs == parent.tOutputs);
        assert(tInternalChromosomes == parent.tInternalChromosomes);
        assert(tChromosomeSize == parent.tChromosomeSize);
        
        for (unsigned i = 0; i != tInternalChromosomes; i++) {
            unsigned crossoverPoint = rand() % tChromosomeSize;
            internalChromosomes[i].crossover(parent.internalChromosomes[i], crossoverPoint, (rand() % 2));
        }
        for (unsigned i = 0; i != tOutputs; i++) {
            unsigned crossoverPoint = rand() % tChromosomeSize;
            outputChromosomes[i].crossover(parent.outputChromosomes[i], crossoverPoint, (rand() % 2));
        }
    }
    
    void MEProgram::randomize(std::mt19937 &rand) {
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
    
    void MEProgram::mutate(std::mt19937 &rand) {
        auto selection = rand() % (tInternalChromosomes + tOutputs);
        if (selection < tInternalChromosomes) {
            internalChromosomes[selection].mutate(selection + tInputs, rand);
        } else {
            outputChromosomes[selection - tInternalChromosomes].mutate(tInternalChromosomes + tInputs, rand);
        }
    }
    
    double solveChromosome(MEProgram &MEProgram, const Chromosome &chromosome, double *inputs);
    double solveChromosomeInstruction(MEProgram &MEProgram, const Chromosome &chromosome, double *inputs,
                                             std::vector<Scratchpad> &intermediates, uint16_t sel);
    
    double solveSelection(MEProgram &MEProgram, const Chromosome &chromosome, double *inputs,
                                 std::vector<Scratchpad> &intermediates, uint16_t sel) {
        if (sel & CHROMOSOME_BIT) {
            uint16_t index = sel & ~CHROMOSOME_BIT;
            if (index < MEProgram.tInputs)
                return inputs[index];
            
            index -= MEProgram.tInputs;
            
            if (MEProgram.scratchValues[index].isSolved())
                return MEProgram.scratchValues[index].result;
            
            MEProgram.scratchValues[index].result = solveChromosome(MEProgram, MEProgram.internalChromosomes[index],
                                                                         inputs);
            assert(std::isnormal(MEProgram.scratchValues[index].result));
            
            return MEProgram.scratchValues[index].result;
        }
        
        if (intermediates[sel].isSolved())
            return intermediates[sel].result;
        
        return solveChromosomeInstruction(MEProgram, chromosome, inputs, intermediates, sel);
    }
    
    double solveChromosomeInstruction(MEProgram &MEProgram, const Chromosome &chromosome, double *inputs,
                                             std::vector<Scratchpad> &intermediates, uint16_t sel) {
        const MUXInstruction &mins = chromosome.instructions[sel];
        auto choice = mins.choose(solveSelection(MEProgram, chromosome, inputs, intermediates,
                                                        mins.params[0]),
                                  solveSelection(MEProgram, chromosome, inputs, intermediates,
                                                        mins.params[1]));
        const Instruction &ins = mins.choices[choice];
        intermediates[sel].result = ins.solve(solveSelection(MEProgram, chromosome, inputs, intermediates,
                                                           ins.params[0]),
                                     solveSelection(MEProgram, chromosome, inputs, intermediates,
                                                           ins.params[1]));
        
        if (!std::isnormal(intermediates[sel].result))
            intermediates[sel].result = 1;
        
        return intermediates[sel].result;
    }
    
    double solveChromosome(MEProgram &MEProgram, const Chromosome &chromosome, double *inputs) {
        std::vector<Scratchpad> intermediates(chromosome.instructions.size());
        return solveChromosomeInstruction(MEProgram, chromosome, inputs, intermediates,
                                                 chromosome.instructions.size() - 1);
    }
    
    double MEProgram::solveOutput(unsigned output, double *inputs) {
        return solveChromosome(*this, outputChromosomes[output], inputs);
    }
    
    void MEProgram::startSolve() {
        scratchValues.resize(0);
        scratchValues.resize(tInternalChromosomes);
    }
}
