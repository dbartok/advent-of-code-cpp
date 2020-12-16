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

    TEST_METHOD(nthNumberSpokenSmallN_SimpleTests)
    {
        Assert::AreEqual(436, AoC::nthNumberSpokenSmallN({0, 3, 6}));
        Assert::AreEqual(1, AoC::nthNumberSpokenSmallN({1, 3, 2}));
        Assert::AreEqual(10, AoC::nthNumberSpokenSmallN({2, 1, 3}));
        Assert::AreEqual(27, AoC::nthNumberSpokenSmallN({1, 2, 3}));
        Assert::AreEqual(78, AoC::nthNumberSpokenSmallN({2, 3, 1}));
        Assert::AreEqual(438, AoC::nthNumberSpokenSmallN({3, 2, 1}));
        Assert::AreEqual(1836, AoC::nthNumberSpokenSmallN({3, 1, 2}));
    }

    TEST_METHOD(nthNumberSpokenLargeN_SimpleTests)
    {
        Assert::AreEqual(175594, AoC::nthNumberSpokenLargeN({0, 3, 6}));
        Assert::AreEqual(2578, AoC::nthNumberSpokenLargeN({1, 3, 2}));
        Assert::AreEqual(3544142, AoC::nthNumberSpokenLargeN({2, 1, 3}));
        Assert::AreEqual(261214, AoC::nthNumberSpokenLargeN({1, 2, 3}));
        Assert::AreEqual(6895259, AoC::nthNumberSpokenLargeN({2, 3, 1}));
        Assert::AreEqual(18, AoC::nthNumberSpokenLargeN({3, 2, 1}));
        Assert::AreEqual(362, AoC::nthNumberSpokenLargeN({3, 1, 2}));
    }

};
