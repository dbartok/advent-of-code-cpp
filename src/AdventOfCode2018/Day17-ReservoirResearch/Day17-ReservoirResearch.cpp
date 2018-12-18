#include "Day17-ReservoirResearch.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash/hash.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>

#include <regex>
#include <unordered_map>
#include <algorithm>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

enum class Tile
{
    SAND,
    CLAY,
    FLOWING_WATER,
    SETTLED_WATER,
};

struct Coordinates
{
    int x;
    int y;

    Coordinates(int y, int x)
        : x{x}
        , y{y}
    {

    }

    bool operator==(const Coordinates& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct CoordinatesHash
{
    std::size_t operator()(const Coordinates& coordinates) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, coordinates.x);
        boost::hash_combine(seed, coordinates.y);

        return seed;
    }
};

using CoordinatesToTile = std::unordered_map<Coordinates, Tile, CoordinatesHash>;

class WaterFlowSimulator
{
public:
    WaterFlowSimulator(CoordinatesToTile clayCoordinates)
        : m_coordinatesToTile{std::move(clayCoordinates)}
    {
        auto minmaxIters = std::minmax_element(m_coordinatesToTile.cbegin(), m_coordinatesToTile.cend(),
                                               [](const auto& lhs, const auto& rhs)
                                               {
                                                   return lhs.first.y < rhs.first.y;
                                               });

        m_yMin = minmaxIters.first->first.y;
        m_yMax = minmaxIters.second->first.y;
    }

    void run()
    {
        startSpring({0, 500});
    }

    unsigned getNumTilesWithWater() const
    {
        return std::count_if(m_coordinatesToTile.cbegin(), m_coordinatesToTile.cend(), [this](const auto& elem)
                             {
                                 const Coordinates& coordinates = elem.first;
                                 Tile tile = getTileAt(coordinates);

                                 bool isWater = (tile == Tile::SETTLED_WATER || tile == Tile::FLOWING_WATER);
                                 bool isInBounds = (coordinates.y >= m_yMin) && (coordinates.y <= m_yMax);

                                 return isWater && isInBounds;
                             });
    }

    unsigned getNumTilesWithSettledWater() const
    {
        return std::count_if(m_coordinatesToTile.cbegin(), m_coordinatesToTile.cend(), [this](const auto& elem)
                             {
                                 return this->getTileAt(elem.first) == Tile::SETTLED_WATER;
                             });
    }

private:
    CoordinatesToTile m_coordinatesToTile;
    int m_yMin;
    int m_yMax;

    void startSpring(const Coordinates& coordinates)
    {
        for (int y = coordinates.y; y <= m_yMax; )
        {
            setTileAt({y, coordinates.x}, Tile::FLOWING_WATER);

            if (!tileIsFreeAt({y + 1, coordinates.x}))
            {
                spreadHorizontally({y, coordinates.x});

                if (getTileAt({y, coordinates.x}) == Tile::SETTLED_WATER)
                {
                    --y;
                }
                else
                {
                    break;
                }
            }
            else if (getTileAt({y + 1, coordinates.x}) == Tile::FLOWING_WATER)
            {
                break;
            }
            else
            {
                ++y;
            }
        }
    }

    void spreadHorizontally(const Coordinates& coordinates)
    {
        Coordinates leftBound = boundLeft(coordinates);
        Coordinates rightBound = boundRight(coordinates);

        if (!tileIsFreeAt(leftBound) && !tileIsFreeAt(rightBound))
        {
            fillUp(leftBound, rightBound, Tile::SETTLED_WATER);
        }
        else
        {
            fillUp(leftBound, rightBound, Tile::FLOWING_WATER);

            startSpringIfFree(leftBound);
            startSpringIfFree(rightBound);
        }
    }

    void fillUp(const Coordinates& leftWall, const Coordinates& rightWall, Tile tileValue)
    {
        int y = leftWall.y;
        for (int x = leftWall.x + 1; x < rightWall.x; ++x)
        {
            setTileAt({y, x}, tileValue);
        }
    }

    void startSpringIfFree(const Coordinates& coordinates)
    {
        if (tileIsFreeAt(coordinates))
        {
            startSpring(coordinates);
        }
    }

    Coordinates boundLeft(Coordinates coordinates)
    {
        return iterateUntil(std::move(coordinates), [](Coordinates& c) {--c.x; });
    }

    Coordinates boundRight(Coordinates coordinates)
    {
        return iterateUntil(std::move(coordinates), [](Coordinates& c) {++c.x; });
    }

    Coordinates iterateUntil(Coordinates coordinates, const std::function<void(Coordinates&)> iterFunc)
    {
        while (true)
        {
            if (getTileAt({coordinates.y, coordinates.x}) == Tile::CLAY || tileIsFreeAt({coordinates.y + 1, coordinates.x}))
            {
                return {coordinates.y, coordinates.x};
            }

            iterFunc(coordinates);
        }
    }


    Tile getTileAt(const Coordinates& coordinates) const
    {
        const auto findResult = m_coordinatesToTile.find(coordinates);
        if (findResult == m_coordinatesToTile.cend())
        {
            return Tile::SAND;
        }

        return findResult->second;
    }

    bool tileIsFreeAt(const Coordinates& coordinates) const
    {
        Tile tile = getTileAt({coordinates.y, coordinates.x});
        return (tile == Tile::SAND || tile == Tile::FLOWING_WATER);
    }

    void setTileAt(const Coordinates& coordinates, Tile newValue)
    {
        m_coordinatesToTile[coordinates] = newValue;
    }
};

struct Range
{
    int begin;
    int end;

    Range(int begin, int end)
        : begin(begin)
        , end(end)
    {

    }

    Range(int begin)
        : Range(begin, begin)
    {

    }
};

Range parseRangeWithPrefix(const std::string& line, const std::string& prefix)
{
    std::regex rangeRegex(prefix + "([0-9]+(\\.\\.[0-9]+)?)");
    std::smatch matchResults;
    regex_search(line, matchResults, rangeRegex);

    if (matchResults.size() < 2)
    {
        throw std::runtime_error("Could not find range.");
    }

    std::string match = matchResults[1].str();

    std::vector<std::string> tokens;
    boost::split(tokens, match, boost::is_any_of("."), boost::token_compress_on);

    if (tokens.size() == 1)
    {
        return Range{std::stoi(tokens[0])};
    }
    else if (tokens.size() == 2)
    {
        return Range{std::stoi(tokens[0]), std::stoi(tokens[1])};
    }
    else
    {
        throw std::runtime_error("Invalid range specified.");
    }
}


CoordinatesToTile parseClayCoordinates(const std::vector<std::string>& clayCoordinatesLines)
{
    CoordinatesToTile clayCoordinates;

    for (const auto& line : clayCoordinatesLines)
    {
        Range xRange = parseRangeWithPrefix(line, "x=");
        Range yRange = parseRangeWithPrefix(line, "y=");

        for (int j = yRange.begin; j <= yRange.end; ++j)
        {
            for (int i = xRange.begin; i <= xRange.end; ++i)
            {
                clayCoordinates.emplace(Coordinates{j, i}, Tile::CLAY);
            }
        }
    }

    return clayCoordinates;
}

unsigned numTilesWaterCanReach(const std::vector<std::string>& clayCoordinatesLines)
{
    CoordinatesToTile clayCoordinates = parseClayCoordinates(clayCoordinatesLines);

    WaterFlowSimulator waterFlowSimulator{std::move(clayCoordinates)};
    waterFlowSimulator.run();

    return waterFlowSimulator.getNumTilesWithWater();
}

unsigned numTilesWaterRetained(const std::vector<std::string>& clayCoordinatesLines)
{
    CoordinatesToTile clayCoordinates = parseClayCoordinates(clayCoordinatesLines);

    WaterFlowSimulator waterFlowSimulator{std::move(clayCoordinates)};
    waterFlowSimulator.run();

    return waterFlowSimulator.getNumTilesWithSettledWater();
}

}
