#include "Day16-TheFloorWillBeLava.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <unordered_set>
#include <algorithm>
#include <stdexcept>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

const char MAIN_DIAGONAL_MIRROR = '\\';
const char SECONDARY_DIAGONAL_MIRROR = '/';
const char VERTICAL_SPLITTER = '|';
const char HORIZONTAL_SPLITTER = '-';

}

namespace AdventOfCode
{
namespace Year2023
{
namespace Day16
{

using Coordinates = std::pair<int, int>;

const Coordinates STARTING_POSITION{0, 0};
const Coordinates LEFT{-1, 0};
const Coordinates RIGHT{1, 0};
const Coordinates UP{0, -1};
const Coordinates DOWN{0, 1};

struct BeamHead
{
    Coordinates position;
    Coordinates direction;

    BeamHead(Coordinates position, Coordinates direction)
        : position{std::move(position)}
        , direction{std::move(direction)}
    {

    }

    bool operator==(const BeamHead& other) const
    {
        return position == other.position &&
            direction == other.direction;
    }
};

struct BeamHeadHash
{
    std::size_t operator()(const BeamHead& beamHead) const
    {
        std::size_t seed = 0;

        boost::hash_combine(seed, beamHead.position);
        boost::hash_combine(seed, beamHead.direction);

        return seed;
    }
};

class ContraptionAnalyzer
{
public:
    ContraptionAnalyzer(const std::vector<std::string>& map)
        : m_map{std::move(map)}
    {

    }

    void shootBeamFrom(const BeamHead& beamHead)
    {
        m_beamHeads.push_back(beamHead);

        while (!m_beamHeads.empty())
        {
            processAllBeamHeads();
        }
    }

    int getNumEnergizedTiles() const
    {
        std::unordered_set<Coordinates, boost::hash<Coordinates>> uniqueEnergizedTiles;

        for (const auto& beamHead : m_visited)
        {
            uniqueEnergizedTiles.insert(beamHead.position);
        }

        return uniqueEnergizedTiles.size();
    }

private:
    const std::vector<std::string> m_map;

    std::vector<BeamHead> m_beamHeads;
    std::unordered_set<BeamHead, BeamHeadHash> m_visited;

    void processAllBeamHeads()
    {
        std::vector<BeamHead> allNewBeamHeads;

        for (const auto& currentBeamHead : m_beamHeads)
        {
            bool wasInserted = m_visited.insert(currentBeamHead).second;
            if (!wasInserted)
            {
                continue;
            }

            std::vector<BeamHead> newBeamHeads = getNewBeamHeadsFor(currentBeamHead);
            allNewBeamHeads.insert(allNewBeamHeads.end(), std::make_move_iterator(newBeamHeads.begin()), std::make_move_iterator(newBeamHeads.end()));
        }

        m_beamHeads = allNewBeamHeads;
    }

    std::vector<BeamHead> getNewBeamHeadsFor(const BeamHead& currentBeamHead) const
    {
        const char tileAtCurrentPosition = m_map.at(currentBeamHead.position.second).at(currentBeamHead.position.first);

        std::vector<Coordinates> newDirections = getDirectionsForTile(tileAtCurrentPosition, currentBeamHead.direction);

        std::vector<BeamHead> newBeamHeads;
        for (const auto& newDirection : newDirections)
        {
            const Coordinates newPosition{currentBeamHead.position.first + newDirection.first, currentBeamHead.position.second + newDirection.second};

            if (newPosition.first >= 0 && newPosition.second >= 0 && newPosition.first < m_map.front().size() && newPosition.second < m_map.size())
            {
                newBeamHeads.emplace_back(newPosition, newDirection);
            }
        }
        return newBeamHeads;
    }

    static std::vector<Coordinates> getDirectionsForTile(char tile, const Coordinates& currentDirection)
    {
        switch (tile)
        {
            case MAIN_DIAGONAL_MIRROR:
                return getDirectionsForMainDiagonalMirror(currentDirection);
                break;
            case SECONDARY_DIAGONAL_MIRROR:
                return getDirectionsForSecondaryDiagonalMirror(currentDirection);
                break;
            case VERTICAL_SPLITTER:
                return getDirectionsForVerticalSplitter(currentDirection);
                break;
            case HORIZONTAL_SPLITTER:
                return getDirectionsForHorizontalSplitter(currentDirection);
                break;
            default:
                return {currentDirection};
                break;
        }
    }

    static std::vector<Coordinates> getDirectionsForMainDiagonalMirror(const Coordinates& currentDirection)
    {
        if (currentDirection == LEFT)
        {
            return {UP};
        }
        else if (currentDirection == UP)
        {
            return {LEFT};
        }
        else if (currentDirection == RIGHT)
        {
            return {DOWN};
        }
        else if (currentDirection == DOWN)
        {
            return {RIGHT};
        }

        throw std::runtime_error("Invalid direction");
    }

    static std::vector<Coordinates> getDirectionsForSecondaryDiagonalMirror(const Coordinates& currentDirection)
    {
        if (currentDirection == LEFT)
        {
            return {DOWN};
        }
        else if (currentDirection == DOWN)
        {
            return {LEFT};
        }
        else if (currentDirection == RIGHT)
        {
            return {UP};
        }
        else if (currentDirection == UP)
        {
            return {RIGHT};
        }

        throw std::runtime_error("Invalid direction");
    }

    static std::vector<Coordinates> getDirectionsForVerticalSplitter(const Coordinates& currentDirection)
    {
        if (currentDirection == LEFT || currentDirection == RIGHT)
        {
            return {UP, DOWN};
        }
        else
        {
            return {currentDirection};
        }
    }

    static std::vector<Coordinates> getDirectionsForHorizontalSplitter(const Coordinates& currentDirection)
    {
        if (currentDirection == UP || currentDirection == DOWN)
        {
            return {LEFT, RIGHT};
        }
        else
        {
            return {currentDirection};
        }
    }
};

std::vector<BeamHead> getInwardsPointingEdgeBeamHeads(const std::vector<std::string>& contraptionLines)
{
    std::vector<BeamHead> inwardsPointingEdgeBeamHeads;

    for (int i = 0; i < contraptionLines.front().size(); ++i)
    {
        inwardsPointingEdgeBeamHeads.emplace_back(Coordinates{i, 0}, DOWN);
    }

    for (int i = 0; i < contraptionLines.front().size(); ++i)
    {
        inwardsPointingEdgeBeamHeads.emplace_back(Coordinates{i, contraptionLines.size() - 1}, UP);
    }

    for (int j = 0; j < contraptionLines.size(); ++j)
    {
        inwardsPointingEdgeBeamHeads.emplace_back(Coordinates{0, j}, RIGHT);
    }

    for (int j = 0; j < contraptionLines.size(); ++j)
    {
        inwardsPointingEdgeBeamHeads.emplace_back(Coordinates{contraptionLines.front().size() - 1, j}, LEFT);
    }

    return inwardsPointingEdgeBeamHeads;
}

int numEnergizedTiles(const std::vector<std::string>& contraptionLines)
{
    ContraptionAnalyzer contraptionAnalyzer{contraptionLines};

    contraptionAnalyzer.shootBeamFrom({STARTING_POSITION, RIGHT});

    return contraptionAnalyzer.getNumEnergizedTiles();
}

int maxNumEnergizedTiles(const std::vector<std::string>& contraptionLines)
{
    std::vector<BeamHead> inwardsPointingEdgeBeamHeads = getInwardsPointingEdgeBeamHeads(contraptionLines);

    int maxNumEnergizedTiles = 0;

    for (const auto& beamHead : inwardsPointingEdgeBeamHeads)
    {
        ContraptionAnalyzer contraptionAnalyzer{contraptionLines};

        contraptionAnalyzer.shootBeamFrom(beamHead);

        maxNumEnergizedTiles = std::max(maxNumEnergizedTiles, contraptionAnalyzer.getNumEnergizedTiles());
    }

    return maxNumEnergizedTiles;
}

}
}
}