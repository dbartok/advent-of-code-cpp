#include "Head.h"

#include <cassert>

namespace AdventOfCode
{

Head::Head(std::pair<int, int> position, Direction direction)
    : m_position(std::move(position))
    , m_direction(direction)
{

}

const std::pair<int, int>& Head::getPosition() const
{
    return m_position;
}

Direction Head::getDirection() const
{
    return m_direction;
}

void Head::step()
{
    switch (m_direction)
    {
        case Direction::LEFT:
            m_position.first -= 1;
            break;
        case Direction::RIGHT:
            m_position.first += 1;
            break;
        case Direction::UP:
            m_position.second += 1;
            break;
        case Direction::DOWN:
            m_position.second -= 1;
            break;
        default:
            // Enum has invalid value
            assert(false);
    }
}

void Head::turnLeft()
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
            // Enum has invalid value
            assert(false);
    }
}

}
