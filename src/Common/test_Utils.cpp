#include "CppUnitTest.h"

#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

namespace Microsoft
{
namespace VisualStudio
{
namespace CppUnitTestFramework
{
    template <typename T>
    std::wstring ToString(const std::vector<T> v)
    {
        std::wstringstream s;

        for (size_t i = 0; i < v.size(); i++)
        {
            s << ToString(v[i]);
            if (i != v.size() - 1)
            {
                s << ", ";
            }
        }

        return s.str();
    }
}
}
}

TEST_CLASS(Utils)
{
public:

    TEST_METHOD(parseStringIntoVector_SimpleTests)
    {
        std::string inputInts{"1 2 3 4 5"};
        std::vector<int> resultInts;
        std::vector<int> expectedInts{ 1, 2, 3, 4, 5 };
        AoC::Utils::parseStringIntoVector(inputInts, resultInts);
        Assert::AreEqual(expectedInts, resultInts);

        std::string inputStrings{ "ab cd e f" };
        std::vector<std::string> resultStrings;
        std::vector<std::string> expectedStrings{ "ab", "cd", "e", "f" };
        AoC::Utils::parseStringIntoVector(inputStrings, resultStrings);
        Assert::AreEqual(expectedStrings, resultStrings);
    }

};
