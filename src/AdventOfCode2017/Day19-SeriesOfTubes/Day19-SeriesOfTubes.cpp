#include "Day19-SeriesOfTubes.h"

#include "PipeDiagramTraverser.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

std::string lettersVisited(const std::vector<std::string>& diagramLines)
{
    PipeDiagramTraverser pipeDiagramTraverser{diagramLines};
    pipeDiagramTraverser.traverse();
    return pipeDiagramTraverser.getLettersVisitedSequence();
}

}
