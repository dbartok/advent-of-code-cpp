#include "Day13-MineCartMadness.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>

#include <unordered_set>
#include <unordered_map>
#include <memory>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<unsigned, unsigned>;

enum class Orientation
{
    LEFT = 0,
    DOWN = 1,
    RIGHT = 2,
    UP = 3,
};

enum class Turn
{
    LEFT = 1,
    STRAIGHT = 0,
    RIGHT = 3,
};

class MineCart
{
public:
    MineCart(Orientation orientation, Coordinates position)
        : m_orientation{orientation}
        , m_position{position}
        , m_nextTurn{Turn::LEFT}
    {

    }

    Orientation getCurrentOrientation() const
    {
        return m_orientation;
    }

    Coordinates getCurrentPosition() const
    {
        return m_position;
    }

    void setCurrentOrientation(Orientation orientation)
    {
        m_orientation = orientation;
    }

    void turnToNextOrientation()
    {
        m_orientation = static_cast<Orientation>((static_cast<unsigned>(m_orientation) + static_cast<unsigned>(m_nextTurn)) % 4);

        if (m_nextTurn == Turn::LEFT)
        {
            m_nextTurn = Turn::STRAIGHT;
        }
        else if (m_nextTurn == Turn::STRAIGHT)
        {
            m_nextTurn = Turn::RIGHT;
        }
        else if (m_nextTurn == Turn::RIGHT)
        {
            m_nextTurn = Turn::LEFT;
        }
    }

    void move()
    {
        switch (m_orientation)
        {
            case Orientation::LEFT:
                m_position.first -= 1;
                break;

            case Orientation::RIGHT:
                m_position.first += 1;
                break;

            case Orientation::UP:
                m_position.second -= 1;
                break;

            case Orientation::DOWN:
                m_position.second += 1;
                break;
        }
    }

private:
    Orientation m_orientation;
    Coordinates m_position;
    Turn m_nextTurn;
};

class RailPiece;

using RailPieceSharedPtr = std::shared_ptr<RailPiece>;

class RailPiece
{
public:
    virtual void applyOrientation(MineCart& mineCart) = 0;
};

class DeterministicOrientingRailPiece : public RailPiece
{
public:
    virtual void applyOrientation(MineCart& mineCart)
    {
        Orientation currentOrientation = mineCart.getCurrentOrientation();

        Orientation nextOrientation = m_orientationMap.at(currentOrientation);

        mineCart.setCurrentOrientation(nextOrientation);
    }

protected:
    std::unordered_map<Orientation, Orientation> m_orientationMap;
};

class MainDiagonalRailPiece : public DeterministicOrientingRailPiece
{
public:
    MainDiagonalRailPiece()
    {
        m_orientationMap =
        {
            {Orientation::LEFT, Orientation::UP},
            {Orientation::UP, Orientation::LEFT},
            {Orientation::RIGHT, Orientation::DOWN},
            {Orientation::DOWN, Orientation::RIGHT}
        };
    }
};

class AntiDiagonalRailPiece : public DeterministicOrientingRailPiece
{
public:
    AntiDiagonalRailPiece()
    {
        m_orientationMap =
        {
            {Orientation::LEFT, Orientation::DOWN},
            {Orientation::DOWN, Orientation::LEFT},
            {Orientation::RIGHT, Orientation::UP},
            {Orientation::UP, Orientation::RIGHT}
        };
    }
};

class PassiveRailPiece : public RailPiece
{
    virtual void applyOrientation(MineCart&)
    {

    }
};

class EmptyRailPiece : public RailPiece
{
    virtual void applyOrientation(MineCart&)
    {
        throw std::runtime_error("Empty rail piece attempted to be used.");
    }
};

class IntersectionRailPiece : public RailPiece
{
    virtual void applyOrientation(MineCart& mineCart)
    {
        mineCart.turnToNextOrientation();
    }
};

using MineMapLine = std::vector<RailPieceSharedPtr>;
using MineMap = std::vector<MineMapLine>;

class MineManager
{
public:
    MineManager(MineMap mineMap, std::list<MineCart> mineCarts)
        : m_mineMap{std::move(mineMap)}
        , m_mineCarts{std::move(mineCarts)}
    {

    }

    Coordinates coordinatesOfFirstCollision()
    {
        while (true)
        {
            sortMineCarts();
            for (auto& mineCart : m_mineCarts)
            {
                mineCart.move();

                if (isThereCollision(mineCart.getCurrentPosition()))
                {
                    return mineCart.getCurrentPosition();
                }

                reorientMineCart(mineCart);
            }
        }
    }

    Coordinates coordinatesOfLastRemainingCart()
    {
        while (m_mineCarts.size() > 1)
        {
            sortMineCarts();
            for (auto mineCartIter = m_mineCarts.begin(); mineCartIter != m_mineCarts.end();)
            {
                mineCartIter->move();

                if (isThereCollision(mineCartIter->getCurrentPosition()))
                {
                    mineCartIter = eraseBothMineCartsWithSamePosition(mineCartIter);
                }
                else
                {
                    reorientMineCart(*mineCartIter);
                    ++mineCartIter;
                }
            }
        }

        return m_mineCarts.front().getCurrentPosition();
    }

private:
    MineMap m_mineMap;
    std::list<MineCart> m_mineCarts;

    bool isThereCollision(const Coordinates& position) const
    {
        auto numMineCartsAtPosition = std::count_if(m_mineCarts.cbegin(), m_mineCarts.cend(), [&position](const MineCart& mc) {return mc.getCurrentPosition() == position; });
        return numMineCartsAtPosition >= 2;
    }

    void sortMineCarts()
    {
        std::vector<MineCart> mineCartsVector{std::make_move_iterator(m_mineCarts.begin()), std::make_move_iterator(m_mineCarts.end())};

        std::sort(mineCartsVector.begin(), mineCartsVector.end(),
                  [](const MineCart& lhs, const MineCart& rhs)
                  {
                      return lhs.getCurrentPosition() < rhs.getCurrentPosition();
                  });

        m_mineCarts.assign(std::make_move_iterator(mineCartsVector.begin()), std::make_move_iterator(mineCartsVector.end()));
    }

    std::list<MineCart>::iterator eraseBothMineCartsWithSamePosition(std::list<MineCart>::iterator mineCartIter)
    {
        std::list<MineCart>::iterator otherMineCartIter = m_mineCarts.begin();
        while (true)
        {
            if (mineCartIter != otherMineCartIter && otherMineCartIter->getCurrentPosition() == mineCartIter->getCurrentPosition())
            {
                break;
            }
            ++otherMineCartIter;
        }

        m_mineCarts.erase(otherMineCartIter);
        return m_mineCarts.erase(mineCartIter);
    }

    void reorientMineCart(MineCart& mineCart)
    {
        const auto& position = mineCart.getCurrentPosition();
        auto railPiece = m_mineMap[position.second][position.first];
        railPiece->applyOrientation(mineCart);
    }
};

std::string coordinatesToString(const Coordinates& coordinates)
{
    return std::to_string(coordinates.first) + "," + std::to_string(coordinates.second);
}

bool isCharMineCart(char c)
{
    static const std::unordered_set<char> mineCartChars{'<', '>', '^', 'v'};

    return mineCartChars.find(c) != mineCartChars.end();
}

Orientation parseOrientation(char c)
{
    static const std::unordered_map<char, Orientation> charToOrientation =
    {
        {'<', Orientation::LEFT},
        {'>', Orientation::RIGHT},
        {'^', Orientation::UP},
        {'v', Orientation::DOWN}
    };

    return charToOrientation.at(c);
}

RailPieceSharedPtr parseRailPieceSharedPtr(char c)
{
    RailPieceSharedPtr result;

    switch (c)
    {
        case '|':
        case '-':
        case '^':
        case 'v':
        case '<':
        case '>':
            result = std::make_shared<PassiveRailPiece>();
            break;

        case '\\':
            result = std::make_shared<MainDiagonalRailPiece>();
            break;

        case '/':
            result = std::make_shared<AntiDiagonalRailPiece>();
            break;

        case '+':
            result = std::make_shared<IntersectionRailPiece>();
            break;

        default:
            result = std::make_shared<EmptyRailPiece>();
    }

    return result;
}


std::list<MineCart> parseMineCarts(const std::vector<std::string>& mapLines)
{
    std::list<MineCart> mineCarts;

    for (size_t j = 0; j < mapLines.size(); ++j)
    {
        for (size_t i = 0; i < mapLines[j].size(); ++i)
        {
            char c = mapLines[j][i];
            if (isCharMineCart(c))
            {
                Orientation orientation = parseOrientation(c);

                MineCart mineCart{orientation, Coordinates{i, j}};
                mineCarts.push_back(std::move(mineCart));
            }
        }
    }

    return mineCarts;
}

MineMap parseMineMap(const std::vector<std::string>& mapLines)
{
    MineMap mineMap;

    for (const auto& lines : mapLines)
    {
        MineMapLine currentMineMapLine;
        for (const char c : lines)
        {
            currentMineMapLine.push_back(parseRailPieceSharedPtr(c));
        }
        mineMap.push_back(std::move(currentMineMapLine));
    }

    return mineMap;
}

MineManager parseMineManager(const std::vector<std::string>& mapLines)
{
    MineMap mineMap = parseMineMap(mapLines);
    std::list<MineCart> mineCarts = parseMineCarts(mapLines);
    return MineManager{std::move(mineMap), std::move(mineCarts)};
}

std::string positionOfFirstCollision(const std::vector<std::string>& mapLines)
{
    MineManager mineManager = parseMineManager(mapLines);

    auto coordinatesOfFirstCollision = mineManager.coordinatesOfFirstCollision();

    return coordinatesToString(coordinatesOfFirstCollision);
}

std::string positionOfLastRemainingCart(const std::vector<std::string>& mapLines)
{
    MineManager mineManager = parseMineManager(mapLines);

    auto coordinatesOfLastRemainingCart = mineManager.coordinatesOfLastRemainingCart();

    return coordinatesToString(coordinatesOfLastRemainingCart);
}

}
