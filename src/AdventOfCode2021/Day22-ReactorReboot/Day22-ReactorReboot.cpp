#include "Day22-ReactorReboot.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/dense>
#include <boost/functional/hash.hpp>

#include <regex>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const int INITIALIZATION_AREA_SIZE = 50;

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day22
{

using CubeState = bool;
using Vector3D = Eigen::Vector3i;

struct Vector3DHash
{
    size_t operator()(const Vector3D& vec) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());
        boost::hash_combine(seed, vec.z());

        return seed;
    }
};

struct RebootStep
{
    CubeState state;
    Vector3D from;
    Vector3D to;

    bool isInitializationStep() const
    {
        std::vector<int> allCoordinates = {from.x(), from.y(), from.z(), to.x(), to.y(), to.z()};

        return std::all_of(allCoordinates.cbegin(), allCoordinates.cend(), [](const auto& coordinate)
                           {
                               return coordinate >= -INITIALIZATION_AREA_SIZE && coordinate <= INITIALIZATION_AREA_SIZE;
                           });
    }
};

class SubmarineReactorRebooter
{
public:

    SubmarineReactorRebooter(std::vector<RebootStep> rebootSteps)
        : m_rebootSteps{std::move(rebootSteps)}
    {

    }

    void executeRebootInitializationProcedure()
    {
        for (const auto& rebootStep : m_rebootSteps)
        {
            if (rebootStep.isInitializationStep())
            {
                executeRebootStep(rebootStep);
            }
        }
    }

    uint64_t getNumCubesOn() const
    {
        return m_cubesOn.size();
    }

private:
    std::vector<RebootStep> m_rebootSteps;

    std::unordered_set<Vector3D, Vector3DHash> m_cubesOn;

    void executeRebootStep(const RebootStep& rebootStep)
    {
        for (int k = rebootStep.from.z(); k <= rebootStep.to.z(); ++k)
        {
            for (int j = rebootStep.from.y(); j <= rebootStep.to.y(); ++j)
            {
                for (int i = rebootStep.from.x(); i <= rebootStep.to.x(); ++i)
                {
                    changeCubeState({i, j, k}, rebootStep.state);
                }
            }
        }
    }

    void changeCubeState(Vector3D coordinates, CubeState targetState)
    {
        if (targetState)
        {
            m_cubesOn.insert(std::move(coordinates));
        }
        else
        {
            m_cubesOn.erase(coordinates);
        }
    }
};

RebootStep parseRebootStepLine(const std::string& rebootStepLine)
{
    std::regex rebootStepLineRegex(R"(((?:on)|(?:off)) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+))");
    std::smatch rebootStepLineMatch;
    std::regex_match(rebootStepLine, rebootStepLineMatch, rebootStepLineRegex);

    const bool state = rebootStepLineMatch[1].str() == "on";
    const int fromX = std::stoi(rebootStepLineMatch[2].str());
    const int toX = std::stoi(rebootStepLineMatch[3].str());
    const int fromY = std::stoi(rebootStepLineMatch[4].str());
    const int toY = std::stoi(rebootStepLineMatch[5].str());
    const int fromZ = std::stoi(rebootStepLineMatch[6].str());
    const int toZ = std::stoi(rebootStepLineMatch[7].str());

    Vector3D from{fromX, fromY, fromZ};
    Vector3D to{toX, toY, toZ};

    return RebootStep{state, std::move(from), std::move(to)};
}

std::vector<RebootStep> parseRebootStepLines(const std::vector<std::string>& rebootStepLines)
{
    std::vector<RebootStep> rebootSteps;

    for (const auto& line : rebootStepLines)
    {
        RebootStep rebootStep = parseRebootStepLine(line);
        rebootSteps.push_back(std::move(rebootStep));
    }

    return rebootSteps;
}

uint64_t numCubesOnAfterInititializationProcedure(const std::vector<std::string>& rebootStepLines)
{
    std::vector<RebootStep> rebootSteps = parseRebootStepLines(rebootStepLines);
    SubmarineReactorRebooter rebooter{std::move(rebootSteps)};
    rebooter.executeRebootInitializationProcedure();
    return rebooter.getNumCubesOn();
}

}
}
}