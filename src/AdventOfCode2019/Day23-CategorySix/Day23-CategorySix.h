#pragma once

#include "IntcodeComputerNetwork.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

IntcodeNumberType secondPartOfFirstPacketAtSpecialAddress(const std::vector<IntcodeNumberType>& intcodeProgram);
IntcodeNumberType secondPartOfFirstDuplicateMessageByNat(const std::vector<IntcodeNumberType>& intcodeProgram);

}
