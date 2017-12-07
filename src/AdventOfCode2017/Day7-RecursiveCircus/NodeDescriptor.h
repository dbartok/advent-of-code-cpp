#pragma once

#include <vector>
#include <string>

namespace AdventOfCode
{

struct NodeDescriptor
{
    std::string parentName;
    std::vector<std::string> childrenNames;

    NodeDescriptor(std::string parentName, std::vector<std::string> childrenNames);
    static NodeDescriptor fromString(const std::string& descriptorString);
};

}
