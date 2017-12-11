#pragma once

#include "Instruction.h"

#include <vector>

namespace AdventOfCode
{

int largestRegisterAfterCompletion(const std::vector<Instruction>& instructions);
int largestRegisterDuringExecution(const std::vector<Instruction>& instructions);

}
