#include "Day14-DockingData.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day14DockingData)
{
public:

    TEST_METHOD(sumOfValuesInMemoryAfterCompletion_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X",
            "mem[8] = 11",
            "mem[7] = 101",
            "mem[8] = 0"
        };

        Assert::AreEqual(165ull, AoC::sumOfValuesInMemoryAfterCompletion(lines));
    }

};
