#include "Day15-DuelingGenerators.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day15DuelingGenerators)
{
public:

    TEST_METHOD(judgeFinalCountNoCriteria_SimpleTests)
    {
        Assert::AreEqual(588u, AoC::judgeFinalCountNoCriteria(m_generatorAStart, m_generatorBStart));
    }

    TEST_METHOD(judgeFinalCountWithCriteria_SimpleTests)
    {
        Assert::AreEqual(309u, AoC::judgeFinalCountWithCriteria(m_generatorAStart, m_generatorBStart));
    }

private:
    const unsigned m_generatorAStart = 65;
    const unsigned m_generatorBStart = 8921;
};
