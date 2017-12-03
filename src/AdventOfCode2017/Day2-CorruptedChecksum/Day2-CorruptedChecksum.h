#pragma once

#include <vector>

namespace AdventOfCode
{

using RowOfNumbers = std::vector<unsigned>;
using RowsOfNumbers = std::vector<RowOfNumbers>;

unsigned checkSumDiffSmallestLargest(const RowsOfNumbers& rows);
unsigned sumEvenlyDivisbleValues(const RowsOfNumbers& rows);

}
