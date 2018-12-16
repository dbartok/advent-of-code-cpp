#include "Day16-ChronalClassification.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <regex>
#include <array>
#include <memory>
#include <unordered_map>
#include <functional>
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

using AssemblyFunc = std::function<void()>;

class InstructionEvaluator
{
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

    std::vector<std::string> getAllPossibleOpcodeStrings() const
    {
        std::vector<std::string> allPossibleOpcodeStrings;

        std::transform(m_opcodeStringToAssemblyFunc.cbegin(), m_opcodeStringToAssemblyFunc.cend(), std::back_inserter(allPossibleOpcodeStrings),
                       [](const auto& elem)
                       {
                           return elem.first;
                       });

        return allPossibleOpcodeStrings;
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

class OpcodeDiscovery
{
public:
    OpcodeDiscovery(std::vector<CapturedSample> capturedSamples)
        : m_capturedSamples{std::move(capturedSamples)}
    {

    }

    void discover()
    {
        for (const auto& capturedSample : m_capturedSamples)
        {
            InstructionEvaluator instructionEvaluator{capturedSample.instruction, capturedSample.before};
            std::vector<std::string> allPossibleOpcodeStrings = instructionEvaluator.getAllPossibleOpcodeStrings();

            unsigned numValidOpcodesForSample = 0;

            for (const auto& opcodeString : allPossibleOpcodeStrings)
            {
                Registers actualAfter = instructionEvaluator.evaluateAs(opcodeString);

                if (capturedSample.after == actualAfter)
                {
                    ++numValidOpcodesForSample;
                }
            }

            if (numValidOpcodesForSample >= 3)
            {
                ++m_numSamplesBehaveMoreThanThreeOpcodes;
            }
        }
    }

    unsigned numSamplesBehaveMoreThanThreeOpcodes() const
    {
        return m_numSamplesBehaveMoreThanThreeOpcodes;
    }

private:
    std::vector<CapturedSample> m_capturedSamples;

    unsigned m_numSamplesBehaveMoreThanThreeOpcodes = 0;
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


unsigned numSamplesBehaveMoreThanThreeOpcodes(const std::vector<std::string>& opcodeManualLines)
{
    std::vector<CapturedSample> capturedSamples = parseCapturedSamples(opcodeManualLines);

    OpcodeDiscovery opcodeDiscovery{capturedSamples};

    opcodeDiscovery.discover();

    return opcodeDiscovery.numSamplesBehaveMoreThanThreeOpcodes();
}

}
