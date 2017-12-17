#include "Day07-RecursiveCircus.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day07RecursiveCircus)
{
public:

    TEST_METHOD(nameOfBottomProgram_SimpleTests)
    {
        Assert::AreEqual(std::string("tknk"), AoC::nameOfBottomProgram(m_nodeDescriptors));
    }

    TEST_METHOD(correctWeightOfWrongWeightProgram_SimpleTests)
    {
        Assert::AreEqual(60, AoC::correctWeightOfWrongWeight(m_nodeDescriptors));
    }

private:
    std::vector<AoC::NodeDescriptor> m_nodeDescriptors =
    {
        {"pbga", 66, {}},
        {"xhth", 57, {}},
        {"ebii", 61, {}},
        {"havc", 66, {}},
        {"ktlj", 57, {}},
        {"fwft", 72, {"ktlj", "cntj", "xhth"}},
        {"qoyq", 66, {}},
        {"padx", 45, {"pbga", "havc", "qoyq"}},
        {"tknk", 41, {"ugml", "padx", "fwft"}},
        {"jptl", 61, {}},
        {"ugml", 68, {"gyxo", "ebii", "jptl"}},
        {"gyxo", 61, {}},
        {"cntj", 57, {}}
    };

};
