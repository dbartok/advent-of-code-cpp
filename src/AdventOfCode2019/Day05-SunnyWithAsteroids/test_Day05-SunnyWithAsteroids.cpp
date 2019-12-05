#include "Day05-SunnyWithAsteroids.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day05SunnyWithAsteroids)
{
public:

    TEST_METHOD(diagnosticCodeProducedByProgram_SimpleTests)
    {
        Assert::AreEqual(4, AoC::diagnosticCodeProducedByProgram(std::vector<int>{4, 0, 99}));
    }

};
