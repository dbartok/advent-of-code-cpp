#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
//TODO: Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned valueAfterLastWritten(unsigned numStepsForward);
unsigned valueAfterZeroAfterManyInsertions(unsigned numStepsForward) noexcept;

}
