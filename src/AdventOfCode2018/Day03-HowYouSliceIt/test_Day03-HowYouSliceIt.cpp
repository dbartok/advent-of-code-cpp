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
        Assert::AreEqual(4u, AoC::numOverlappingSquares(m_lines));
    }

    TEST_METHOD(findSingleNonOperlappingSquare_SimpleTests)
    {
        Assert::AreEqual(3u, AoC::findSingleNonOperlappingSquare(m_lines));
    }

private:
    std::vector<std::string> m_lines
    {
        "#1 @ 1,3: 4x4",
        "#2 @ 3,1: 4x4",
        "#3 @ 5,5: 2x2"
    };
};
