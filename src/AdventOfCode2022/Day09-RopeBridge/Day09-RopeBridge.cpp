#include "Day09-RopeBridge.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>
#include <Eigen/dense>

#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day09
{

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct Motion
{
    Direction direction;
    unsigned magnitude;
};

using Vector2D = Eigen::Vector2i;

struct Vector2DHash
{
    size_t operator()(const Vector2D& vec) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, vec.x());
        boost::hash_combine(seed, vec.y());

        return seed;
    }
};

class RopeSimulator
{
public:
    RopeSimulator(std::vector<Motion> motions)
        : m_motions{std::move(motions)}
        , m_headPosition{0, 0}
        , m_tailPosition{0, 0}
    {

    }

    void simulate()
    {
        for (const auto& motion : m_motions)
        {
            simulateMotion(motion);
        }
    }

    int getNumPositionsVisitedByTail() const
    {
        return m_positionsVisitedByTail.size();
    }

private:
    std::vector<Motion> m_motions;

    Vector2D m_headPosition;
    Vector2D m_tailPosition;
    std::unordered_set<Vector2D, Vector2DHash> m_positionsVisitedByTail;

    void simulateMotion(const Motion& motion)
    {
        for (unsigned iteration = 0; iteration < motion.magnitude; ++iteration)
        {
            simulateSingleStep(motion.direction);
        }
    }

    void simulateSingleStep(Direction direction)
    {
        moveHeadBySingleStepTowardsDirection(direction);
        adjustTail();
        m_positionsVisitedByTail.insert(m_tailPosition);
    }

    void moveHeadBySingleStepTowardsDirection(Direction direction)
    {
        const Vector2D movementVector = getSingleStepMotionVector(direction);
        m_headPosition += movementVector;
    }

    void adjustTail()
    {
        const Vector2D tailToHeadVector = m_headPosition - m_tailPosition;

        if (abs(tailToHeadVector.x()) > 1 || abs(tailToHeadVector.y()) > 1)
        {
            const int adjustmentX = abs(tailToHeadVector.x()) > 0 ? tailToHeadVector.x() / abs(tailToHeadVector.x()) : 0;
            const int adjustmentY = abs(tailToHeadVector.y()) > 0 ? tailToHeadVector.y() / abs(tailToHeadVector.y()) : 0;

            const Vector2D adjustmentVector = Vector2D{adjustmentX, adjustmentY};

            m_tailPosition += adjustmentVector;
        }
    }

    static Vector2D getSingleStepMotionVector(Direction direction)
    {
        switch (direction)
        {
            case Direction::UP:
                return Vector2D{0, 1};
            case Direction::DOWN:
                return Vector2D{0, -1};
            case Direction::LEFT:
                return Vector2D{-1, 0};
            case Direction::RIGHT:
                return Vector2D{1, 0};
            default:
                throw std::runtime_error("Invalid direction: " + std::to_string(static_cast<int>(direction)));
        }
    }
};

Direction parseDirectionChar(char directionChar)
{
    switch (directionChar)
    {
        case 'U':
            return Direction::UP;
        case 'D':
            return Direction::DOWN;
        case 'L':
            return Direction::LEFT;
        case 'R':
            return Direction::RIGHT;
        default:
            throw std::runtime_error("Error parsing direction char: " + std::string{directionChar});
    }
}

Motion parseMotionLine(const std::string& motionLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, motionLine, boost::is_any_of(" "));

    const Direction direction = parseDirectionChar(tokens.at(0).front());
    const unsigned magnitude = std::stoi(tokens.at(1));

    return Motion{direction, magnitude};
}

std::vector<Motion> parseMotionLines(const std::vector<std::string>& motionLines)
{
    std::vector<Motion> motions;

    for (const auto& motionLine : motionLines)
    {
        Motion motion = parseMotionLine(motionLine);
        motions.push_back(std::move(motion));
    }

    return motions;
}


int numPositionsVisitedByTail(const std::vector<std::string>& motionLines)
{
    std::vector<Motion> motions = parseMotionLines(motionLines);

    RopeSimulator ropeSimulator{std::move(motions)};

    ropeSimulator.simulate();

    return ropeSimulator.getNumPositionsVisitedByTail();
}

}
}
}