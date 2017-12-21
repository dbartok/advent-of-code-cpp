#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned DEFAULT_NUM_ITERATIONS = 5;

}

namespace AdventOfCode
{

unsigned numPixelsOnAfterEnhancements(const std::vector<std::string>& enhancementRuleLines, unsigned numIterations = DEFAULT_NUM_ITERATIONS);

}
