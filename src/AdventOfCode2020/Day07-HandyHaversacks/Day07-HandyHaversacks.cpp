#include "Day07-HandyHaversacks.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cassert>
#include <numeric>
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
    void addRelationship(const BagColor& containerBagColor, const BagColor& containedBagColor, int containedQuantity)
    {
        m_bagColorToContainerBags[containedBagColor].emplace_back(std::move(containerBagColor), containedQuantity);
        m_bagColorToContainedBags[containerBagColor].emplace_back(std::move(containedBagColor), containedQuantity);
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

    int getNumTotalBagsContainedInsideShinyGold() const
    {
        std::unordered_map<BagColor, int> bagColorToTotalQuantity;
        bagColorToTotalQuantity[SHINY_GOLD] = 1;

        std::vector<BagColor> topologicalOrder = getTopologicalOrder();

        for (const auto& currentBagColor : topologicalOrder)
        {
            const auto containedBagsIter = m_bagColorToContainedBags.find(currentBagColor);
            if (containedBagsIter == m_bagColorToContainedBags.cend())
            {
                continue;
            }

            int currentBagTotalQuantity = bagColorToTotalQuantity.at(currentBagColor);

            for (const auto& containedBag : containedBagsIter->second)
            {
                bagColorToTotalQuantity[containedBag.color] += (currentBagTotalQuantity * containedBag.quantity);
            }
        }


        int numTotalBags = std::accumulate(bagColorToTotalQuantity.cbegin(), bagColorToTotalQuantity.cend(), 0, [](int acc, const auto& element)
                                           {
                                               return acc + element.second;
                                           });
        return numTotalBags - 1;
    }


private:
    BagColorToQuantifiedBags m_bagColorToContainerBags;
    BagColorToQuantifiedBags m_bagColorToContainedBags;

    std::vector<BagColor> getTopologicalOrder() const
    {
        std::vector<BagColor> finishedOrder;
        std::unordered_set<BagColor> visited;
        visited.insert(SHINY_GOLD);

        getReverseTopologicalOrderRecursive(finishedOrder, visited, SHINY_GOLD);

        std::reverse(finishedOrder.begin(), finishedOrder.end());
        assert(finishedOrder.front() == SHINY_GOLD);

        return finishedOrder;
    }

    void getReverseTopologicalOrderRecursive(std::vector<BagColor>& finishedOrder, std::unordered_set<BagColor>& visited, const BagColor& currentBagColor) const
    {
        const auto containedBagsIter = m_bagColorToContainedBags.find(currentBagColor);
        if (containedBagsIter != m_bagColorToContainedBags.cend())
        {
            for (const auto& containedBag : containedBagsIter->second)
            {
                bool wasInserted = visited.insert(containedBag.color).second;
                if (wasInserted)
                {
                    getReverseTopologicalOrderRecursive(finishedOrder, visited, containedBag.color);
                }
            }
        }

        finishedOrder.push_back(currentBagColor);
    }
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

int numTotalBagsContainedInsideShinyGold(const std::vector<std::string>& lines)
{
    BagTypesAnalyzer bagTypesAnalyzer = initializeBagTypesAnalyzer(lines);
    return bagTypesAnalyzer.getNumTotalBagsContainedInsideShinyGold();
}

}
