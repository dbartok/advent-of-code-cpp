#include "Day21-ChronalConversion.h"

#include "HandOptimizedHalting.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

unsigned lowestValueForFirstRegisterFewestInstructions()
{
    return getTerminatingValuesOfFirstRegister().front();
}

unsigned lowestValueForFirstRegisterMostInstructions()
{
    return getTerminatingValuesOfFirstRegister().back();
}

}
