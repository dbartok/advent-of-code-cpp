#include "Day24-ElectromagneticMoat.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day24ElectromagneticMoat)
{
public:

    TEST_METHOD(strengthOfStrongestBridge_SimpleTests)
    {
        const std::vector<std::string> componentLines =
        {
            "0/2",
            "2/2",
            "2/3",
            "3/4",
            "3/5",
            "0/1",
            "10/1",
            "9/10"
        };

        Assert::AreEqual(31u, AoC::strengthOfStrongestBridge(componentLines));
    }

};
