#include "Day13-CarePackage.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Coordinates = std::pair<int, int>;

enum class TileID
{
    EMPTY = 0,
    WALL = 1,
    BLOCK = 2,
    PADDLE = 3,
    BALL = 4,
};

using CoordinatesToTileID = std::unordered_map<Coordinates, TileID, boost::hash<Coordinates>>;

class ArcadeCabinet
{
public:
    ArcadeCabinet(IntcodeInterpreter interpreter)
        : m_interpreter{std::move(interpreter)}
    {

    }

    void play()
    {
        m_interpreter.execute();

        const auto& outputs = m_interpreter.getOutputs();
        assert(outputs.size() % 3 == 0);

        for (size_t i = 0; i < outputs.size(); i+=3)
        {
            int xCoord = outputs.at(i);
            int yCoord = outputs.at(i + 1);
            TileID tileID = static_cast<TileID>(outputs.at(i + 2));

            m_coordinatesToTileID[{xCoord, yCoord}] = tileID;
        }
    }

    unsigned getNumBlockTiles() const
    {
        return std::count_if(m_coordinatesToTileID.cbegin(), m_coordinatesToTileID.cend(), [](const auto& coordinatesAndTileID)
                             {
                                 return coordinatesAndTileID.second == TileID::BLOCK;
                             });
    }

private:
    IntcodeInterpreter m_interpreter;

    CoordinatesToTileID m_coordinatesToTileID;
};

unsigned numBlockTilesWhenGameExits(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    ArcadeCabinet arcadeCabinet{std::move(interpreter)};

    arcadeCabinet.play();

    return arcadeCabinet.getNumBlockTiles();
}

}
