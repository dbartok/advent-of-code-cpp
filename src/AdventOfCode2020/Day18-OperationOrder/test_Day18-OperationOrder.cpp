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

    TEST_METHOD(sumOfResultingValuesWithoutPrecedence_SimpleTests)
    {
        Assert::AreEqual(71ll, AoC::sumOfResultingValuesWithoutPrecedence({"1 + 2 * 3 + 4 * 5 + 6"}));
        Assert::AreEqual(51ll, AoC::sumOfResultingValuesWithoutPrecedence({"1 + (2 * 3) + (4 * (5 + 6))"}));
        Assert::AreEqual(26ll, AoC::sumOfResultingValuesWithoutPrecedence({"2 * 3 + (4 * 5)"}));
        Assert::AreEqual(437ll, AoC::sumOfResultingValuesWithoutPrecedence({"5 + (8 * 3 + 9 + 3 * 4 * 3)"}));
        Assert::AreEqual(12240ll, AoC::sumOfResultingValuesWithoutPrecedence({"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"}));
        Assert::AreEqual(13632ll, AoC::sumOfResultingValuesWithoutPrecedence({"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"}));
    }

    TEST_METHOD(sumOfResultingValuesWithPrecedence_SimpleTests)
    {
        Assert::AreEqual(231ll, AoC::sumOfResultingValuesWithPrecedence({"1 + 2 * 3 + 4 * 5 + 6"}));
        Assert::AreEqual(51ll, AoC::sumOfResultingValuesWithPrecedence({"1 + (2 * 3) + (4 * (5 + 6))"}));
        Assert::AreEqual(46ll, AoC::sumOfResultingValuesWithPrecedence({"2 * 3 + (4 * 5)"}));
        Assert::AreEqual(1445ll, AoC::sumOfResultingValuesWithPrecedence({"5 + (8 * 3 + 9 + 3 * 4 * 3)"}));
        Assert::AreEqual(669060ll, AoC::sumOfResultingValuesWithPrecedence({"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))"}));
        Assert::AreEqual(23340ll, AoC::sumOfResultingValuesWithPrecedence({"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"}));
    }

};
