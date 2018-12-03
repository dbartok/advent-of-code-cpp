#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned sumOfDigitsMatchNextDigit(const std::string& digitsString);
unsigned sumOfDigitsMatchHalfwayRoundDigit(const std::string& digitsString);

}
