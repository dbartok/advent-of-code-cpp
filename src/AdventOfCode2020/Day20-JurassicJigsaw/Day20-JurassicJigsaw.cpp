#include "Day20-JurassicJigsaw.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>
#include <Eigen/Dense>

#include <regex>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

using Image = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;

class Tile
{
public:
    Tile(int id, Image image)
        : m_id{id}
        , m_image{std::move(image)}
    {

    }

    Tile()
        : m_id{-1}
    {

    }

    void rotateRight()
    {
        m_image.transposeInPlace();
        m_image.rowwise().reverseInPlace();
    }

    void flipHorizontally()
    {
        m_image.rowwise().reverseInPlace();
    }

    int getID() const
    {
        return m_id;
    }

    const Image& getImage() const
    {
        return m_image;
    }

    std::vector<Tile> getAllVariants() const
    {
        std::vector<Tile> variants;

        auto copy{*this};
        for (size_t i = 0; i < 4; ++i)
        {
            variants.push_back(copy);
            copy.flipHorizontally();
            variants.push_back(copy);
            copy.flipHorizontally();
            copy.rotateRight();
        }

        return variants;
    }

    bool operator==(const Tile& other) const
    {
        return this->m_id == other.m_id;
    }

private:
    int m_id = -1;
    Image m_image;
};

enum class Direction
{
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
};

class ImageReassembler
{
public:
    ImageReassembler(std::vector<Tile> scrambledTiles)
        : m_scrambledTiles{std::move(scrambledTiles)}
        , m_height{static_cast<size_t>(std::sqrt(m_scrambledTiles.size()))}
        , m_reassembledImage(m_height, std::vector<Tile>(m_height))
    {

    }

    void reassemble()
    {
        reassembleTopLeftCorner();
    }

    int64_t getCornerTileIDsMultiplied()
    {
        return m_cornerTileIDsMultiplied;
    }

private:
    std::vector<Tile> m_scrambledTiles;
    size_t m_height;
    std::vector<std::vector<Tile>> m_reassembledImage;
    int64_t m_cornerTileIDsMultiplied = 1;

    void reassembleTopLeftCorner()
    {
        Tile topLeftCorner;

        for (const auto& tile : m_scrambledTiles)
        {
            for (const auto& tileVariant : tile.getAllVariants())
            {
                if (isTileVariantTopLeftCorner(tileVariant))
                {
                    topLeftCorner = tileVariant;
                    m_cornerTileIDsMultiplied *= topLeftCorner.getID();
                    break;
                }
            }
        }

        m_reassembledImage.at(0).at(0) = topLeftCorner;
    }

    bool isTileVariantTopLeftCorner(const Tile& tileVariant)
    {
        std::vector<Tile> allPossibleNeighborsToTheLeft = findAllPossibleNeighborsInDirection(tileVariant, Direction::LEFT);
        std::vector<Tile> allPossibleNeighborsToUpwards = findAllPossibleNeighborsInDirection(tileVariant, Direction::UP);

        return allPossibleNeighborsToTheLeft.empty() && allPossibleNeighborsToUpwards.empty();
    }

    std::vector<Tile> findAllPossibleNeighborsInDirection(const Tile& anchorTileVariant, Direction direction)
    {
        std::vector<Tile> allPossibleNeighborsInDirection;
        for (const auto& tile : m_scrambledTiles)
        {
            if (tile == anchorTileVariant)
            {
                continue;
            }

            for (const auto& tileVariant : tile.getAllVariants())
            {
                if (doesTileVariantFitAnchorInDirection(anchorTileVariant, tileVariant, direction))
                {
                    allPossibleNeighborsInDirection.push_back(tileVariant);
                }
            }
        }

        return allPossibleNeighborsInDirection;
    }

    bool doesTileVariantFitAnchorInDirection(Tile anchorTileVariant, Tile tileVariant, Direction direction)
    {
        int numRightRotationsRequired = static_cast<int>(direction);

        while ((numRightRotationsRequired--) > 0)
        {
            anchorTileVariant.rotateRight();
            tileVariant.rotateRight();
        }

        return doesTileVariantFitAnchorToUpwards(anchorTileVariant, tileVariant);
    }

    bool doesTileVariantFitAnchorToUpwards(const Tile& anchorTileVariant, const Tile& tileVariant)
    {
        const auto& anchorTileVariantImage = anchorTileVariant.getImage();
        const auto& tileVariantImage = tileVariant.getImage();

        return tileVariantImage.row(tileVariantImage.rows() - 1) == anchorTileVariantImage.row(0);
    }
};

int createTileID(const std::string& tileIDLine)
{
    const std::regex idLineRegex{R"(Tile (\d+):)"};
    std::smatch matches;
    std::regex_search(tileIDLine, matches, idLineRegex);
    if (matches.size() != 2)
    {
        throw std::runtime_error("Invalid tile ID line: " + tileIDLine);
    }
    return std::stoi(matches[1]);
}

Image createImage(const std::vector<std::string>& imageLines)
{
    Image image{imageLines.size(), imageLines.at(0).size()};

    for (size_t j = 0; j < imageLines.size(); ++j)
    {
        for (size_t i = 0; i < imageLines.size(); ++i)
        {
            image(j, i) = (imageLines[j][i] == '#');
        }
    }

    return image;
}

Tile createTile(const std::vector<std::string>& tileLines)
{
    int tileID = createTileID(tileLines.front());

    std::vector<std::string> imageLines{std::next(tileLines.cbegin()), tileLines.cend()};
    Image image = createImage(imageLines);

    return Tile{std::move(tileID), std::move(image)};
}

std::vector<Tile> createTiles(const std::vector<std::string>& cameraArrayLines)
{
    std::vector<std::vector<std::string>> tokens;
    boost::split(tokens, cameraArrayLines, [](const auto& line) {return line.empty(); });

    std::vector<Tile> tiles;

    for (const auto& tileLines : tokens)
    {
        if (!tileLines.empty())
        {
            Tile tile = createTile(tileLines);
            tiles.push_back(std::move(tile));
        }
    }

    return tiles;
}

int64_t cornerTileIDsMultiplied(const std::vector<std::string>& cameraArrayLines)
{
    std::vector<Tile> tiles = createTiles(cameraArrayLines);
    ImageReassembler imageReassembler{std::move(tiles)};
    imageReassembler.reassemble();
    return imageReassembler.getCornerTileIDsMultiplied();
}

}
