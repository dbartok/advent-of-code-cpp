#pragma once

#include "RegisterValueType.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

RegisterValueType valueOfFirstRecoveredFrequency(const std::vector<std::string>& instructionStrings);
unsigned numTimesProgram1Sent(const std::vector<std::string>& instructionStrings);

}
