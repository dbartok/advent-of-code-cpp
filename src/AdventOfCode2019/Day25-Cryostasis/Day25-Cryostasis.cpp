#include "Day25-Cryostasis.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <array>
#include <cassert>
#include <regex>
#include <string>
#include <sstream>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class IntcodeTextAdventure
{
public:
    IntcodeTextAdventure(IntcodeInterpreter interpreter, std::istream& in)
        : m_interpreter{std::move(interpreter)}
        , m_in{in}
    {

    }

    void play()
    {
        while (m_interpreter.getExecutionState() != IntcodeProgramExecutionState::TERMINATED)
        {
            m_interpreter.clearOutputs();
            m_interpreter.execute();
            feedInput();
        }
    }

    std::string getPasscode()
    {
        std::ostringstream oss;
        for (auto c : m_interpreter.getOutputs())
        {
            oss << static_cast<char>(c);
        }

        std::string outputString = oss.str();
        std::smatch match;
        std::regex numbersRegex("[0-9]+");
        std::regex_search(outputString, match, numbersRegex);

        assert(match.size() == 1);
        return match[0];
    }

private:
    IntcodeInterpreter m_interpreter;
    std::istream& m_in;

    void feedInput()
    {
        std::string inputString;
        std::getline(m_in, inputString);
        inputString += '\n';

        for (auto c : inputString)
        {
            m_interpreter.addInput(c);
        }
    }
};

std::array<std::string, 23> INPUTS =
{
    "south",
    "take mouse",
    "north",
    "west",
    "north",
    "north",
    "north",
    "west",
    "take semiconductor",
    "east",
    "south",
    "west",
    "south",
    "take hypercube",
    "north",
    "east",
    "south",
    "west",
    "take antenna",
    "west",
    "south",
    "south",
    "south",
};

std::string passwordForMainAirlock(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    std::string inputString;
    for (auto& instruction : INPUTS)
    {
        inputString += (instruction + '\n');
    }

    std::istringstream iss(inputString);

    IntcodeTextAdventure textAdventure{interpreter, iss};

    textAdventure.play();

    return textAdventure.getPasscode();
}

}
