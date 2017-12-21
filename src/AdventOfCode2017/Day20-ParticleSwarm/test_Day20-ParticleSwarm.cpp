#include "Day20-ParticleSwarm.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day20ParticleSwarm)
{
public:

    TEST_METHOD(particleIDClosestToOriginLongTerm_SimpleTests)
    {
        const std::vector<std::string> particleLines =
        {
            "p=< 3,0,0>, v=< 2,0,0>, a=<-1,0,0>",
            "p=< 4,0,0>, v=< 0,0,0>, a=<-2,0,0>"
        };

        Assert::AreEqual(0u, AoC::particleIDClosestToOriginLongTerm(particleLines));
    }

    TEST_METHOD(numParticlesLeftAfterCollisions_SimpleTests)
    {
        const std::vector<std::string> particleLines =
        {
            "p=<-6,0,0>, v=< 3,0,0>, a=< 0,0,0>",
            "p=<-4,0,0>, v=< 2,0,0>, a=< 0,0,0>",
            "p=<-2,0,0>, v=< 1,0,0>, a=< 0,0,0>",
            "p=< 3,0,0>, v=<-1,0,0>, a=< 0,0,0>"
        };

        Assert::AreEqual(1u, AoC::numParticlesLeftAfterCollisions(particleLines));
    }

};
