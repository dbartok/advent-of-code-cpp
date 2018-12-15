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

    TEST_METHOD(numRecipesBeforeSequenceAppears_SimpleTests)
    {
        Assert::AreEqual(9u, AoC::numRecipesBeforeSequenceAppears("51589"));
        Assert::AreEqual(5u, AoC::numRecipesBeforeSequenceAppears("01245"));
        Assert::AreEqual(18u, AoC::numRecipesBeforeSequenceAppears("92510"));
        Assert::AreEqual(2018u, AoC::numRecipesBeforeSequenceAppears("59414"));
    }

};
