#include "Day13-MineCartMadness.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include <unordered_set>
#include <unordered_map>
#include <memory>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

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
    MineCart(Orientation orientation)
        : m_orientation{orientation}
        , m_turn{Turn::LEFT}
    {

    }

    Orientation getCurrentOrientation() const
    {
        return m_orientation;
    }

    void setCurrentOrientation(Orientation orientation)
    {
        m_orientation = orientation;
    }

    void turnToNextOrientation()
    {
        m_orientation = static_cast<Orientation>((static_cast<unsigned>(m_orientation) + static_cast<unsigned>(m_turn)) % 4);

        if (m_turn == Turn::LEFT)
        {
            m_turn = Turn::STRAIGHT;
        }
        else if (m_turn == Turn::STRAIGHT)
        {
            m_turn = Turn::RIGHT;
        }
        else if (m_turn == Turn::RIGHT)
        {
            m_turn = Turn::LEFT;
        }
    }

private:
    Orientation m_orientation;
    Turn m_turn;
};

class RailPiece;

using RailPieceSharedPtr = std::shared_ptr<RailPiece>;

class RailPiece
{
public:
    virtual void setOrientation(MineCart& mineCart) = 0;
};

class DeterministicOrientingRailPiece : public RailPiece
{
public:
    virtual void setOrientation(MineCart& mineCart)
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
    virtual void setOrientation(MineCart&)
    {

    }
};

class EmptyRailPiece : public RailPiece
{
    virtual void setOrientation(MineCart&)
    {
        throw std::runtime_error("Empty rail piece attempted to be used.");
    }
};

class IntersectionRailPiece : public RailPiece
{
    virtual void setOrientation(MineCart& mineCart)
    {
        mineCart.turnToNextOrientation();
    }
};

struct MineMapField
{
    RailPieceSharedPtr railPieceSharedPtr;
    boost::optional<MineCart> mineCart;

    MineMapField(RailPieceSharedPtr railPieceSharedPtr)
        : railPieceSharedPtr{std::move(railPieceSharedPtr)}
        , mineCart{}
    {

    }

    MineMapField(RailPieceSharedPtr railPieceSharedPtr, MineCart mineCart)
        : railPieceSharedPtr{std::move(railPieceSharedPtr)}
        , mineCart{std::move(mineCart)}
    {

    }
};

using MineMapLine = std::vector<MineMapField>;
using MineMap = std::vector<MineMapLine>;

using Coordinates = std::pair<unsigned, unsigned>;

class MineManager
{
public:
    MineManager(MineMap mineMap)
        : m_mineMap{std::move(mineMap)}
    {

    }

    Coordinates coordinatesOfFirstCollision()
    {
        while (true)
        {
            std::vector<Coordinates> mineCartCoordinates;

            for (size_t j = 0; j < m_mineMap.size(); ++j)
            {
                for (size_t i = 0; i < m_mineMap[j].size(); ++i)
                {
                    auto& currentMineMapField = m_mineMap[j][i];
                    auto& currentMineCartOpt = currentMineMapField.mineCart;
                    if (currentMineCartOpt.is_initialized())
                    {
                        mineCartCoordinates.push_back(Coordinates{i, j});
                    }
                }
            }

            for (const auto& currentCoordinates : mineCartCoordinates)
            {
                auto& currentMineMapField = m_mineMap[currentCoordinates.second][currentCoordinates.first];
                auto& currentMineCartOpt = currentMineMapField.mineCart;
                if (currentMineCartOpt.is_initialized())
                {
                    Coordinates newCoordinates = nextCoordinateForOrientation(currentCoordinates, currentMineCartOpt.get().getCurrentOrientation());

                    auto& nextMineMapField = m_mineMap[newCoordinates.second][newCoordinates.first];
                    auto& newMineCartOpt = nextMineMapField.mineCart;

                    if (newMineCartOpt.is_initialized())
                    {
                        return newCoordinates;
                    }

                    newMineCartOpt = currentMineCartOpt.get();
                    currentMineCartOpt.reset();

                    nextMineMapField.railPieceSharedPtr->setOrientation(newMineCartOpt.get());
                }
            }
        }
    }

private:
    MineMap m_mineMap;

    Coordinates nextCoordinateForOrientation(const Coordinates& c, Orientation orientation)
    {
        switch (orientation)
        {
            case Orientation::LEFT:
                return Coordinates{c.first - 1, c.second};
            case Orientation::RIGHT:
                return Coordinates{c.first + 1, c.second};
            case Orientation::UP:
                return Coordinates{c.first, c.second - 1};
            case Orientation::DOWN:
                return Coordinates{c.first, c.second + 1};
        }
    }
};

bool isMineCart(char c)
{
    static const std::unordered_set<char> mineCartChars{'<', '>', '^', 'v'};

    return mineCartChars.find(c) != mineCartChars.end();
}

RailPieceSharedPtr parseRailPieceSharedPtrFromRailPieceChar(char c)
{
    RailPieceSharedPtr result;

    switch (c)
    {
        case '-':
            result = std::make_shared<PassiveRailPiece>();
            break;

        case '|':
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

MineCart parseMineCartFromMineCartChar(char c)
{
    static const std::unordered_map<char, MineCart> charToMineCartMap =
    {
        {'<', MineCart{Orientation::LEFT}},
        {'>', MineCart{Orientation::RIGHT}},
        {'^', MineCart{Orientation::UP}},
        {'v', MineCart{Orientation::DOWN}}
    };

    return charToMineCartMap.at(c);
}

MineMapField parseMineMapFieldFromMineCartChar(char c)
{
    RailPieceSharedPtr railPieceSharedPtr = std::static_pointer_cast<RailPiece>(std::make_shared<PassiveRailPiece>());

    return MineMapField{railPieceSharedPtr, parseMineCartFromMineCartChar(c)};
}

MineMapField parseMineMapFieldFromRailPieceChar(char c)
{
    return MineMapField{parseRailPieceSharedPtrFromRailPieceChar(c)};
}

MineMapField parseMineMapField(char c)
{
    if (isMineCart(c))
    {
        return parseMineMapFieldFromMineCartChar(c);
    }
    else
    {
        return parseMineMapFieldFromRailPieceChar(c);
    }
}

MineMap parseMineMap(const std::vector<std::string>& mapLines)
{
    MineMap mineMap;

    for (const auto& lines : mapLines)
    {
        MineMapLine currentMineMapLine;
        for (const char c : lines)
        {
            currentMineMapLine.push_back(parseMineMapField(c));
        }
        mineMap.push_back(std::move(currentMineMapLine));
    }

    return mineMap;
}

std::string locationOfFirstCollision(const std::vector<std::string>& mapLines)
{
    MineMap mineMap = parseMineMap(mapLines);
    MineManager mineManager{std::move(mineMap)};

    auto coordinatesOfFirstCollision = mineManager.coordinatesOfFirstCollision();

    return boost::lexical_cast<std::string>(coordinatesOfFirstCollision.first) + "," + boost::lexical_cast<std::string>(coordinatesOfFirstCollision.second);
}

}
