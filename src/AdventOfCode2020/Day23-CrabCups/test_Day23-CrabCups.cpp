#include "Day23-CrabCups.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day23CrabCups)
{
public:

    TEST_METHOD(cupLabelsStartingFromCupOne_SimpleTests)
    {
        Assert::AreEqual(std::string{"92658374"}, AoC::cupLabelsStartingFromCupOne("389125467", 10));
        Assert::AreEqual(std::string{"67384529"}, AoC::cupLabelsStartingFromCupOne("389125467", 100));
    }

};
