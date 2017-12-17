#include "Utils.h"

#include "DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

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

    TEST_METHOD(splitStringIntoTypedVector_DefaultDelimiterTests)
    {
        std::string inputInts{"1 2 3 4 5"};
        std::vector<int> resultInts;
        std::vector<int> expectedInts{1, 2, 3, 4, 5};
        AoC::Utils::splitStringIntoTypedVector(inputInts, resultInts);
        Assert::AreEqual(expectedInts, resultInts);

        std::string inputStrings{"ab cd e f"};
        std::vector<std::string> resultStrings;
        std::vector<std::string> expectedStrings{"ab", "cd", "e", "f"};
        AoC::Utils::splitStringIntoTypedVector(inputStrings, resultStrings);
        Assert::AreEqual(expectedStrings, resultStrings);
    }

    TEST_METHOD(splitStringIntoTypedVector_NonDefaultDelimiterTests)
    {
        std::string inputInts{"1,2,3,4,5"};
        std::vector<int> resultInts;
        std::vector<int> expectedInts{1, 2, 3, 4, 5};
        AoC::Utils::splitStringIntoTypedVector(inputInts, resultInts, ',');
        Assert::AreEqual(expectedInts, resultInts);

        std::string inputStrings{"ab-cd-e-f"};
        std::vector<std::string> resultStrings;
        std::vector<std::string> expectedStrings{"ab", "cd", "e", "f"};
        AoC::Utils::splitStringIntoTypedVector(inputStrings, resultStrings, '-');
        Assert::AreEqual(expectedStrings, resultStrings);
    }

    TEST_METHOD(allElementsEqual_RangeIsEqual_ReturnTrue)
    {
        std::string emptyString{""};
        Assert::IsTrue(AoC::Utils::allElementsEqual(emptyString.cbegin(), emptyString.cend()));

        std::vector<int> emptyVector;
        Assert::IsTrue(AoC::Utils::allElementsEqual(emptyVector.cbegin(), emptyVector.cend()));

        std::string oneElementString{"t"};
        Assert::IsTrue(AoC::Utils::allElementsEqual(oneElementString.cbegin(), oneElementString.cend()));

        std::vector<int> oneElementVector{11};
        Assert::IsTrue(AoC::Utils::allElementsEqual(oneElementVector.cbegin(), oneElementVector.cend()));

        std::string allTheSameLettersString{"aaaaaa"};
        Assert::IsTrue(AoC::Utils::allElementsEqual(allTheSameLettersString.cbegin(), allTheSameLettersString.cend()));

        std::vector<int> allTheSameElementsVector{12, 12, 12, 12};
        Assert::IsTrue(AoC::Utils::allElementsEqual(allTheSameElementsVector.cbegin(), allTheSameElementsVector.cend()));

        std::vector<double> equalDoublesWithThresholdVector{1.0, 0.999, 1.002, 1.05};
        Assert::IsTrue(AoC::Utils::allElementsEqual(equalDoublesWithThresholdVector.cbegin(), equalDoublesWithThresholdVector.cend(),
                                                    [](double lhs, double rhs)
                                                    {
                                                        return std::fabs(lhs - rhs) < 0.1;
                                                    }));
    }

    TEST_METHOD(allElementsEqual_RangeIsNotEqual_ReturnFalse)
    {
        std::string differentLetters{"aabcdefa"};
        Assert::IsFalse(AoC::Utils::allElementsEqual(differentLetters.cbegin(), differentLetters.cend()));

        std::vector<int> differentElementsVector{12, 13, 12, 14};
        Assert::IsFalse(AoC::Utils::allElementsEqual(differentElementsVector.cbegin(), differentElementsVector.cend()));

        std::vector<double> farDifferentDoubles{1.7, 0.999, 1.25, 1.05};
        Assert::IsFalse(AoC::Utils::allElementsEqual(farDifferentDoubles.cbegin(), farDifferentDoubles.cend(),
                                                     [](double lhs, double rhs)
                                                     {
                                                         return std::fabs(lhs - rhs) < 0.1;
                                                     }));
    }

};
