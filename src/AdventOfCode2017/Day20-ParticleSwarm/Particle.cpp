#include "Particle.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
// Add library includes here
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

Particle::Particle(VectorType position, VectorType velocity, VectorType acceleration) noexcept
    : m_position(std::move(position))
    , m_velocity(std::move(velocity))
    , m_acceleration(std::move(acceleration))
{

}

void Particle::stepTime()
{
    m_velocity += m_acceleration;
    m_position += m_velocity;
}

const Particle::VectorType& Particle::getPosition() const noexcept
{
    return m_position;
}

}
