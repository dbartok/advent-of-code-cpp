#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
using Register = long long;

// Hand optimized version of the original assembly code
unsigned optimizedFinalValueOfH() noexcept;

}
