#include "Day20-ParticleSwarm.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <fstream>
#include <iostream>
__END_LIBRARIES_DISABLE_WARNINGS

int main()
{
    namespace AoC = AdventOfCode;

    std::fstream fileIn("input.txt");
    std::vector<std::string> inputParticleLines;

    std::string lineBuffer;
    while (std::getline(fileIn, lineBuffer))
    {
        inputParticleLines.push_back(std::move(lineBuffer));
    }

    std::cout << "First part: " << AoC::particleIDClosestToOriginLongTerm(inputParticleLines) << std::endl;
    std::cout << "Second part: " << AoC::numParticlesLeftAfterCollisions(inputParticleLines) << std::endl;
}
