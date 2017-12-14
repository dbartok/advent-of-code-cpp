#pragma once

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct NodeDescriptor
{
    std::string name;
    int weight;
    std::vector<std::string> childrenNames;

    NodeDescriptor(std::string name, int weight, std::vector<std::string> childrenNames);

    static NodeDescriptor fromString(const std::string& nodeDescriptorString);
};

}
