#pragma once

#include "Particle.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class ParticleSystem
{
public:
    ParticleSystem(std::vector<Particle> particles) noexcept;
    void simulate(unsigned numSteps);
    unsigned particleIDClosestToOrigin() const;

private:
    std::vector<Particle> m_particles;
};

}
