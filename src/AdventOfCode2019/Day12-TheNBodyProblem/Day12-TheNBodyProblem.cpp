#include "Day12-TheNBodyProblem.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/math/common_factor_rt.hpp>

#include <Eigen/Dense>

#include <numeric>
#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Vector3D = Eigen::Matrix<int, 3, 1>;

struct Moon
{
    Moon(Vector3D pos)
        : pos{std::move(pos)}
        , vel{0, 0, 0}
    {

    }

    Vector3D pos;
    Vector3D vel;

    bool operator==(const Moon& other) const = default;

    bool isAxisMatching(const Moon& other, size_t axisIndex) const
    {
        return pos[axisIndex] == other.pos[axisIndex] && vel[axisIndex] == other.vel[axisIndex];
    }
};

class MoonMotionSimulator
{
public:
    MoonMotionSimulator(std::vector<Moon> moons)
        : m_initialMoons{std::move(moons)}
    {
        reset();
    }

    void simulate(unsigned numSteps)
    {
        for (size_t i = 0; i < numSteps; ++i)
        {
            step();
        }
    }

    void simulateUntilRepetition()
    {
        std::vector<unsigned long long> individualAxisResults;

        for (int axisIndex = 0; axisIndex <= 2; ++axisIndex)
        {
            reset();

            simulateAxisUntilRepetition(axisIndex);

            individualAxisResults.push_back(getNumSteps());
        }

        m_numSteps = std::accumulate(individualAxisResults.cbegin(), individualAxisResults.cend(), 1ull, [](auto total, auto current)
                                     {
                                         return boost::math::lcm(total, current);
                                     });
    }

    int getTotalEnergy() const
    {
        return std::accumulate(m_moons.cbegin(), m_moons.cend(), 0, [](int total, const Moon& moon)
                               {
                                   return total + (moon.pos.cwiseAbs().sum() * moon.vel.cwiseAbs().sum());
                               });
    }

    unsigned long long getNumSteps() const
    {
        return m_numSteps;
    }

private:
    std::vector<Moon> m_moons;

    std::vector<Moon> m_initialMoons;
    unsigned long long m_numSteps;

    void step()
    {
        ++m_numSteps;
        applyAllGravity();
        applyAllVelocity();
    }

    void applyAllGravity()
    {
        for (auto firstMoonIter = m_moons.begin(); firstMoonIter != m_moons.end(); ++firstMoonIter)
        {
            for (auto secondMoonIter = std::next(firstMoonIter); secondMoonIter != m_moons.end(); ++secondMoonIter)
            {
                applyGravity(*firstMoonIter, *secondMoonIter);
            }
        }
    }

    void applyAllVelocity()
    {
        for (auto& moon : m_moons)
        {
            moon.pos += moon.vel;
        }
    }

    void simulateAxisUntilRepetition(size_t axisIndex)
    {
        while (true)
        {
            step();

            bool isRepeat = std::equal(m_moons.cbegin(), m_moons.cend(), m_initialMoons.cbegin(), [axisIndex](const auto& lhs, const auto& rhs)
                                       {
                                           return lhs.isAxisMatching(rhs, axisIndex);
                                       });

            if (isRepeat)
            {
                break;
            }
        }
    }

    void reset()
    {
        m_numSteps = 0;
        m_moons = m_initialMoons;
    }

    static void applyGravity(Moon& firstMoon, Moon& secondMoon)
    {
        for (int axisIndex = 0; axisIndex <= 2; ++axisIndex)
        {
            int& firstMoonPositionCoordinate = firstMoon.pos[axisIndex];
            int& secondMoonPositionCoordinate = secondMoon.pos[axisIndex];

            int& firstMoonVelocityCoordinate = firstMoon.vel[axisIndex];
            int& secondMoonVelocityCoordinate = secondMoon.vel[axisIndex];

            pullCloser(firstMoonPositionCoordinate, secondMoonPositionCoordinate, firstMoonVelocityCoordinate, secondMoonVelocityCoordinate);
        }
    }

    static void pullCloser(int p1, int p2, int& v1, int& v2)
    {
        if (p1 < p2)
        {
            ++v1;
            --v2;
        }
        else if (p1 > p2)
        {
            --v1;
            ++v2;
        }
    }
};

std::vector<Moon> createMoons(const std::vector<std::string>& moonPositionLines)
{
    std::vector<Moon> moons;

    for (const auto& line : moonPositionLines)
    {
        std::regex coordinatesRegex(R"(<x=(-?[0-9]+), y=(-?[0-9]+), z=(-?[0-9]+)>)");
        std::smatch coordinatesMatch;

        std::regex_match(line, coordinatesMatch, coordinatesRegex);

        int posX = std::stoi(coordinatesMatch[1].str());
        int posY = std::stoi(coordinatesMatch[2].str());
        int posZ = std::stoi(coordinatesMatch[3].str());

        moons.emplace_back(Vector3D{posX, posY, posZ});
    }

    return moons;

}

int totalEnergyAfterSteps(const std::vector<std::string>& moonPositionLines, unsigned numSteps)
{
    std::vector<Moon> moons = createMoons(moonPositionLines);

    MoonMotionSimulator simulator{std::move(moons)};

    simulator.simulate(numSteps);

    return simulator.getTotalEnergy();
}

unsigned long long numStepsUntilRepetition(const std::vector<std::string>& moonPositionLines)
{
    std::vector<Moon> moons = createMoons(moonPositionLines);

    MoonMotionSimulator simulator{std::move(moons)};

    simulator.simulateUntilRepetition();

    return simulator.getNumSteps();
}

}
