#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
unsigned DEFAULT_DISTANCE_THRESHOLD = 1000;
}

namespace AdventOfCode
{

unsigned distanceToFarthestRoom(const std::string& roomLayoutRegex);
unsigned numRoomsWithLargeDistance(const std::string& roomLayoutRegex, unsigned distanceThreshold = DEFAULT_DISTANCE_THRESHOLD);

}
