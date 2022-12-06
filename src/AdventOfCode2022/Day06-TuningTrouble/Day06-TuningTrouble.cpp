#include "Day06-TuningTrouble.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <stdexcept>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

size_t MARKER_LENGTH = 4;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day06
{

bool isMarkerEndPosition(const std::string& data, size_t index)
{
    std::string possibleMarker = data.substr(index - (MARKER_LENGTH - 1), MARKER_LENGTH);
    std::unordered_set<char> uniqueCharsInPossibleMarker{possibleMarker.cbegin(), possibleMarker.cend()};

    return possibleMarker.size() == uniqueCharsInPossibleMarker.size();
}

int numCharactersProcessedBeforeStartOfPacketMarker(const std::string& data)
{
    for (size_t index = MARKER_LENGTH - 1; index < data.size(); ++index)
    {
        if (isMarkerEndPosition(data, index))
        {
            return index + 1;
        }
    }

    throw std::runtime_error("Marker not found");
}

}
}
}