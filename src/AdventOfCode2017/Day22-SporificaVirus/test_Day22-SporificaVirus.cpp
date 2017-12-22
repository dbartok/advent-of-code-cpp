#include "Day22-SporificaVirus.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day22SporificaVirus)
{
public:

    TEST_METHOD(basicVirusInfectingBursts_SimpleTests)
    {
        Assert::AreEqual(5u, AoC::basicVirusInfectingBursts(m_initialVirusPosLines, 7u));
        Assert::AreEqual(41u, AoC::basicVirusInfectingBursts(m_initialVirusPosLines, 70u));
        Assert::AreEqual(5587u, AoC::basicVirusInfectingBursts(m_initialVirusPosLines));
    }

    TEST_METHOD(evolvedVirusInfectingBursts_SimpleTests)
    {
        Assert::AreEqual(26u, AoC::evolvedVirusInfectingBursts(m_initialVirusPosLines, 100u));

        // Runs slow in Debug (about two minutes)
        //Assert::AreEqual(2511944u, AoC::evolvedVirusInfectingBursts(m_initialVirusPosLines));
    }

private:
    const std::vector<std::string> m_initialVirusPosLines =
    {
        "..#",
        "#..",
        "..."
    };

};
