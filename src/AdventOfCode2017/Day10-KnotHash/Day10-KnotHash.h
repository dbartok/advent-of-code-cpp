#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned firstTwoMultipliedAfterKnotting(size_t listSize, const std::vector<unsigned>& lengths);
std::string knotHashDenseString(const std::string& stringToHash);

}
