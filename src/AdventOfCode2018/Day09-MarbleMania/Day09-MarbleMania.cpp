#include "Day09-MarbleMania.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/lexical_cast.hpp>

#include <regex>
#include <vector>
#include <algorithm>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class MarbleGame
{
public:
    MarbleGame(unsigned numPlayers, unsigned lastMarbleScore)
        : m_playerToScore(numPlayers, 0)
        , m_lastMarbleScore{lastMarbleScore}
        , m_marbles{0}
    {

    }

    void play()
    {
        unsigned currentMarbleIdx = 0;
        unsigned currentPlayerIdx = 0;

        for (unsigned currentMarbleScore = 1; currentMarbleScore <= m_lastMarbleScore; ++currentMarbleScore)
        {
            if (currentMarbleScore % 23 == 0)
            {
                m_playerToScore[currentPlayerIdx] += currentMarbleScore;

                currentMarbleIdx = (currentMarbleIdx - 7 + m_marbles.size()) % m_marbles.size();
                m_playerToScore[currentPlayerIdx] += m_marbles[currentMarbleIdx];
                m_marbles.erase(m_marbles.begin() + currentMarbleIdx);
            }
            else
            {
                currentMarbleIdx = ((currentMarbleIdx + 1) % m_marbles.size()) + 1;
                m_marbles.insert(m_marbles.begin() + currentMarbleIdx, currentMarbleScore);
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

    std::vector<unsigned> m_marbles;
};

MarbleGame parseMarbleGame(const std::string& gameDescriptionLine)
{
    std::regex numbersRegex("([0-9]+)");
    std::smatch matchResults;

    std::regex_search(gameDescriptionLine, matchResults, numbersRegex);
    unsigned numPlayers = boost::lexical_cast<unsigned>(matchResults[1]);

    std::string suffix = matchResults.suffix();
    std::regex_search(suffix, matchResults, numbersRegex);
    unsigned lastMarbleScore = boost::lexical_cast<unsigned>(matchResults[1]);

    MarbleGame marbleGame{numPlayers, lastMarbleScore};

    return marbleGame;
}

unsigned winningElfsScore(const std::string& gameDescriptionLine)
{
    MarbleGame marbleGame = parseMarbleGame(gameDescriptionLine);

    marbleGame.play();

    return marbleGame.getWinningsElfsScore();
}

}
