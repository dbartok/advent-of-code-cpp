#include "NodeDescriptor.h"

#include <boost/algorithm/string.hpp>

#include <algorithm>

namespace AdventOfCode
{

NodeDescriptor::NodeDescriptor(std::string parentName, std::vector<std::string> childrenNames)
    : parentName(std::move(parentName))
    , childrenNames(std::move(childrenNames))
{

}

NodeDescriptor NodeDescriptor::fromString(const std::string& descriptorString)
{
    std::vector<std::string> tokens;
    boost::split(tokens, descriptorString, boost::is_any_of("\t, "));

    if (tokens.size() < 2)
    {
        throw(std::runtime_error("The descriptorString has too few tokens."));
    }

    std::string parentName = tokens[0];
    std::vector<std::string> childrenNames;

    if (tokens.size() >= 3)
    {
        std::for_each(tokens.begin() + 3, tokens.end(), [&childrenNames](std::string str) { childrenNames.push_back(std::move(str)); });
    }

    return NodeDescriptor(parentName, childrenNames);
}

}
