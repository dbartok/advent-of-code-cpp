#include "Day01-Trebuchet.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char* DIGITS = "0123456789";

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day01
{

int getCalibrationValue(const std::string& calibrationDocumentLine)
{
    const size_t firstDigitIndex = calibrationDocumentLine.find_first_of(DIGITS);
    const size_t lastDigitIndex = calibrationDocumentLine.find_last_of(DIGITS);

    std::string calibrationValueString;
    calibrationValueString += calibrationDocumentLine.at(firstDigitIndex);
    calibrationValueString += calibrationDocumentLine.at(lastDigitIndex);

    return std::stoi(calibrationValueString);
}

int sumOfAllCalibrationValues(const std::vector<std::string>& calibrationDocumentLines)
{
    return std::accumulate(calibrationDocumentLines.cbegin(), calibrationDocumentLines.cend(), 0, [](int acc, const std::string& line)
                           {
                               return acc + getCalibrationValue(line);
                           });
}

}
}
}