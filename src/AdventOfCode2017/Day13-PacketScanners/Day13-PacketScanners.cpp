#include "Day13-PacketScanners.h"

#include "ScannerCorridor.h"

#include <fstream>
#include <iostream>

namespace AdventOfCode
{

unsigned severityOfWholeTrip(const std::vector<std::string>& scannerRangeLines)
{
    ScannerCorridor scannerCorrider = ScannerCorridor::fromScannerRangeLines(scannerRangeLines);
    return scannerCorrider.severityOfWholeTrip();
}

unsigned smallestDelayNotToGetCaught(const std::vector<std::string>& scannerRangeLines)
{
    ScannerCorridor scannerCorrider = ScannerCorridor::fromScannerRangeLines(scannerRangeLines);
    return scannerCorrider.smallestDelayNotToGetCaught();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputScannerRangeLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputScannerRangeLines.push_back(lineBuffer);
    }

    std::cout << "First part: " << AoC::severityOfWholeTrip(inputScannerRangeLines) << std::endl;
    std::cout << "Second part: " << AoC::smallestDelayNotToGetCaught(inputScannerRangeLines) << std::endl;
}
