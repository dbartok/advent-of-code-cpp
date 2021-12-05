#include "Day04-GiantSquid.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day04
{

using NumberTable = std::vector<std::vector<int>>;

class Board
{
public:
    Board(NumberTable numberTable)
        : m_numberTable(std::move(numberTable))
    {

    }

    void markNumber(int number)
    {
        m_markedNumbers.push_back(number);
        if (!m_hasWon)
        {
            m_hasWon = isAnyRowCompleted() || isAnyColumnCompleted();
            m_hasJustWon = m_hasWon;
        }
        else
        {
            m_hasJustWon = false;
        }
    }

    bool hasWon() const
    {
        return m_hasWon;
    }

    bool hasJustWon() const
    {
        return m_hasJustWon;
    }

    int getScore() const
    {
        int sumOfUnmarkedNumbers = std::accumulate(m_numberTable.cbegin(), m_numberTable.cend(), 0, [this](const auto& acc, const auto& row)
                                                   {
                                                       return acc + std::accumulate(row.cbegin(), row.cend(), 0, [this](const auto& acc, auto number)
                                                                                    {
                                                                                        if (!this->isMarked(number))
                                                                                        {
                                                                                            return acc + number;
                                                                                        }

                                                                                        return acc;
                                                                                    });
                                                   });

        return sumOfUnmarkedNumbers * m_markedNumbers.back();
    }

private:
    NumberTable m_numberTable;
    std::vector<int> m_markedNumbers;

    bool m_hasWon = false;
    bool m_hasJustWon = false;

    bool isAnyRowCompleted() const
    {
        for (size_t j = 0; j < m_numberTable.size(); ++j)
        {
            if (isRowCompleted(j))
            {
                return true;
            }
        }

        return false;
    }

    bool isAnyColumnCompleted() const
    {
        for (size_t i = 0; i < m_numberTable.at(0).size(); ++i)
        {
            if (isColumnCompleted(i))
            {
                return true;
            }
        }

        return false;
    }

    bool isRowCompleted(int rowIndex) const
    {
        for (size_t i = 0; i < m_numberTable.at(0).size(); ++i)
        {
            if (!isMarked(m_numberTable.at(rowIndex).at(i)))
            {
                return false;
            }
        }

        return true;
    }


    bool isColumnCompleted(int columnIndex) const
    {
        for (size_t j = 0; j < m_numberTable.size(); ++j)
        {
            if (!isMarked(m_numberTable.at(j).at(columnIndex)))
            {
                return false;
            }
        }

        return true;
    }

    bool isMarked(int number) const
    {
        return std::find(m_markedNumbers.cbegin(), m_markedNumbers.cend(), number) != m_markedNumbers.cend();
    }
};

class BingoSubsystem
{
public:
    BingoSubsystem(std::vector<int> numbersDrawn, std::vector<Board> boards)
        : m_numbersDrawn(numbersDrawn)
        , m_boards{std::move(boards)}
    {

    }

    void playUntilFirstWinner()
    {
        for (auto number : m_numbersDrawn)
        {
            processNumberDrawn(number);
            if (hasAnyBoardWon())
            {
                return;
            }
        }

        throw std::runtime_error("No winning board");
    }

    void playUntilLastWinner()
    {
        for (auto number : m_numbersDrawn)
        {
            processNumberDrawn(number);
            if (haveAllBoardsWon())
            {
                return;
            }
        }

        throw std::runtime_error("Not all boards have won");
    }

    int getLatestToWinBoardScore() const
    {
        const auto lastToWinBoardIter = std::find_if(m_boards.cbegin(), m_boards.cend(), [](const auto& board)
                                                   {
                                                       return board.hasJustWon();
                                                   });

        if (lastToWinBoardIter == m_boards.cend())
        {
            throw std::runtime_error("No boards have just won");
        }

        return lastToWinBoardIter->getScore();
    }

private:
    std::vector<int> m_numbersDrawn;
    std::vector<Board> m_boards;

    void processNumberDrawn(int number)
    {
        for (auto& board : m_boards)
        {
            board.markNumber(number);
            if (board.hasWon())
            {

            }
        }
    }

    bool hasAnyBoardWon() const
    {
        return std::any_of(m_boards.cbegin(), m_boards.cend(), [](const auto& board)
                           {
                               return board.hasWon();
                           });
    }

    bool haveAllBoardsWon() const
    {
        return std::all_of(m_boards.cbegin(), m_boards.cend(), [](const auto& board)
                           {
                               return board.hasWon();
                           });
    }
};

using TextSection = std::vector<std::string>;

Board parseBoardTextSection(const TextSection& boardTextSection)
{
    NumberTable numberTable;

    for (const auto& line : boardTextSection)
    {
        std::vector<int> numbers;
        AdventOfCode::Utils::splitStringIntoTypedVector(line, numbers);
        numberTable.push_back(std::move(numbers));
    }

    return Board{std::move(numberTable)};
}

std::vector<Board> parseBoardTextSections(const std::vector<TextSection>& boardTextSections)
{
    std::vector<Board> boards;

    for (const auto& boardTextSection : boardTextSections)
    {
        auto board = parseBoardTextSection(boardTextSection);
        boards.push_back(std::move(board));
    }

    return boards;
}

BingoSubsystem parseBingoSubsystemInputLines(const std::vector<std::string>& bingoSubsystemInputLines)
{
    std::vector<TextSection> bingoSubsystemInputTextSections;
    boost::split(bingoSubsystemInputTextSections, bingoSubsystemInputLines, [](const auto& elem) { return elem.empty(); });

    std::vector<int> numbersDrawn;
    AdventOfCode::Utils::splitStringIntoTypedVector(bingoSubsystemInputTextSections.at(0).at(0), numbersDrawn, ',');

    std::vector<TextSection> boardTextSections(std::next(bingoSubsystemInputTextSections.cbegin()), bingoSubsystemInputTextSections.cend());
    std::vector<Board> boards = parseBoardTextSections(boardTextSections);

    return BingoSubsystem{std::move(numbersDrawn), std::move(boards)};
}

int winningBoardFinalScore(const std::vector<std::string>& bingoSubsystemInputLines)
{
    BingoSubsystem bingoSubsystem = parseBingoSubsystemInputLines(bingoSubsystemInputLines);
    bingoSubsystem.playUntilFirstWinner();
    return bingoSubsystem.getLatestToWinBoardScore();
}

int lastToWinBoardFinalScore(const std::vector<std::string>& bingoSubsystemInputLines)
{
    BingoSubsystem bingoSubsystem = parseBingoSubsystemInputLines(bingoSubsystemInputLines);
    bingoSubsystem.playUntilLastWinner();
    return bingoSubsystem.getLatestToWinBoardScore();
}

}
}
}