#include "Day16-ChronalClassification.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <regex>
#include <array>
#include <memory>
#include <unordered_map>
#include <functional>
#include <set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Registers = std::array<int, 4>;

struct Instruction
{
    int opcode;
    int a;
    int b;
    int c;

    Instruction(int opcode, int a, int b, int c)
        : opcode{opcode}
        , a{a}
        , b{b}
        , c{c}
    {

    }
};

struct CapturedSample
{
    Registers before;
    Instruction instruction;
    Registers after;

    CapturedSample(Registers before, Instruction instruction, Registers after)
        : before{std::move(before)}
        , instruction{std::move(instruction)}
        , after{std::move(after)}

    {

    }
};

using OpcodeStringSet = std::set<std::string>;

class InstructionEvaluator
{
private:
    using AssemblyFunc = std::function<void()>;

public:
    InstructionEvaluator(Instruction instruction, Registers registers)
        : m_instruction{std::move(instruction)}
        , m_registers{std::move(registers)}
    {
        m_opcodeStringToAssemblyFunc =
        {
            {"addr", [this]() {this->addr(); }},
            {"addi", [this]() {this->addi(); }},
            {"mulr", [this]() {this->mulr(); }},
            {"muli", [this]() {this->muli(); }},
            {"banr", [this]() {this->banr(); }},
            {"bani", [this]() {this->bani(); }},
            {"borr", [this]() {this->borr(); }},
            {"bori", [this]() {this->bori(); }},
            {"setr", [this]() {this->setr(); }},
            {"seti", [this]() {this->seti(); }},
            {"gtir", [this]() {this->gtir(); }},
            {"gtri", [this]() {this->gtri(); }},
            {"gtrr", [this]() {this->gtrr(); }},
            {"eqir", [this]() {this->eqir(); }},
            {"eqri", [this]() {this->eqri(); }},
            {"eqrr", [this]() {this->eqrr(); }},
        };
    }

    Registers evaluateAs(const std::string& opcodeString)
    {
        Registers registersSnapshot{m_registers};

        m_opcodeStringToAssemblyFunc.at(opcodeString)();

        std::swap(registersSnapshot, m_registers);

        return registersSnapshot;
    }

    OpcodeStringSet getAllOpcodeStrings() const
    {
        OpcodeStringSet allOpcodeStrings;

        std::transform(m_opcodeStringToAssemblyFunc.cbegin(), m_opcodeStringToAssemblyFunc.cend(), std::inserter(allOpcodeStrings, allOpcodeStrings.begin()),
                       [](const auto& elem)
                       {
                           return elem.first;
                       });

        return allOpcodeStrings;
    }

private:
    Instruction m_instruction;
    Registers m_registers;
    std::unordered_map<std::string, AssemblyFunc> m_opcodeStringToAssemblyFunc;

    void addr()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] + m_registers[m_instruction.b];
    }

    void addi()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] + m_instruction.b;
    }

    void mulr()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] * m_registers[m_instruction.b];
    }

    void muli()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] * m_instruction.b;
    }

    void banr()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] & m_registers[m_instruction.b];
    }

    void bani()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] & m_instruction.b;
    }

    void borr()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] | m_registers[m_instruction.b];
    }

    void bori()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a] | m_instruction.b;
    }

    void setr()
    {
        m_registers[m_instruction.c] = m_registers[m_instruction.a];
    }

    void seti()
    {
        m_registers[m_instruction.c] = m_instruction.a;
    }

    void gtir()
    {
        m_registers[m_instruction.c] = (m_instruction.a > m_registers[m_instruction.b]) ? 1 : 0;
    }

    void gtri()
    {
        m_registers[m_instruction.c] = (m_registers[m_instruction.a] > m_instruction.b) ? 1 : 0;
    }

    void gtrr()
    {
        m_registers[m_instruction.c] = (m_registers[m_instruction.a] > m_registers[m_instruction.b]) ? 1 : 0;
    }

    void eqir()
    {
        m_registers[m_instruction.c] = (m_instruction.a == m_registers[m_instruction.b]) ? 1 : 0;
    }

    void eqri()
    {
        m_registers[m_instruction.c] = (m_registers[m_instruction.a] == m_instruction.b) ? 1 : 0;
    }

    void eqrr()
    {
        m_registers[m_instruction.c] = (m_registers[m_instruction.a] == m_registers[m_instruction.b]) ? 1 : 0;
    }

};

using OpcodeToOpcodeString = std::unordered_map<int, std::string>;
using OpcodeToOpcodeStrings = std::unordered_map<int, OpcodeStringSet>;

class OpcodeDiscovery
{
public:
    OpcodeDiscovery(std::vector<CapturedSample> capturedSamples)
        : m_capturedSamples{std::move(capturedSamples)}
        , m_opcodeToPossibleOpcodeStrings{}
    {

    }

    void discover()
    {
        for (const auto& capturedSample : m_capturedSamples)
        {
            auto possibleOpcodeStrings = getPossibleOpcodeStringsForSample(capturedSample);

            processOpcodeDiscovery(capturedSample.instruction.opcode, possibleOpcodeStrings);
        }

        disambiguatePossibleOpcodes();
    }

    unsigned getNumSamplesBehaveMoreThanThreeOpcodes() const
    {
        return m_numSamplesBehaveMoreThanThreeOpcodes;
    }

    OpcodeToOpcodeString getOpcodeToOpcodeString() const
    {
        return m_opcodeToOpcodeStringUnambiguous;
    }

private:
    std::vector<CapturedSample> m_capturedSamples;

    OpcodeToOpcodeStrings m_opcodeToPossibleOpcodeStrings;
    OpcodeToOpcodeString m_opcodeToOpcodeStringUnambiguous;
    unsigned m_numSamplesBehaveMoreThanThreeOpcodes = 0;

    OpcodeStringSet getPossibleOpcodeStringsForSample(const CapturedSample& sample)
    {
        InstructionEvaluator instructionEvaluator{sample.instruction, sample.before};
        OpcodeStringSet allOpcodeStrings = instructionEvaluator.getAllOpcodeStrings();

        OpcodeStringSet possibleOpcodeStrings;

        for (const auto& opcodeString : allOpcodeStrings)
        {
            Registers actualAfter = instructionEvaluator.evaluateAs(opcodeString);

            if (sample.after == actualAfter)
            {
                possibleOpcodeStrings.insert(opcodeString);
            }
        }

        return possibleOpcodeStrings;
    }

    void processOpcodeDiscovery(int opcode, const OpcodeStringSet& constrainedOpcodeStrings)
    {
        if (constrainedOpcodeStrings.size() >= 3)
        {
            ++m_numSamplesBehaveMoreThanThreeOpcodes;
        }

        auto findResult = m_opcodeToPossibleOpcodeStrings.find(opcode);
        if (findResult == m_opcodeToPossibleOpcodeStrings.end())
        {
            m_opcodeToPossibleOpcodeStrings.emplace(opcode, constrainedOpcodeStrings);
        }
        else
        {
            auto& currentPossibleOpcodeStrings = findResult->second;

            OpcodeStringSet intersection;
            set_intersection(currentPossibleOpcodeStrings.cbegin(), currentPossibleOpcodeStrings.cend(),
                             constrainedOpcodeStrings.cbegin(), constrainedOpcodeStrings.cend(),
                             std::inserter(intersection, intersection.begin()));

            currentPossibleOpcodeStrings = intersection;
        }
    }

    void disambiguatePossibleOpcodes()
    {
        auto opcodeToPossibleOpcodeStringsAmbiguous{m_opcodeToPossibleOpcodeStrings};

        std::vector<int> newlyDisambiguatedOpcodes;

        while (!opcodeToPossibleOpcodeStringsAmbiguous.empty())
        {
            newlyDisambiguatedOpcodes.clear();

            for (const auto& elem : opcodeToPossibleOpcodeStringsAmbiguous)
            {
                int opcode = elem.first;
                const OpcodeStringSet& possibleOpcodeStrings = elem.second;

                if (possibleOpcodeStrings.size() == 1)
                {
                    newlyDisambiguatedOpcodes.emplace_back(opcode);
                    m_opcodeToOpcodeStringUnambiguous.emplace(opcode, *possibleOpcodeStrings.cbegin());
                }
            }

            for (const auto& opcode : newlyDisambiguatedOpcodes)
            {
                opcodeToPossibleOpcodeStringsAmbiguous.erase(opcode);
            }

            for (const auto& opcode : newlyDisambiguatedOpcodes)
            {
                std::string opcodeSring = m_opcodeToOpcodeStringUnambiguous.at(opcode);
                for (auto& elem : opcodeToPossibleOpcodeStringsAmbiguous)
                {
                    OpcodeStringSet& possibleOpcodeStrings = elem.second;
                    possibleOpcodeStrings.erase(opcodeSring);
                }
            }

            if (newlyDisambiguatedOpcodes.empty())
            {
                throw std::runtime_error("Failed to disambiguate possible opcodes.");
            }
        }
    }
};

class ProgramExecutor
{
public:
    ProgramExecutor(std::vector<Instruction> instructions, OpcodeToOpcodeString opcodeToOpcodeString)
        : m_instructions{std::move(instructions)}
        , m_opcodeToOpcodeString{std::move(opcodeToOpcodeString)}
        , m_registers{0, 0, 0, 0}
    {

    }

    void execute()
    {
        for (const auto& instruction : m_instructions)
        {
            InstructionEvaluator instructionEvalutator{instruction, std::move(m_registers)};

            std::string opcodeString = m_opcodeToOpcodeString.at(instruction.opcode);
            m_registers = instructionEvalutator.evaluateAs(opcodeString);
        }
    }

    int getValueInFirstRegister() const
    {
        return m_registers.front();
    }

private:
    std::vector<Instruction> m_instructions;
    OpcodeToOpcodeString m_opcodeToOpcodeString;

    Registers m_registers;
};

std::vector<int> parseAllNumbers(std::string line)
{
    std::vector<int> allNumbers;

    std::regex numbersRegex("([0-9]+)");
    std::smatch matchResults;

    while (regex_search(line, matchResults, numbersRegex))
    {
        allNumbers.push_back(std::stoi(matchResults.str()));
        line = matchResults.suffix();
    }

    return allNumbers;
}

Instruction parseInstruction(const std::string& line)
{
    std::vector<int> numbers = parseAllNumbers(line);

    if (numbers.size() != 4)
    {
        throw std::runtime_error("Unable to parse instruction: " + line);
    }

    return Instruction{numbers[0], numbers[1], numbers[2], numbers[3]};
}

Registers parseRegisters(const std::string& line)
{
    Registers registers{};

    std::vector<int> numbers = parseAllNumbers(line);
    if (numbers.size() != 4)
    {
        throw std::runtime_error("Unable to parse registers: " + line);
    }

    std::copy(numbers.cbegin(), numbers.cend(), registers.begin());

    return registers;
}

std::vector<CapturedSample> parseCapturedSamples(const std::vector<std::string>& opcodeManualLines)
{
    std::vector<CapturedSample> capturedSamples;

    size_t lineIndex = 0;
    while (true)
    {
        if (opcodeManualLines[lineIndex].find("Before") == std::string::npos)
        {
            break;
        }

        Registers before = parseRegisters(opcodeManualLines[lineIndex]);
        Instruction instruction = parseInstruction(opcodeManualLines[lineIndex + 1]);
        Registers after = parseRegisters(opcodeManualLines[lineIndex + 2]);

        capturedSamples.emplace_back(std::move(before), std::move(instruction), std::move(after));

        lineIndex += 4;
    }

    return capturedSamples;

}

std::vector<Instruction> parseProgramInstructions(const std::vector<std::string>& opcodeManualLines)
{
    std::vector<Instruction> programInstructions;

    size_t lineIndex = 2;
    for (; lineIndex < opcodeManualLines.size(); ++lineIndex)
    {
        if (opcodeManualLines[lineIndex].empty() && opcodeManualLines[lineIndex - 1].empty() && opcodeManualLines[lineIndex - 2].empty())
        {
            ++lineIndex;
            break;
        }
    }

    for (; lineIndex < opcodeManualLines.size(); ++lineIndex)
    {
        Instruction instruction = parseInstruction(opcodeManualLines[lineIndex]);
        programInstructions.push_back(std::move(instruction));
    }

    return programInstructions;
}

unsigned numSamplesBehaveMoreThanThreeOpcodes(const std::vector<std::string>& opcodeManualLines)
{
    std::vector<CapturedSample> capturedSamples = parseCapturedSamples(opcodeManualLines);

    OpcodeDiscovery opcodeDiscovery{capturedSamples};
    opcodeDiscovery.discover();

    return opcodeDiscovery.getNumSamplesBehaveMoreThanThreeOpcodes();
}

unsigned valueInFirstRegisterAfterProgramFinishes(const std::vector<std::string>& opcodeManualLines)
{
    std::vector<CapturedSample> capturedSamples = parseCapturedSamples(opcodeManualLines);

    OpcodeDiscovery opcodeDiscovery{capturedSamples};
    opcodeDiscovery.discover();

    std::vector<Instruction> programInstructions = parseProgramInstructions(opcodeManualLines);

    ProgramExecutor programExecutor{std::move(programInstructions), opcodeDiscovery.getOpcodeToOpcodeString()};
    programExecutor.execute();

    return programExecutor.getValueInFirstRegister();

}

}
