#include "Day02-PasswordPhilosophy.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day02PasswordPhilosophy)
{
public:

    TEST_METHOD(numValidPasswords_SimpleTests)
    {
        std::vector<std::string> lines =
        {
            "1-3 a: abcde",
            "1-3 b: cdefg",
            "2-9 c: ccccccccc"
        };

        Assert::AreEqual(2, AoC::numValidPasswords(lines));
    }

};
