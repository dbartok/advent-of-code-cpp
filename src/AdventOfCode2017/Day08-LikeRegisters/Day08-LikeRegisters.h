#pragma once

#include "Instruction.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int largestRegisterAfterCompletion(const std::vector<Instruction>& instructions);
int largestRegisterDuringExecution(const std::vector<Instruction>& instructions);

}
