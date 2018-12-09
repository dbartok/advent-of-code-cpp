#include "Day09-MarbleMania.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/lexical_cast.hpp>

#include <regex>
#include <vector>
#include <algorithm>
#include <list>
#include <cmath>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const unsigned LONGER_GAME_MULTIPLYING_FACTOR = 100;
}

namespace AdventOfCode
{

class MarbleGame
{
public:
    MarbleGame(unsigned numPlayers, unsigned lastMarbleScore)
        : m_playerToScore(numPlayers, 0)
        , m_lastMarbleScore{lastMarbleScore}
        , m_marbles{0}
        , m_currentMarbleIter{m_marbles.begin()}
    {

    }

    void play()
    {
        unsigned currentPlayerIdx = 0;

        for (unsigned currentMarbleScore = 1; currentMarbleScore <= m_lastMarbleScore; ++currentMarbleScore)
        {
            if (currentMarbleScore % 23 == 0)
            {
                m_playerToScore[currentPlayerIdx] += currentMarbleScore;

                stepCurrentMarbleIter(-7);
                m_playerToScore[currentPlayerIdx] += *m_currentMarbleIter;
                m_currentMarbleIter = m_marbles.erase(m_currentMarbleIter);
            }
            else
            {
                stepCurrentMarbleIter(1);
                auto insertionIter = m_currentMarbleIter;
                ++insertionIter;
                m_currentMarbleIter = m_marbles.insert(insertionIter, currentMarbleScore);
            }

            currentPlayerIdx = (currentPlayerIdx + 1) % m_playerToScore.size();
        }
    }

    unsigned getWinningsElfsScore() const
    {
        auto maxIter = std::max_element(m_playerToScore.cbegin(), m_playerToScore.cend());
        return *maxIter;
    }

private:
    std::vector<unsigned> m_playerToScore;
    unsigned m_lastMarbleScore;

    std::list<unsigned> m_marbles;
    std::list<unsigned>::iterator m_currentMarbleIter;

    void stepCurrentMarbleIter(int steps)
    {
        std::function<void(std::list<unsigned>::iterator)> iterFunc;
        if (steps > 0)
        {
            for (int i = 0; i < steps; ++i)
            {
                ++m_currentMarbleIter;
                if (m_currentMarbleIter == m_marbles.end())
                {
                    m_currentMarbleIter = m_marbles.begin();
                }
            }
        }
        else
        {
            for (int i = 0; i < -steps; ++i)
            {
                if (m_currentMarbleIter == m_marbles.begin())
                {
                    m_currentMarbleIter = m_marbles.end();
                }
                --m_currentMarbleIter;
            }
        }
    }
};

MarbleGame parseMarbleGame(const std::string& gameDescriptionLine, bool longerGame = false)
{
    std::regex numbersRegex("([0-9]+)");
    std::smatch matchResults;

    std::regex_search(gameDescriptionLine, matchResults, numbersRegex);
    unsigned numPlayers = boost::lexical_cast<unsigned>(matchResults[1]);

    std::string suffix = matchResults.suffix();
    std::regex_search(suffix, matchResults, numbersRegex);
    unsigned lastMarbleScore = boost::lexical_cast<unsigned>(matchResults[1]);

    if (longerGame)
    {
        lastMarbleScore *= LONGER_GAME_MULTIPLYING_FACTOR;
    }

    MarbleGame marbleGame{numPlayers, lastMarbleScore};

    return marbleGame;
}

unsigned winningElfsScore(const std::string& gameDescriptionLine)
{
    MarbleGame marbleGame = parseMarbleGame(gameDescriptionLine);

    marbleGame.play();

    return marbleGame.getWinningsElfsScore();
}

unsigned winningElfsScoreLongerGame(const std::string& gameDescriptionLine)
{
    MarbleGame marbleGame = parseMarbleGame(gameDescriptionLine, true);

    marbleGame.play();

    return marbleGame.getWinningsElfsScore();
}

}
