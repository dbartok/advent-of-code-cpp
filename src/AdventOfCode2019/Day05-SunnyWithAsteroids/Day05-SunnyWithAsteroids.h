#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int diagnosticCodeProducedByProgram(const std::vector<int>& intcodeProgram, const std::vector<int>& inputs);

}
