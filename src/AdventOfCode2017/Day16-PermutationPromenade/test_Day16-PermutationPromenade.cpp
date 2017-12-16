#include "Day16-PermutationPromenade.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day16PermutationPromenade)
{
public:

    TEST_METHOD(orderAfterDanceMovesSingleRepetition_SimpleTests)
    {
        Assert::AreEqual(std::string{"baedc"}, AoC::orderAfterDanceMovesSingleRepetition(m_danceMoveStrings, 5u));
    }

    TEST_METHOD(orderAfterDanceMovesRepetitions_SimpleTests)
    {
        Assert::AreEqual(std::string{"ceadb"}, AoC::orderAfterDanceMovesRepetitions(m_danceMoveStrings, 2u, 5u));
    }

private:
    const std::vector<std::string> m_danceMoveStrings =
    {
        "s1",
        "x3/4",
        "pe/b"
    };
};
