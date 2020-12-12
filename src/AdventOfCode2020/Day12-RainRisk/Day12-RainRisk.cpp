#include "Day12-RainRisk.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <Eigen/Dense>

#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Vector2D = Eigen::Matrix<int, 2, 1>;

enum class NavigationAction
{
    FORWARD,
    LEFT,
    RIGHT,
    NORTH,
    SOUTH,
    EAST,
    WEST,
};

struct NavigationInstruction
{
    NavigationAction action;
    int value;
};

class ShipNavigator
{
public:
    ShipNavigator(std::vector<NavigationInstruction> navigationInstructions)
        : m_navigationInstructions{std::move(navigationInstructions)}
        , m_positionVector{0, 0}
        , m_directionVector{1, 0}
    {
        m_actionToFunc =
        {
            {NavigationAction::FORWARD, [this](int value) {this->moveForward(value); }},
            {NavigationAction::LEFT, [this](int value) {this->turnLeft(value); }},
            {NavigationAction::RIGHT, [this](int value) {this->turnRight(value); }},
            {NavigationAction::NORTH, [this](int value) {this->moveNorth(value); }},
            {NavigationAction::SOUTH, [this](int value) {this->moveSouth(value); }},
            {NavigationAction::EAST, [this](int value) {this->moveEast(value); }},
            {NavigationAction::WEST, [this](int value) {this->moveWest(value); }},
        };
    }

    void executeAllInstructions()
    {
        for (const auto& instruction : m_navigationInstructions)
        {
            m_actionToFunc.at(instruction.action)(instruction.value);
        }
    }

    int getManhattanDistanceFromStart() const
    {
        return m_positionVector.cwiseAbs().sum();
    }

private:
    std::vector<NavigationInstruction> m_navigationInstructions;

    std::unordered_map<NavigationAction, std::function<void(int)>> m_actionToFunc;

    Vector2D m_positionVector;
    Vector2D m_directionVector;

    void moveForward(int value)
    {
        m_positionVector += m_directionVector * value;
    }

    void turnLeft(int value)
    {
        while ((value -= 90) >= 0)
        {
            m_directionVector[1] *= -1;
            std::swap(m_directionVector[0], m_directionVector[1]);
        }
    }

    void turnRight(int value)
    {
        turnLeft(360 - value);
    }

    void moveNorth(int value)
    {
        m_positionVector += Vector2D{0, 1} * value;
    }

    void moveSouth(int value)
    {
        m_positionVector += Vector2D{0, -1} * value;
    }

    void moveEast(int value)
    {
        m_positionVector += Vector2D{1, 0} * value;
    }

    void moveWest(int value)
    {
        m_positionVector += Vector2D{-1, 0} * value;
    }
};

NavigationAction charToNavigationAction(char c)
{
    const static std::unordered_map<char, NavigationAction> mapping
    {
        {'F', NavigationAction::FORWARD},
        {'L', NavigationAction::LEFT},
        {'R', NavigationAction::RIGHT},
        {'N', NavigationAction::NORTH},
        {'S', NavigationAction::SOUTH},
        {'E', NavigationAction::EAST},
        {'W', NavigationAction::WEST},
    };

    return mapping.at(c);
}

NavigationInstruction parseInstruction(const std::string& instructionLine)
{
    NavigationAction action = charToNavigationAction(instructionLine.front());
    int value = std::stoi(instructionLine.substr(1));

    return NavigationInstruction{action, value};
}

std::vector<NavigationInstruction> parseInstructions(const std::vector<std::string>& instructionLines)
{
    std::vector<NavigationInstruction> instructions;

    for (const auto& line : instructionLines)
    {
        NavigationInstruction instruction = parseInstruction(line);
        instructions.push_back(std::move(instruction));
    }

    return instructions;
}

int manhattanDistanceToEndLocation(const std::vector<std::string>& instructionLines)
{
    std::vector<NavigationInstruction> instructions = parseInstructions(instructionLines);
    ShipNavigator shipNavigator{std::move(instructions)};
    shipNavigator.executeAllInstructions();
    return shipNavigator.getManhattanDistanceFromStart();
}

}
