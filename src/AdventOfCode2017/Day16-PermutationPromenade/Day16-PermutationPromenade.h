#pragma once

#include "DanceMove.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned DEFAULT_NUM_PARTICIPANTS = 16;

}

namespace AdventOfCode
{

std::string orderAfterDanceMoves(const std::vector<std::string>& danceMoveStrings, unsigned numParticipants = DEFAULT_NUM_PARTICIPANTS);

}
