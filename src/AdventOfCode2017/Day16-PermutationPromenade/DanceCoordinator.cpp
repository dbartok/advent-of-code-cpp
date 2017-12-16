#include "DanceCoordinator.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
#include <numeric>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

DanceCoordinator::DanceCoordinator(std::vector<DanceMove::SharedPtr> danceMoves, unsigned numRepetitions, unsigned numParticipants)
    : m_danceMoves{std::move(danceMoves)}
    , m_numRepetitions{numRepetitions}
    , m_nameOrderString(numParticipants, ' ') // Fill constructor
{
    const char firstProgramName = 'a';
    assert(static_cast<int>(numParticipants) <= 'z' - firstProgramName);

    std::iota(m_nameOrderString.begin(), m_nameOrderString.end(), firstProgramName);
}

void DanceCoordinator::executeMoves()
{
    for (unsigned i = 0; i < m_numRepetitions; ++i)
    {
        for (const auto& danceMove : m_danceMoves)
        {
            danceMove->execute(m_nameOrderString);
        }
    }
}

const std::string& DanceCoordinator::getNameOrderString() const noexcept
{
    return m_nameOrderString;
}

}
