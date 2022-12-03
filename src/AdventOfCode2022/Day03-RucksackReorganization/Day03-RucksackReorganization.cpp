#include "Day03-RucksackReorganization.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <numeric>
#include <iterator>
#include <stdexcept>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

int LOWERCASE_STARTING_PRIORITY = 1;
int UPPERCASE_STARTING_PRIORITY = 27;

}

namespace AdventOfCode
{
namespace Year2022
{
namespace Day03
{

char getSharedItemBetweenCompartments(std::string firstCompartment, std::string secondCompartment)
{
    std::sort(firstCompartment.begin(), firstCompartment.end());
    std::sort(secondCompartment.begin(), secondCompartment.end());

    std::string intersection;
    std::set_intersection(firstCompartment.begin(), firstCompartment.end(),
                          secondCompartment.begin(), secondCompartment.end(),
                          std::back_inserter(intersection));

    auto uniqueRangeEndIter = std::unique(intersection.begin(), intersection.end());
    intersection.erase(uniqueRangeEndIter, intersection.end());

    if (intersection.size() != 1)
    {
        throw std::runtime_error("Invalid amount of intersecting elements: "  + std::to_string(intersection.size()));
    }

    return intersection.front();
}

char getSharedItem(const std::string& rucksackContentsLine)
{
    const std::string firstCompartment = rucksackContentsLine.substr(0, rucksackContentsLine.size() / 2);
    const std::string secondCompartment = rucksackContentsLine.substr(rucksackContentsLine.size() / 2);

    return getSharedItemBetweenCompartments(firstCompartment, secondCompartment);
}

int getPriority(char item)
{
    if (islower(item))
    {
        return item - 'a' + LOWERCASE_STARTING_PRIORITY;
    }
    else if (isupper(item))
    {
        return item - 'A' + UPPERCASE_STARTING_PRIORITY;
    }

    throw std::runtime_error("Invalid item: " + std::string{item});
}

int getSharedItemPriority(const std::string& rucksackContentsLine)
{
    char sharedItem = getSharedItem(rucksackContentsLine);
    return getPriority(sharedItem);
}

int sumOfSharedItemPriorities(const std::vector<std::string>& rucksackContentsLines)
{
    return std::accumulate(rucksackContentsLines.cbegin(), rucksackContentsLines.cend(), 0, [](const auto& acc, const auto& line) { return acc + getSharedItemPriority(line); });
}

}
}
}