#include "Day13-PacketScanners.h"

#include "ScannerCorridor.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

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
