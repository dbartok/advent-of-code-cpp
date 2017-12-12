#include "CppUnitTest.h"

#include "Day12-DigitalPlumber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day12DigitalPlumber)
{
public:

    TEST_METHOD(numNodesInGroupNodeZero_SimpleTests)
    {
        Assert::AreEqual(6u, AoC::numNodesInGroupNodeZero(m_neighborsLines));
    }

    TEST_METHOD(numTotalGroups_SimpleTests)
    {
        Assert::AreEqual(2u, AoC::numTotalGroups(m_neighborsLines));
    }

private:
    std::vector<std::string> m_neighborsLines =
    {
        "0 <-> 2",
        "1 <-> 1",
        "2 <-> 0, 3, 4",
        "3 <-> 2, 4",
        "4 <-> 2, 3, 6",
        "5 <-> 6",
        "6 <-> 4, 5"
    };
};