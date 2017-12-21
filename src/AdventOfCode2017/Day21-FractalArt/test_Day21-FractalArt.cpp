#include "Day21-FractalArt.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day21FractalArt)
{
public:

    TEST_METHOD(numPixelsOnAfterEnhancements_SimpleTests)
    {
        const std::vector<std::string> enhancementRuleLines =
        {
            "../.# => ##./#../...",
            ".#./..#/### => #..#/..../..../#..#"
        };

        Assert::AreEqual(12u, AoC::numPixelsOnAfterEnhancements(enhancementRuleLines, 2));
    }

};
