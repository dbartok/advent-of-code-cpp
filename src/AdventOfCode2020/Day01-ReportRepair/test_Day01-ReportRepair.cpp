#include "Day01-ReportRepair.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day01ReportRepair)
{
public:

    TEST_METHOD(productOfTwoEntriesWithGivenSum_SimpleTests)
    {
        Assert::AreEqual(514579, AoC::productOfTwoEntriesWithGivenSum({1721, 979, 366, 299, 675, 1456}));
    }

};
