#pragma once

#include "DanceMove.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class DanceCoordinator
{
public:
    DanceCoordinator(std::vector<DanceMove::SharedPtr> danceMoves, unsigned numRepetitions, unsigned numParticipants);
    void executeMoves();
    std::string currentOrderString() const;

private:
    std::vector<DanceMove::SharedPtr> m_danceMoves;
    unsigned m_numRepetitions;

    std::vector<std::string> m_names;
};

}
