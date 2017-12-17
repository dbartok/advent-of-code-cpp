#include "Day08-LikeRegisters.h"

#include "Processor.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int largestRegisterAfterCompletion(const std::vector<Instruction>& instructions)
{
    Processor processor{instructions};
    processor.run();
    return processor.largestRegister();
}

int largestRegisterDuringExecution(const std::vector<Instruction>& instructions)
{
    Processor processor{instructions};
    processor.run();
    return processor.globalMaxValueOfRegisters();
}

}
