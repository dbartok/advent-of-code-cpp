#include "Day07-HandyHaversacks.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <queue>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using BagColor = std::string;
struct QuantifiedBag;
using QuantifiedBags = std::vector<QuantifiedBag>;
using BagColorToQuantifiedBags = std::unordered_map<BagColor, QuantifiedBags>;

const BagColor SHINY_GOLD = "shiny gold";

struct QuantifiedBag
{
    BagColor color;
    int quantity;

    QuantifiedBag(BagColor color, int quantity)
        : color{std::move(color)}
        , quantity{quantity}
    {

    }

};

class BagTypesAnalyzer
{
public:
    void addRelationship(BagColor containerBagColor, BagColor containedBagColor, int containedQuantity)
    {
        m_bagColorToContainerBags[std::move(containedBagColor)].emplace_back(std::move(containerBagColor), containedQuantity);
    }

    int getNumBagColorsThatCanContainShinyGold() const
    {
        std::queue<BagColor> queue;
        queue.push(SHINY_GOLD);

        std::unordered_set<BagColor> visited;
        visited.insert(SHINY_GOLD);

        while (!queue.empty())
        {
            BagColor currentColor = queue.front();
            queue.pop();

            const auto containerBagsIter = m_bagColorToContainerBags.find(currentColor);
            if (containerBagsIter == m_bagColorToContainerBags.cend())
            {
                continue;
            }

            for (const auto& containerBag : containerBagsIter->second)
            {
                bool wasInserted = visited.insert(containerBag.color).second;
                if (wasInserted)
                {
                    queue.push(containerBag.color);
                }
            }
        }

        return visited.size() - 1;
    }

private:
    BagColorToQuantifiedBags m_bagColorToContainerBags;
};

void processContainedBagsSubstring(BagTypesAnalyzer& bagTypesAnalyzer, const std::string& containerBagColor, const std::string& containedBagsString)
{
    std::vector<std::string> containedBagsSubstrings;
    boost::split(containedBagsSubstrings, containedBagsString, boost::is_any_of(","));

    for (const auto& containedBagsSubstring : containedBagsSubstrings)
    {
        std::regex containedBagsSubstringRegex(R"((?:\s?)(\d+) ([\w ]+) bag(?:s?))");
        std::smatch containedBagsSubstringMatches;
        std::regex_search(containedBagsSubstring, containedBagsSubstringMatches, containedBagsSubstringRegex);

        if (containedBagsSubstringMatches.size() == 3)
        {
            bagTypesAnalyzer.addRelationship(containerBagColor, containedBagsSubstringMatches[2], std::stoi(containedBagsSubstringMatches[1]));
        }
    }

}

BagTypesAnalyzer initializeBagTypesAnalyzer(const std::vector<std::string>& lines)
{
    BagTypesAnalyzer bagTypesAnalyzer;

    for (const auto& line : lines)
    {
        std::regex lineRegex(R"(([\w ]+) bags contain (.+)\.)");
        std::smatch lineMatches;
        std::regex_search(line, lineMatches, lineRegex);

        processContainedBagsSubstring(bagTypesAnalyzer, lineMatches[1], lineMatches[2]);
    }

    return bagTypesAnalyzer;
}

int numBagColorsThatCanContainShinyGold(const std::vector<std::string>& lines)
{
    BagTypesAnalyzer bagTypesAnalyzer = initializeBagTypesAnalyzer(lines);
    return bagTypesAnalyzer.getNumBagColorsThatCanContainShinyGold();
}

}
