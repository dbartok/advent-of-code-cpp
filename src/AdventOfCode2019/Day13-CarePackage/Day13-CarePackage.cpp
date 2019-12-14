#include "Day13-CarePackage.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash.hpp>

#include <algorithm>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const int ENOUGH_QUARTERS_TO_PLAY = 2;

const int LEFT_INPUT = -1;
const int NEUTRAL_INPUT = 0;
const int RIGHT_INPUT = 1;
}

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
        , m_score{0}
    {

    }

    void play()
    {
        while (m_interpreter.getExecutionState() != IntcodeProgramExecutionState::TERMINATED)
        {
            m_interpreter.execute();

            const auto& outputs = m_interpreter.getOutputs();
            updateDisplay(outputs);

            m_interpreter.addInput(getNextInput());
            m_interpreter.clearOutputs();
        }
    }

    unsigned getNumBlockTiles() const
    {
        return std::count_if(m_coordinatesToTileID.cbegin(), m_coordinatesToTileID.cend(), [](const auto& coordinatesAndTileID)
                             {
                                 return coordinatesAndTileID.second == TileID::BLOCK;
                             });
    }

    int getScore() const
    {
        return m_score;
    }

private:
    IntcodeInterpreter m_interpreter;

    CoordinatesToTileID m_coordinatesToTileID;

    Coordinates m_ballPos;
    Coordinates m_paddlePos;

    int m_score;

    void updateDisplay(const std::vector<IntcodeNumberType>& outputs)
    {
        assert(outputs.size() % 3 == 0);
        for (size_t i = 0; i < outputs.size(); i += 3)
        {
            int xCoord = outputs.at(i);
            int yCoord = outputs.at(i + 1);
            int value = outputs.at(i + 2);

            if (xCoord == -1)
            {
                m_score = value;
                continue;
            }

            updatePixel({xCoord, yCoord}, value);
        }
    }

    void updatePixel(const Coordinates& coordinates, IntcodeNumberType value)
    {
        TileID tileID = static_cast<TileID>(value);
        if (tileID == TileID::BALL)
        {
            m_ballPos = coordinates;
        }
        else if (tileID == TileID::PADDLE)
        {
            m_paddlePos = coordinates;
        }

        m_coordinatesToTileID[coordinates] = tileID;
    }

    int getNextInput()
    {
        if (m_paddlePos.first < m_ballPos.first)
        {
            return RIGHT_INPUT;
        }
        else if (m_paddlePos.first > m_ballPos.first)
        {
            return LEFT_INPUT;
        }
        else
        {
            return NEUTRAL_INPUT;
        }
    }
};

unsigned numBlockTilesWhenGameExits(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    IntcodeInterpreter interpreter{intcodeProgram};

    ArcadeCabinet arcadeCabinet{std::move(interpreter)};

    arcadeCabinet.play();

    return arcadeCabinet.getNumBlockTiles();
}

unsigned scoreAfterLastBlockBroken(const std::vector<IntcodeNumberType>& intcodeProgram)
{
    auto playableProgram{intcodeProgram};
    playableProgram.at(0) = ENOUGH_QUARTERS_TO_PLAY;
    IntcodeInterpreter interpreter{std::move(playableProgram)};

    ArcadeCabinet arcadeCabinet{std::move(interpreter)};

    arcadeCabinet.play();

    return arcadeCabinet.getScore();
}

}
