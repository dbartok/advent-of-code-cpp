#include "Day24-PlanetOfDiscord.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day24PlanetOfDiscord)
{
public:

    TEST_METHOD(biodiversityRatingOfFirstDuplicateLayout_SimpleTests)
    {
        std::vector<std::string> initialLayout =
        {
            "....#",
            "#..#.",
            "#..##",
            "..#..",
            "#...."
        };

        Assert::AreEqual(2129920, AoC::biodiversityRatingOfFirstDuplicateLayout(initialLayout));
    }

};
