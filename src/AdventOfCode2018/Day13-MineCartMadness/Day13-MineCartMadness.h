#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string positionOfFirstCollision(const std::vector<std::string>& mapLines);
std::string positionOfLastRemainingCart(const std::vector<std::string>& mapLines);

}
