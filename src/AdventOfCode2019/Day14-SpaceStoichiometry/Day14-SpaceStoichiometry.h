#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

long long minOreRequiredToProduceFuel(const std::vector<std::string>& reactionLines);
long long maxProducableFuel(const std::vector<std::string>& reactionLines);

}
