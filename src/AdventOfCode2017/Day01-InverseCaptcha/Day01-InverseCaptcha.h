#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned sumOfDigitsMatchNextDigit(const std::string& digitsString);
unsigned sumOfDigitsMatchHalfwayRoundDigit(const std::string& digitsString);

}
