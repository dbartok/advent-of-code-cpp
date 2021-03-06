#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned totalNumberOfDirectAndIndirectOrbits(const std::vector<std::string>& orbitLines);
unsigned minimumNumberOfOrbitalTransfers(const std::vector<std::string>& orbitLines);

}
