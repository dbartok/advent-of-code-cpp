#include "Day15-RambunctiousRecitation.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day15RambunctiousRecitation)
{
public:

    TEST_METHOD(nthNumberSpoken_SimpleTests)
    {
        Assert::AreEqual(436, AoC::nthNumberSpoken({0, 3, 6}));
        Assert::AreEqual(1, AoC::nthNumberSpoken({1, 3, 2}));
        Assert::AreEqual(10, AoC::nthNumberSpoken({2, 1, 3}));
        Assert::AreEqual(27, AoC::nthNumberSpoken({1, 2, 3}));
        Assert::AreEqual(78, AoC::nthNumberSpoken({2, 3, 1}));
        Assert::AreEqual(438, AoC::nthNumberSpoken({3, 2, 1}));
        Assert::AreEqual(1836, AoC::nthNumberSpoken({3, 1, 2}));
    }

};
