#include "Day03-HowYouSliceIt.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day03HowYouSliceIt)
{
public:

    TEST_METHOD(numOverlappingSquares_SimpleTests)
    {
        std::vector<std::string> lines
        {
            "#1 @ 1,3: 4x4",
            "#2 @ 3,1: 4x4",
            "#3 @ 5,5: 2x2"
        };

        Assert::AreEqual(4u, AoC::numOverlappingSquares(lines));
    }

};
