#include "Day14-RegolithReservoir.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>
#include <boost/functional/hash.hpp>
#include <boost/algorithm/string.hpp>

#include <unordered_set>
#include <algorithm>
#include <iterator>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2022
{
namespace Day14
{

using Coordinates = std::pair<int, int>;
using CoordinatesSet = std::unordered_set<Coordinates, boost::hash<Coordinates>>;

const Coordinates SAND_START_COORDINATES = {500, 0};
const int SIMULATION_BOTTOM_Y_COORDINATE = 1'000;

class SandFlowSimulator
{
public:
    SandFlowSimulator(CoordinatesSet allRockCoordinates)
        : m_allOccupiedCoordinates{std::move(allRockCoordinates)}
    {

    }

    void simulate()
    {
        while (true)
        {
            if (!trySettleNextSandParticle())
            {
                break;
            }
            ++m_numSettledUnits;
        }
    }

    unsigned getNumSettledUnits()
    {
        return m_numSettledUnits;
    }

private:
    CoordinatesSet m_allOccupiedCoordinates;

    unsigned m_numSettledUnits = 0;

    bool trySettleNextSandParticle()
    {
        Coordinates currentSandCoordinates = SAND_START_COORDINATES;

        while (true)
        {
            boost::optional<Coordinates> nextSandCoordinates = getNextSandCoordinates(currentSandCoordinates);

            // Sand particle has settled
            if (!nextSandCoordinates)
            {
                m_allOccupiedCoordinates.insert(currentSandCoordinates);
                return true;
            }
            // Sand particle will never settle
            else if (currentSandCoordinates.second > SIMULATION_BOTTOM_Y_COORDINATE)
            {
                return false;
            }
            // Move sand particle to next position
            else
            {
                currentSandCoordinates = nextSandCoordinates.get();
            }
        }
    }

    boost::optional<Coordinates> getNextSandCoordinates(const Coordinates& currentCoordinates)
    {
        std::vector<Coordinates> nextCoordinatesInOrderOfPreference =
        {
            {currentCoordinates.first, currentCoordinates.second + 1}, // Down
            {currentCoordinates.first - 1, currentCoordinates.second + 1}, // Down-left
            {currentCoordinates.first + 1, currentCoordinates.second + 1}, // Down-right
        };

        for (const auto& nextCoordinates : nextCoordinatesInOrderOfPreference)
        {
            if (m_allOccupiedCoordinates.find(nextCoordinates) == m_allOccupiedCoordinates.cend())
            {
                return nextCoordinates;
            }
        }

        return boost::none;
    }
};

std::vector<Coordinates> getAllCoordinatesDescribingPath(const std::string& rockPathLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, rockPathLine, boost::is_any_of(" ->,"), boost::token_compress_on);

    std::vector<Coordinates> allCoordinatesDescribingPath;

    for (size_t i = 0; i < tokens.size(); i += 2)
    {
        allCoordinatesDescribingPath.emplace_back(std::stoi(tokens.at(i)), std::stoi(tokens.at(i + 1)));
    }

    return allCoordinatesDescribingPath;
}

int getStep(int from, int to)
{
    if (from == to)
    {
        return 0;
    }

    return (to - from) / std::abs(to - from);
}

CoordinatesSet getAllRockCoordinatesInPathSection(const Coordinates& fromCoordinates, const Coordinates& toCoordinates)
{
    CoordinatesSet allRockCoordinatesInPathSection;

    const int deltaX = getStep(fromCoordinates.first, toCoordinates.first);
    const int deltaY = getStep(fromCoordinates.second, toCoordinates.second);

    for (Coordinates c = fromCoordinates; c != toCoordinates; c.first += deltaX, c.second += deltaY)
    {
        allRockCoordinatesInPathSection.insert(c);
    }

    allRockCoordinatesInPathSection.insert(toCoordinates);

    return allRockCoordinatesInPathSection;
}

CoordinatesSet getAllRockCoordinatesInPath(const std::vector<Coordinates>& rockPath)
{
    CoordinatesSet allRockCoordinatesInPath;

    for (auto coordinatesIter = rockPath.cbegin(); coordinatesIter < rockPath.cend() - 1; ++coordinatesIter)
    {
        const auto& fromCoordinates = *coordinatesIter;
        const auto& toCoordinates = *std::next(coordinatesIter);

        CoordinatesSet allRockCoordinatesInPathSection = getAllRockCoordinatesInPathSection(fromCoordinates, toCoordinates);
        allRockCoordinatesInPath.insert(std::make_move_iterator(allRockCoordinatesInPathSection.begin()), std::make_move_iterator(allRockCoordinatesInPathSection.end()));
    }

    return allRockCoordinatesInPath;
}

CoordinatesSet parseRockPathLine(const std::string& rockPathLine)
{
    std::vector<Coordinates> rockPath = getAllCoordinatesDescribingPath(rockPathLine);
    return getAllRockCoordinatesInPath(rockPath);
}

CoordinatesSet parseRockPathLines(const std::vector<std::string>& rockPathLines)
{
    CoordinatesSet allRockCoordinatesInPaths;

    for (const auto& rockPathLine : rockPathLines)
    {
        CoordinatesSet allRockCoordinatesInSinglePath = parseRockPathLine(rockPathLine);
        allRockCoordinatesInPaths.insert(std::make_move_iterator(allRockCoordinatesInSinglePath.begin()), std::make_move_iterator(allRockCoordinatesInSinglePath.end()));
    }

    return allRockCoordinatesInPaths;
}

unsigned numSettledUnitsBeforeSandFlowsIntoAbyss(const std::vector<std::string>& rockPathLines)
{
    CoordinatesSet allRockCoordinates = parseRockPathLines(rockPathLines);

    SandFlowSimulator sandFlowSimulator{std::move(allRockCoordinates)};

    sandFlowSimulator.simulate();

    return sandFlowSimulator.getNumSettledUnits();
}

}
}
}