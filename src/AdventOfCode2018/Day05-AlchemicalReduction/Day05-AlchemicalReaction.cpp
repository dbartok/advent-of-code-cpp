#include "Day05-AlchemicalReaction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <cmath>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const unsigned REDUCIBLE_DISTANCE = std::abs('A' - 'a');
}

namespace AdventOfCode
{

bool reducible(char a, char b)
{
    return std::abs(a - b) == REDUCIBLE_DISTANCE;
}

std::string getReducedPolymerString(const std::string& polymerString)
{
    if (polymerString.length() <= 1)
    {
        return polymerString;
    }

    std::string tail = polymerString.substr(1);
    std::string reducedTail = getReducedPolymerString(tail);

    if (reducible(polymerString.front(), reducedTail.front()))
    {
        return reducedTail.substr(1);
    }
    else
    {
        return polymerString.front() + reducedTail;
    }
}

unsigned getReducedPolymerStringLength(const std::string& polymerString)
{
    std::string reducedPolymerString = getReducedPolymerString(polymerString);

    return reducedPolymerString.length();
}

}
