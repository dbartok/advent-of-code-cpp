#include "Day18-OperationOrder.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day18OperationOrder)
{
public:

    TEST_METHOD(sumOfResultingValues_SimpleTests)
    {
        Assert::AreEqual(71ll, AoC::sumOfResultingValues({"1 + 2 * 3 + 4 * 5 + 6"}));
        Assert::AreEqual(51ll, AoC::sumOfResultingValues({"1 + (2 * 3) + (4 * (5 + 6))"}));
        Assert::AreEqual(26ll, AoC::sumOfResultingValues({"2 * 3 + (4 * 5)"}));
        Assert::AreEqual(437ll, AoC::sumOfResultingValues({"5 + (8 * 3 + 9 + 3 * 4 * 3)"}));
        Assert::AreEqual(12240ll, AoC::sumOfResultingValues({"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"}));
        Assert::AreEqual(13632ll, AoC::sumOfResultingValues({"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"}));
    }

};
