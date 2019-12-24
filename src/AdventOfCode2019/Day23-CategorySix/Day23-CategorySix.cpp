#include "Day23-CategorySix.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

IntcodeNumberType secondPartOfFirstPacketAtSpecialAddress(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeComputerNetwork network{intcodeProgram};

    network.runUntilFirstNatPacket();

    return network.getSecondPartOfNatPacket();
}

IntcodeNumberType secondPartOfFirstDuplicateMessageByNat(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeComputerNetwork network{intcodeProgram};

    network.runUntilFirstDuplicateNatPacket();

    return network.getSecondPartOfNatPacket();
}

}
