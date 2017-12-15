#include "Day15-DuelingGenerators.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned GENERATOR_A_MULTIPLIER = 16807;
const unsigned GENERATOR_B_MULTIPLIER = 48271;

const unsigned GENERATOR_MODULUS = 2'147'483'647;

const unsigned NUM_ROUNDS = 40'000'000;

const unsigned JUDGE_NUM_BINARY_DIGITS_TO_MATCH = 16;

}

namespace AdventOfCode
{

unsigned generatorStartFromLine(const std::string& line)
{
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" "));

    if (tokens.size() != 5)
    {
        throw std::runtime_error("Input line needs to have exactly 5 tokens.");
    }

    return boost::lexical_cast<unsigned>(tokens[4]);
}

unsigned judgeFinalCount(unsigned generatorAStart, unsigned generatorBStart)
{
    const double judgeModulusDouble = (pow(2, JUDGE_NUM_BINARY_DIGITS_TO_MATCH));
    assert(judgeModulusDouble <= std::numeric_limits<unsigned>::max());

    const unsigned judgeModulus = static_cast<unsigned>(judgeModulusDouble);

    uint64_t generatorAPrevValue = generatorAStart;
    uint64_t generatorBPrevValue = generatorBStart;

    unsigned judgeScore = 0;

    for (unsigned i = 0; i < NUM_ROUNDS; ++i)
    {
        const uint64_t generatorACurrValue = (generatorAPrevValue * GENERATOR_A_MULTIPLIER) % GENERATOR_MODULUS;
        const uint64_t generatorBCurrValue = (generatorBPrevValue * GENERATOR_B_MULTIPLIER) % GENERATOR_MODULUS;

        if (generatorACurrValue % judgeModulus == generatorBCurrValue % judgeModulus)
        {
            ++judgeScore;
        }

        generatorAPrevValue = generatorACurrValue;
        generatorBPrevValue = generatorBCurrValue;
    }

    return judgeScore;
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");

    std::string generatorAStartLine;
    std::getline(fileIn, generatorAStartLine);
    const unsigned generatorAStartInput = AoC::generatorStartFromLine(generatorAStartLine);

    std::string generatorBStartLine;
    std::getline(fileIn, generatorBStartLine);
    const unsigned generatorBStartInput = AoC::generatorStartFromLine(generatorBStartLine);

    std::cout << "First part: " << AoC::judgeFinalCount(generatorAStartInput, generatorBStartInput) << std::endl;
}
