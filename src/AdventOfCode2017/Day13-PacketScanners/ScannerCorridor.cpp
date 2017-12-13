#include "ScannerCorridor.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace AdventOfCode
{

ScannerCorridor::ScannerCorridor(RangeToDepthMap rangeToDepthMap)
    : m_rangeToDepthMap{rangeToDepthMap}
{

}

unsigned ScannerCorridor::severityOfWholeTrip() const
{
    unsigned severity = 0;
    for (const auto& depthRangePair : m_rangeToDepthMap)
    {
        unsigned depth;
        unsigned range;
        std::tie(depth, range) = depthRangePair;

        if (range == 0)
        {
            continue;
        }

        if (range == 1)
        {
            severity += depth * range;
            continue;
        }

        // Number of steps the scanner has taken since last being at the top
        unsigned scannerNumStepsFromTop = depth % (2 * (range - 1));

        unsigned scannerPos;

        // Scanner in the first half of its path
        if (scannerNumStepsFromTop <= range - 1)
        {
            scannerPos = scannerNumStepsFromTop;
        }

        // Scanner in the second half of its path
        else
        {
            // Number of steps the scanner has taken since last being at the bottom
            unsigned scannerNumStepsFromBottom = scannerNumStepsFromTop - (range - 1);

            scannerPos = (range - 1) - scannerNumStepsFromBottom;
        }

        if (scannerPos == 0)
        {
            severity += depth * range;
        }
    }

    return severity;
}

ScannerCorridor ScannerCorridor::fromScannerRangeLines(const std::vector<std::string> scannerRangeLines)
{
    RangeToDepthMap rangeToDepthMap;
    for (const auto& scannerRangeLine : scannerRangeLines)
    {
        std::vector<std::string> tokens;
        boost::split(tokens, scannerRangeLine, boost::is_any_of(": "), boost::token_compress_on);

        if (tokens.size() != 2)
        {
            throw std::runtime_error("Each line needs to have exactly 2 tokens.");
        }

        unsigned depth = boost::lexical_cast<unsigned>(tokens[0]);
        unsigned range = boost::lexical_cast<unsigned>(tokens[1]);

        bool insertionTookPlace;
        std::tie(std::ignore, insertionTookPlace) = rangeToDepthMap.insert(std::make_pair(depth, range));
        if (!insertionTookPlace)
        {
            throw std::runtime_error("Same depth appears twice in the input lines.");
        }
    }

    return ScannerCorridor{std::move(rangeToDepthMap)};
}

}
