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
    , m_names(numParticipants) // Size constructor
{
    char programName = 'a';
    assert(static_cast<int>(numParticipants) <= 'z' - programName);

    std::generate(m_names.begin(), m_names.end(), [&programName]() noexcept { return programName++; });
}

void DanceCoordinator::executeMoves()
{
    for (unsigned i = 0; i < m_numRepetitions; ++i)
    {
        for (const auto& danceMove : m_danceMoves)
        {
            danceMove->execute(m_names);
        }
    }
}

std::string DanceCoordinator::currentOrderString() const
{
    return std::accumulate(m_names.cbegin(), m_names.cend(), std::string{""}, [](const std::string& orderString, const std::string name) {return orderString + name; });
}

}
