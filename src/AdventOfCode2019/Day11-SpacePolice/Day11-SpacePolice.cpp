#include "Day11-SpacePolice.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <Eigen/Dense>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;

using Vector2D = Eigen::Matrix<int, 2, 1>;

enum class PaintColor
{
    BLACK = 0,
    WHITE = 1,
};

const char BLACK_CHAR = ' ';
const char WHITE_CHAR = '.';

using CoordinatesToColor = std::unordered_map<Coordinates, PaintColor, boost::hash<Coordinates>>;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class HullPaintingRobot
{
public:
    HullPaintingRobot(IntcodeInterpreter interpreter)
        : HullPaintingRobot(interpreter, {})
    {

    }

    HullPaintingRobot(IntcodeInterpreter interpreter, CoordinatesToColor initialCoordinatesToColor)
        : m_interpreter{std::move(interpreter)}
        , m_currentCoordinates{0, 0}
        , m_direction{Direction::UP}
        , m_coordinatesToColor{std::move(initialCoordinatesToColor)}
    {

    }

    void paint()
    {
        while (m_interpreter.getExecutionState() != IntcodeProgramExecutionState::TERMINATED)
        {
            m_interpreter.addInput(static_cast<IntcodeNumberType>(getCurrentColor()));
            m_interpreter.execute();

            const auto& outputs = m_interpreter.getOutputs();
            assert(outputs.size() == 0 || outputs.size() == 2);

            if (outputs.size() == 2)
            {
                m_coordinatesToColor[m_currentCoordinates] = static_cast<PaintColor>(outputs.front());

                if (outputs.back() == 0)
                {
                    turnLeft();
                }
                else
                {
                    turnRight();
                }

                moveForward();
                m_interpreter.clearOutputs();
            }
        }
    }

    unsigned getNumPanelsPaintedAtLeastOnce()
    {
        return m_coordinatesToColor.size();
    }

    std::string getRegistrationIdentifier()
    {
        std::pair<Vector2D, Vector2D> boundingBox = getBoundingBox();

        const Vector2D& offset = boundingBox.first;
        const Vector2D& size = boundingBox.second;

        const int width = size[0];
        const int height = size[1];

        std::vector<std::string> serializedLines(height, std::string(width, BLACK_CHAR));

        for (const auto& coordinatesAndColor : m_coordinatesToColor)
        {
            if (coordinatesAndColor.second == PaintColor::WHITE)
            {
                const Coordinates& coordinates = coordinatesAndColor.first;
                Vector2D positionVector{coordinates.first, coordinates.second};
                Vector2D positionRelativeToBoundingBox{positionVector - offset};

                const int posX = positionRelativeToBoundingBox[0];
                const int posY = positionRelativeToBoundingBox[1];

                serializedLines.at(posY).at(posX) = WHITE_CHAR;
            }
        }

        std::reverse(serializedLines.begin(), serializedLines.end());

        return boost::algorithm::join(serializedLines, "\n");
    }

private:
    IntcodeInterpreter m_interpreter;

    Coordinates m_currentCoordinates;
    Direction m_direction;

    CoordinatesToColor m_coordinatesToColor;

    void moveForward()
    {
        switch (m_direction)
        {
            case Direction::LEFT:
                m_currentCoordinates.first -= 1;
                break;
            case Direction::RIGHT:
                m_currentCoordinates.first += 1;
                break;
            case Direction::UP:
                m_currentCoordinates.second += 1;
                break;
            case Direction::DOWN:
                m_currentCoordinates.second -= 1;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void turnLeft()
    {
        switch (m_direction)
        {
            case Direction::LEFT:
                m_direction = Direction::DOWN;
                break;
            case Direction::RIGHT:
                m_direction = Direction::UP;
                break;
            case Direction::UP:
                m_direction = Direction::LEFT;
                break;
            case Direction::DOWN:
                m_direction = Direction::RIGHT;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void turnRight()
    {
        for (int i = 1; i <= 3; ++i)
        {
            turnLeft();
        }
    }

    PaintColor getCurrentColor() const
    {
        const auto currentCoordinatesAndColorIter = m_coordinatesToColor.find(m_currentCoordinates);
        if (currentCoordinatesAndColorIter == m_coordinatesToColor.cend())
        {
            return PaintColor::BLACK;
        }
        return currentCoordinatesAndColorIter->second;
    }

    std::pair<Vector2D, Vector2D> getBoundingBox() const
    {
        auto minmaxXCoordinatesIterPair = std::minmax_element(m_coordinatesToColor.cbegin(), m_coordinatesToColor.cend(), [](const auto& lhs, const auto& rhs)
                                                              {
                                                                  const auto& lhsCoords = lhs.first;
                                                                  const auto& rhsCoords = rhs.first;
                                                                  return lhsCoords.first < rhsCoords.first;
                                                              });

        const auto& minXCoordsAndColor = minmaxXCoordinatesIterPair.first;
        const auto& maxXCoordsAndColor = minmaxXCoordinatesIterPair.second;
        const auto& minXCoords = minXCoordsAndColor->first;
        const auto& maxXCoords = maxXCoordsAndColor->first;
        const int minX = minXCoords.first;
        const int maxX = maxXCoords.first;

        auto minmaxYCoordinatesIterPair = std::minmax_element(m_coordinatesToColor.cbegin(), m_coordinatesToColor.cend(), [](const auto& lhs, const auto& rhs)
                                                              {
                                                                  const auto& lhsCoords = lhs.first;
                                                                  const auto& rhsCoords = rhs.first;
                                                                  return lhsCoords.second < rhsCoords.second;
                                                              });

        const auto& minYCoordsAndColor = minmaxYCoordinatesIterPair.first;
        const auto& maxYCoordsAndColor = minmaxYCoordinatesIterPair.second;
        const auto& minYCoords = minYCoordsAndColor->first;
        const auto& maxYCoords = maxYCoordsAndColor->first;
        const int minY = minYCoords.second;
        const int maxY = maxYCoords.second;

        return std::make_pair(Vector2D{minX, minY}, Vector2D{maxX - minX + 1, maxY - minY + 1});
    }
};

unsigned numPanelsPaintedAtLeastOnce(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    HullPaintingRobot robot{std::move(interpreter)};

    robot.paint();

    return robot.getNumPanelsPaintedAtLeastOnce();
}

std::string registrationIdentifier(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};
    CoordinatesToColor initialCoordinatesToColor;
    initialCoordinatesToColor[{0, 0}] = PaintColor::WHITE;

    HullPaintingRobot robot{std::move(interpreter), initialCoordinatesToColor};

    robot.paint();

    return robot.getRegistrationIdentifier();
}

}
