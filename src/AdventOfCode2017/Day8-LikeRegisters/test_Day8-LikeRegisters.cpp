#include "CppUnitTest.h"

#include "Day8-LikeRegisters.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day8LikeRegisters)
{
public:

    TEST_METHOD(largestRegisterAfterCompletion_SimpleTests)
    {
        std::vector<AoC::Instruction> instructions =
        {
            {{"b", "inc",   5}, {"a", ">",   1}},
            {{"a", "inc",   1}, {"b", "<",   5}},
            {{"c", "dec", -10}, {"a", ">=",  1}},
            {{"c", "inc", -20}, {"c", "==", 10}}
        };

        Assert::AreEqual(1, AoC::largestRegisterAfterCompletion(instructions));
    }

};
