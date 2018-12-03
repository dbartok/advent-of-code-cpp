#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

struct NodeDescriptor
{
    std::string name;
    int weight;
    std::vector<std::string> childrenNames;

    NodeDescriptor(std::string name, int weight, std::vector<std::string> childrenNames) noexcept;

    static NodeDescriptor fromString(const std::string& nodeDescriptorString);
};

}
