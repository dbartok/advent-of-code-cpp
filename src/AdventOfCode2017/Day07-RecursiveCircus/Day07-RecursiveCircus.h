#pragma once

#include "NodeDescriptor.h"

#include <unordered_map>
#include <vector>

namespace AdventOfCode
{

std::string nameOfBottomProgram(const std::vector<NodeDescriptor>& nodeDescriptors);
int correctWeightOfWrongWeight(const std::vector<NodeDescriptor>& nodeDescriptors);

}
