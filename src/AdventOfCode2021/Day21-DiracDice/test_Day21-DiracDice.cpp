#include "Day21-DiracDice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace CurrentDay = AdventOfCode::Year2021::Day21;

TEST_CLASS(Day21DiracDice)
{
public:

    TEST_METHOD(losingPlayerScoreTimesNumDiceRollsDeterministicDice_SimpleTests)
    {
        const std::vector<std::string> lines =
        {
            "Player 1 starting position : 4",
            "Player 2 starting position : 8"
        };

        Assert::AreEqual(739785, CurrentDay::losingPlayerScoreTimesNumDiceRollsDeterministicDice(lines));
    }

};
