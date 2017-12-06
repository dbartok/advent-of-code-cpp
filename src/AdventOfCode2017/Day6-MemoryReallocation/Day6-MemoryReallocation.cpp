#include "Day6-MemoryReallocation.h"

#include <boost/functional/hash/hash.hpp>

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <algorithm>

namespace AdventOfCode
{

template<typename T>
unsigned wrappingForwardDistance(const T& from, const T& to, size_t size)
{
    if (to > from)
    {
        return to - from;
    }
    else
    {
        return size - (from - to);
    }
}

void redistribute(MemoryBanks& banks)
{
    auto maxElementIter = std::max_element(banks.begin(), banks.end());
    unsigned maxValue = *maxElementIter;
    *maxElementIter = 0;

    for (auto iter = banks.begin(); iter != banks.end(); ++iter)
    {
        unsigned& bank = *iter;
        unsigned distance = wrappingForwardDistance(maxElementIter, iter, banks.size());

        unsigned increments = 0;

        if (maxValue >= distance)
        {
            // First increment
            ++increments;

            // All other subsequent increments
            unsigned remainingValue = maxValue - distance;
            increments += (remainingValue / banks.size());
        }

        bank += increments;
    }
}

unsigned numUniqueRedistributions(MemoryBanks banks)
{
    if (banks.empty())
    {
        return 1;
    }

    std::unordered_set<MemoryBanks, boost::hash<MemoryBanks>> banksOccurences;

    while (true)
    {
        bool insertionTookPlace;
        std::tie(std::ignore, insertionTookPlace) = banksOccurences.insert(banks);

        if (!insertionTookPlace)
        {
            break;
        }

        redistribute(banks);
    }

    return banksOccurences.size();
}

}


int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    AoC::MemoryBanks inputBanks;

    unsigned inputBank;
    while (fileIn >> inputBank)
    {
        inputBanks.push_back(inputBank);
    }

    std::cout << "First part: " << AoC::numUniqueRedistributions(inputBanks) << std::endl;
}
