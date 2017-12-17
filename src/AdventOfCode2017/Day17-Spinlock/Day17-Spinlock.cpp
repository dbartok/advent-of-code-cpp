#include "Day17-Spinlock.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned NUM_INSERTIONS = 2017;

}

namespace AdventOfCode
{

unsigned valueAfterLastWritten(unsigned numStepsForward)
{
    std::vector<unsigned> spinLockValues{0};
    size_t currentPos = 0;

    for (unsigned insertedValue = 1; insertedValue <= NUM_INSERTIONS; ++insertedValue)
    {
        currentPos = (currentPos + numStepsForward) % spinLockValues.size();
        ++currentPos;
        spinLockValues.insert(spinLockValues.begin() + currentPos, insertedValue);
    }

    return spinLockValues[(currentPos + 1) % spinLockValues.size()];
}

}
