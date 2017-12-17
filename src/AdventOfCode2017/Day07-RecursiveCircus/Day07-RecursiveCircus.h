#pragma once

#include "NodeDescriptor.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <unordered_map>
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string nameOfBottomProgram(const std::vector<NodeDescriptor>& nodeDescriptors);
int correctWeightOfWrongWeight(const std::vector<NodeDescriptor>& nodeDescriptors);

}
