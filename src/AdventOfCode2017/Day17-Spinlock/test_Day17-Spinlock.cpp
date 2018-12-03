#include "Day17-Spinlock.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day17Spinlock)
{
public:

    TEST_METHOD(valueAfterLastWritten_SimpleTests)
    {
        Assert::AreEqual(638u, AoC::valueAfterLastWritten(3u));
    }

    TEST_METHOD(valueAfterZeroAfterManyInsertions_ThreeStepsForward)
    {
        Assert::AreEqual(1222153u, AoC::valueAfterZeroAfterManyInsertions(3u));
    }

};
