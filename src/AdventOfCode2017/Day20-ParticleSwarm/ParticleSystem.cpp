#include "ParticleSystem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <algorithm>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

ParticleSystem::ParticleSystem(std::vector<Particle> particles) noexcept
    : m_particles{std::move(particles)}
{

}

void ParticleSystem::simulate(unsigned numSteps)
{
    for (unsigned i = 0; i < numSteps; ++i)
    {
        for (auto& particle : m_particles)
        {
            particle.stepTime();
        }
    }
}

unsigned ParticleSystem::particleIDClosestToOrigin() const
{
    if (m_particles.empty())
    {
        throw std::runtime_error("No particles.");
    }

    const auto minDistanceIter = std::min_element(m_particles.cbegin(), m_particles.cend(), [](const Particle& lhs, const Particle& rhs)
                                                  {
                                                      return lhs.getPosition().cwiseAbs().sum() < rhs.getPosition().cwiseAbs().sum();
                                                  });

    return minDistanceIter - m_particles.cbegin();
}

}
