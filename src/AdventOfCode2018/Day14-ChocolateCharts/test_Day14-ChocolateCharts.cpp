#include "Day14-ChocolateCharts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day14ChocolateCharts)
{
public:

    TEST_METHOD(scoresOfTenRecipesAfterSeveralRecipes_SimpleTests)
    {
        Assert::AreEqual(std::string{"5158916779"}, AoC::scoresOfTenRecipesAfterSeveralRecipes(9));
        Assert::AreEqual(std::string{"0124515891"}, AoC::scoresOfTenRecipesAfterSeveralRecipes(5));
        Assert::AreEqual(std::string{"9251071085"}, AoC::scoresOfTenRecipesAfterSeveralRecipes(18));
        Assert::AreEqual(std::string{"5941429882"}, AoC::scoresOfTenRecipesAfterSeveralRecipes(2018));
    }

};
