#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string firstEightDigitsOfFinalOutput(const std::string& signalString);
std::string messageInFinalOutputForRealSignal(const std::string& signalString);

}
