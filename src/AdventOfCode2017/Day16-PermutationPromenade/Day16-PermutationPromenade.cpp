#include "Day16-PermutationPromenade.h"

#include "DanceCoordinator.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::vector<DanceMove::SharedPtr> danceMoveSharedPtrsFromStrings (const std::vector<std::string>& danceMoveStrings)
{
    std::vector<DanceMove::SharedPtr> danceMoveSharedPtrs;

    std::transform(danceMoveStrings.cbegin(), danceMoveStrings.cend(), std::back_inserter(danceMoveSharedPtrs), [](const std::string& danceMoveString)
                   {
                       return DanceMove::sharedPtrFromString(danceMoveString);
                   });

    return danceMoveSharedPtrs;
}

std::string orderAfterDanceMoves(const std::vector<std::string>& danceMoveStrings, unsigned numParticipants)
{
    std::vector<DanceMove::SharedPtr> danceMoves = danceMoveSharedPtrsFromStrings(danceMoveStrings);
    DanceCoordinator danceCoordinator{std::move(danceMoves), numParticipants};
    danceCoordinator.executeMoves();
    return danceCoordinator.currentOrderString();
}

}
