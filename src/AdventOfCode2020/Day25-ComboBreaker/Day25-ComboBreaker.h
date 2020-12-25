#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <cstdint>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

int64_t encryptionKeyForHandshake(int64_t cardPublicKey, int64_t doorPublicKey);

}
