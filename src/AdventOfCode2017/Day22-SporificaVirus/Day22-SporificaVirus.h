#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned DEFAULT_NUM_ITERATIONS = 10'000;

}

namespace AdventOfCode
{

unsigned numBurstsThatAreInfecting(const std::vector<std::string>& initialVirusPosLines, unsigned numIterations = DEFAULT_NUM_ITERATIONS);

}
