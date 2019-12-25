#pragma once

#include <AdventOfCode2019/Day05-SunnyWithAsteroids/IntcodeInterpreter.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class IntcodeTextAdventure
{
public:
    IntcodeTextAdventure(IntcodeInterpreter interpreter, std::istream& in);

    void play();

    std::string getPasscode();

private:
    IntcodeInterpreter m_interpreter;
    std::istream& m_in;

    void feedInput();
};

}
