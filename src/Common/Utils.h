#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace AdventOfCode
{
namespace Utils
{

template <typename T>
void splitStringIntoVector(const std::string& inputString, std::vector<T>& outputVector)
{
    std::istringstream inputStringStream(inputString);

    T element;
    while (inputStringStream >> element)
    {
        outputVector.push_back(std::move(element));
    }
}

template <typename Iterator, typename Comparator>
bool allElementsEqual(Iterator first, Iterator last, const Comparator& comparator)
{
    // Zero elemnts
    if (first == last)
    {
        return true;
    }

    // Compare each element with the previous one
    return std::equal(std::next(first), last, first, comparator);
}

template <typename Iterator>
bool allElementsEqual(Iterator first, Iterator last)
{
    return allElementsEqual(first, last, [](const auto& lhs, const auto& rhs)
    {
        return lhs == rhs;
    });
}

}
}
