#pragma once

#include <vector>
#include <string>
#include <sstream>

template <typename T>
void parseStringIntoVector(const std::string& inputString, std::vector<T>& outputVector)
{
    std::istringstream inputStringStream(inputString);

    T element;
    while (inputStringStream >> element)
    {
        outputVector.push_back(std::move(element));
    }
}
