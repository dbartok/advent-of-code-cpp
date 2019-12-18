#include "Day17-SetAndForget.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_set>
#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
size_t CHARACTER_LIMIT = 20;
}

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct RobotCameraView
{
    Coordinates robotPosition;
    Direction robotDirection;
    CoordinatesSet scaffolds;
};

class ScaffoldAnalyzer
{
public:
    ScaffoldAnalyzer(RobotCameraView robotCameraView)
        : m_robotCameraView{std::move(robotCameraView)}
    {

    }

    int getSumOfAlignmentParameters() const
    {
        int sum = 0;

        for (const auto& scaffold : m_robotCameraView.scaffolds)
        {
            std::vector<Coordinates> neighborScaffolds = getNeighborScaffolds(scaffold);
            if (neighborScaffolds.size() == 4)
            {
                sum += (scaffold.first * scaffold.second);
            }
        }

        return sum;
    }

    void traverse()
    {
        while (true)
        {
            turnIfNeeded();
            stepForward();

            if (getNeighborScaffolds(m_robotCameraView.robotPosition).size() == 1)
            {
                m_journeySteps.push_back(std::to_string(m_stepsInStraightLine));
                break;
            }
        }
    }

    std::vector<std::string> getJourneySteps()
    {
        return m_journeySteps;
    }

private:
    RobotCameraView m_robotCameraView;

    std::vector<std::string> m_journeySteps;
    int m_stepsInStraightLine = 0;

    std::vector<Coordinates> getNeighborScaffolds(const Coordinates& coordinates) const
    {
        std::vector<Coordinates> neighbors
        {
            {coordinates.first - 1, coordinates.second},
            {coordinates.first + 1, coordinates.second},
            {coordinates.first, coordinates.second - 1},
            {coordinates.first, coordinates.second + 1},
        };

        neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [this](const auto& coord)
                                       {
                                           return this->m_robotCameraView.scaffolds.find(coord) == this->m_robotCameraView.scaffolds.cend();
                                       }), neighbors.end());

        return neighbors;
    }

    void turnIfNeeded()
    {
        if (isForwardPathObstructed())
        {
            if (m_stepsInStraightLine != 0)
            {
                m_journeySteps.push_back(std::to_string(m_stepsInStraightLine));
                m_stepsInStraightLine = 0;
            }

            turnLeft();
            if (!isForwardPathObstructed())
            {
                m_journeySteps.push_back("L");
            }
            else
            {
                turnLeft();
                turnLeft();
                assert(!isForwardPathObstructed());
                m_journeySteps.push_back("R");
            }
        }
    }

    bool isForwardPathObstructed()
    {
        Coordinates coordinatesInFront = getCoordinatesInFront();
        return m_robotCameraView.scaffolds.find(coordinatesInFront) == m_robotCameraView.scaffolds.cend();
    }

    Coordinates getCoordinatesInFront()
    {
        switch (m_robotCameraView.robotDirection)
        {
            case Direction::LEFT:
                return {m_robotCameraView.robotPosition.first - 1, m_robotCameraView.robotPosition.second};
            case Direction::RIGHT:
                return {m_robotCameraView.robotPosition.first + 1, m_robotCameraView.robotPosition.second};
            case Direction::UP:
                return {m_robotCameraView.robotPosition.first, m_robotCameraView.robotPosition.second - 1};
            case Direction::DOWN:
                return {m_robotCameraView.robotPosition.first, m_robotCameraView.robotPosition.second + 1};
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void turnLeft()
    {
        switch (m_robotCameraView.robotDirection)
        {
            case Direction::LEFT:
                m_robotCameraView.robotDirection = Direction::DOWN;
                break;
            case Direction::RIGHT:
                m_robotCameraView.robotDirection = Direction::UP;
                break;
            case Direction::UP:
                m_robotCameraView.robotDirection = Direction::LEFT;
                break;
            case Direction::DOWN:
                m_robotCameraView.robotDirection = Direction::RIGHT;
                break;
            default:
                throw std::runtime_error("Invalid direction");
        }
    }

    void stepForward()
    {
        assert(!isForwardPathObstructed());
        m_robotCameraView.robotPosition = getCoordinatesInFront();
        ++m_stepsInStraightLine;
    }
};

Direction createDirection(char c)
{
    switch (c)
    {
        case '^':
            return Direction::UP;
        case 'v':
            return Direction::DOWN;
        case '<':
            return Direction::LEFT;
        case '>':
            return Direction::RIGHT;
    }

    throw std::runtime_error("Unknown direction: " + std::string{c});
}

RobotCameraView createRobotCameraView(const std::vector<IntcodeNumberType>& outputs)
{
    RobotCameraView robotCameraView;

    int i = 0;
    int j = 0;
    for (auto output : outputs)
    {
        if (output == '\n')
        {
            i = 0;
            ++j;
            continue;
        }

        if (output != '.')
        {
            robotCameraView.scaffolds.insert({i, j});
            if (output != '#')
            {
                robotCameraView.robotPosition = {i, j};
                robotCameraView.robotDirection = createDirection(output);
            }
        }
        ++i;
    }

    return robotCameraView;
}

bool getSubstringDecomposition(const std::string& stringToDecompose, size_t numSubstrings, std::vector<std::string>& result)
{
    if (numSubstrings == 0)
    {
        return stringToDecompose.empty();
    }

    for (size_t i = 1; i <= stringToDecompose.length() && i <= CHARACTER_LIMIT; ++i)
    {
        std::string prefix = stringToDecompose.substr(0, i);

        std::string remaining = std::regex_replace(stringToDecompose, std::regex(prefix), "");

        result.push_back(prefix);
        if (getSubstringDecomposition(remaining, numSubstrings - 1, result))
        {
            return true;
        }
        result.pop_back();
    }

    return false;
}

std::vector<std::string> getSubstringDecomposition(const std::string& s, size_t numSubstrings)
{
    std::vector<std::string> substrings;
    getSubstringDecomposition(s, numSubstrings, substrings);
    return substrings;
}

std::string createProgramInput(const std::vector<std::string>& steps)
{
    std::string stepsString = boost::join(steps, ",");
    std::string decomposableString = stepsString + ",";
    std::vector<std::string> decomposition;
    decomposition = getSubstringDecomposition(decomposableString, 3);

    for (auto& element : decomposition)
    {
        element = element.substr(0, element.size() - 1); // strip the trailing comma
    }

    std::string mainRoutine = std::regex_replace(stepsString, std::regex(decomposition.at(0)), "A");
    mainRoutine = std::regex_replace(mainRoutine, std::regex(decomposition.at(1)), "B");
    mainRoutine = std::regex_replace(mainRoutine, std::regex(decomposition.at(2)), "C");

    return mainRoutine + "\n" +
        decomposition.at(0) + "\n" +
        decomposition.at(1) + "\n" +
        decomposition.at(2) + "\n" +
        "n\n"; // turn off video feed
}

int sumOfAlignmentParameters(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};
    interpreter.execute();

    RobotCameraView cameraView = createRobotCameraView(interpreter.getOutputs());

    ScaffoldAnalyzer analyzer{std::move(cameraView)};

    return analyzer.getSumOfAlignmentParameters();
}

int dustCollectedByRobot(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreterForAnalysis{intcodeProgram};
    interpreterForAnalysis.execute();

    RobotCameraView cameraView = createRobotCameraView(interpreterForAnalysis.getOutputs());

    ScaffoldAnalyzer analyzer{std::move(cameraView)};

    analyzer.traverse();

    auto interactiveProgram{intcodeProgram};
    interactiveProgram.at(0) = 2;

    IntcodeInterpreter interpreterForMovingRobot{std::move(interactiveProgram)};

    std::string programInput = createProgramInput(analyzer.getJourneySteps());
    for (auto c : programInput)
    {
        interpreterForMovingRobot.addInput(c);
    }

    interpreterForMovingRobot.execute();
    return interpreterForMovingRobot.getOutputs().back();
}


}
