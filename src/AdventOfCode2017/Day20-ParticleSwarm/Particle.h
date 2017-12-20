#pragma once

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class Particle
{
public:
    using VectorType = Eigen::Matrix<long long, 3, 1>;

    Particle(VectorType position, VectorType velocity, VectorType acceleration) noexcept;

    void stepTime();
    const VectorType& getPosition() const noexcept;

private:
    VectorType m_position;
    VectorType m_velocity;
    VectorType m_acceleration;
};

}
