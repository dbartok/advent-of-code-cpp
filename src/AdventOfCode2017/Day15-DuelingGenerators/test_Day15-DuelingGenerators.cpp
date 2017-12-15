#include "Day15-DuelingGenerators.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day15DuelingGenerators)
{
public:

    TEST_METHOD(judgeFinalCount_SimpleTests)
    {
        Assert::AreEqual(588u, AoC::judgeFinalCount(65, 8921));
    }

};
