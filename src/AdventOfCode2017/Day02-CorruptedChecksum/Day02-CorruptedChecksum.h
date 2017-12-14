#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using RowOfNumbers = std::vector<unsigned>;
using RowsOfNumbers = std::vector<RowOfNumbers>;

unsigned checkSumDiffSmallestLargest(const RowsOfNumbers& rows);
unsigned sumEvenlyDivisbleValues(const RowsOfNumbers& rows);

}
