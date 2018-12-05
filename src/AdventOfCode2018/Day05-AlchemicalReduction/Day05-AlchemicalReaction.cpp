#include "Day05-AlchemicalReaction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <cmath>
#include <list>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const int REDUCIBLE_DISTANCE = std::abs('A' - 'a');
}

namespace AdventOfCode
{

bool isReducible(char a, char b)
{
    return std::abs(a - b) == REDUCIBLE_DISTANCE;
}

std::string getReducedPolymerString(const std::string& polymerString)
{
    std::list<char> charList{polymerString.cbegin(), polymerString.cend()};

    auto currentIter = charList.begin();
    while (currentIter != charList.end())
    {
        auto previousIter = currentIter;
        ++currentIter;

        if (currentIter == charList.end())
        {
            break;
        }

        if (isReducible(*previousIter, *currentIter))
        {
            currentIter = charList.erase(previousIter);
            currentIter = charList.erase(currentIter);
            if (currentIter != charList.begin())
            {
                --currentIter;
            }
        }
    }

    return std::string{charList.cbegin(), charList.cend()};
}

unsigned getReducedPolymerStringLength(const std::string& polymerString)
{
    std::string reducedPolymerString = getReducedPolymerString(polymerString);

    return reducedPolymerString.length();
}

}
