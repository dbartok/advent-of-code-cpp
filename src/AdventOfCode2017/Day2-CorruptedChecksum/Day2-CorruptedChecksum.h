#pragma once

#include <vector>

using RowOfNumbers = std::vector<unsigned>;
using RowsOfNumbers = std::vector<RowOfNumbers>;

unsigned checkSumDiffSmallestLargest(const RowsOfNumbers& rows);
