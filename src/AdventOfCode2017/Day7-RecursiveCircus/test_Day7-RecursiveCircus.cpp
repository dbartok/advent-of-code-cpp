#include "CppUnitTest.h"

#include "Day7-RecursiveCircus.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day7RecursiveCircus)
{
public:

    TEST_METHOD(nameOfBottomProgram_SimpleTests)
    {
        std::vector<AoC::NodeDescriptor> nodeDescriptors =
        {
            {"pbga", {}},
            {"xhth", {}},
            {"ebii", {}},
            {"havc", {}},
            {"ktlj", {}},
            {"fwft", {"ktlj", "cntj", "xhth"}},
            {"qoyq", {}},
            {"padx", {"pbga", "havc", "qoyq"}},
            {"tknk", {"ugml", "padx", "fwft"}},
            {"jptl", {}},
            {"ugml", {"gyxo", "ebii", "jptl"}},
            {"gyxo", {}},
            {"cntj", {}}
        };

        Assert::AreEqual(std::string("tknk"), AoC::nameOfBottomProgram(nodeDescriptors));
    }

};
