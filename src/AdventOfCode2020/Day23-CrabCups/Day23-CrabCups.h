#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string cupLabelsStartingFromCupOne(const std::string& initialCupLabellingString, size_t numMixes);
int64_t twoCupLabelsAfterCupOneMultipliedManyCups(const std::string& initialCupLabellingString, size_t numMixes);

}
