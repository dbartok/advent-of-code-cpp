#include "Day02-CorruptedChecksum.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    AoC::RowsOfNumbers inputRows;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        AoC::RowOfNumbers row;
        AoC::Utils::splitStringIntoTypedVector(lineBuffer, row);
        inputRows.push_back(std::move(row));
    }

    std::cout << "First part: " << AoC::checkSumDiffSmallestLargest(inputRows) << std::endl;
    std::cout << "Second part: " << AoC::sumEvenlyDivisbleValues(inputRows) << std::endl;
}
