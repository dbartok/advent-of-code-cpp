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

}
}
