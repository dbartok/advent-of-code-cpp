#include "Day17-Spinlock.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const unsigned FIRST_PART_NUM_INSERTIONS = 2017;
const unsigned SECOND_PART_NUM_INSERTIONS = 50'000'000;

}

namespace AdventOfCode
{

unsigned valueAfterLastWritten(unsigned numStepsForward)
{
    std::vector<unsigned> spinLockValues{0};
    size_t currentPos = 0;

    for (unsigned insertedValue = 1; insertedValue <= FIRST_PART_NUM_INSERTIONS; ++insertedValue)
    {
        currentPos = (currentPos + numStepsForward) % spinLockValues.size();
        ++currentPos;
        spinLockValues.insert(spinLockValues.begin() + currentPos, insertedValue);
    }

    return spinLockValues[(currentPos + 1) % spinLockValues.size()];
}

unsigned valueAfterZeroAfterManyInsertions(unsigned numStepsForward) noexcept
{
    size_t spinLockSize = 1;
    size_t currentPos = 0;

    unsigned valueAfterZero = 0;

    for (unsigned insertedValue = 1; insertedValue <= SECOND_PART_NUM_INSERTIONS; ++insertedValue)
    {
        currentPos = (currentPos + numStepsForward) % spinLockSize;
        ++currentPos;

        // The final value after the number 0 is the same as the value after position 0 (at position 1).
        // Therefore, the only insertions that we need to record are the ones happening at position 1.
        // Out of these, only the latest one is relevant.
        if (currentPos == 1)
        {
            valueAfterZero = insertedValue;
        }

        ++spinLockSize;
    }

    return valueAfterZero;
}

}
