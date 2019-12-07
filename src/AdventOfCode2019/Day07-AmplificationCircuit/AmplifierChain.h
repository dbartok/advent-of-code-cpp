#pragma once

#include <AdventOfCode2019/Day05-SunnyWithAsteroids/IntcodeInterpreter.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class AmplifierChain
{
public:
    AmplifierChain(const std::vector<int>& intcodeProgram, const std::vector<int>& phaseSettings);

    void execute(int initialInput);
    void executeWithFeedbackLoop(int initialInput);

    int getLastOutput() const;

private:
    std::vector<IntcodeInterpreter> m_amplifiers;
};

}
