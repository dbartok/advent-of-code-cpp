#include "Day20-TrenchMap.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <stdexcept>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{

unsigned NUM_ENHANCEMENT_STEPS_PART_ONE = 2;
unsigned NUM_ENHANCEMENT_STEPS_PART_TWO = 50;
unsigned SOME_OUT_OF_BOUNDS_COORDINATE = 100000;
char LIGHT_PIXEL_CHAR = '#';

}

namespace AdventOfCode
{
namespace Year2021
{
namespace Day20
{

using Pixel = bool;
using ImageRow = std::vector<Pixel>;
using Image = std::vector<ImageRow>;

class ImageEnhancer
{
public:
    ImageEnhancer(ImageRow enhancementAlgorithm, Image originalImage)
        : m_enhancementAlgorithm{std::move(enhancementAlgorithm)}
        , m_image{std::move(originalImage)}
        , m_width{m_image.at(0).size()}
        , m_height{m_image.size()}
    {

    }

    void enhanceRepatedly(unsigned numEnhancementSteps)
    {
        addPadding(numEnhancementSteps);

        for (unsigned step = 0; step < numEnhancementSteps; ++step)
        {
            enhance();
        }
    }

    unsigned getNumLitPixels()
    {
        if (m_areOutOfBoundsPixelsLit)
        {
            throw std::runtime_error("There are an infinite number of lit pixels");
        }

        unsigned numLitPixels = 0;

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                if (getCurrentPixelAt(i, j))
                {
                    ++numLitPixels;
                }
            }
        }

        return numLitPixels;
    }

private:
    ImageRow m_enhancementAlgorithm;
    Image m_image;

    size_t m_width;
    size_t m_height;

    bool m_areOutOfBoundsPixelsLit = false;

    void addPadding(unsigned paddingLength)
    {
        const size_t newWidth = m_width + 2 * paddingLength;
        const size_t newHeight = m_height + 2 * paddingLength;

        Image newImage = Image(newHeight, ImageRow(newWidth, m_areOutOfBoundsPixelsLit));

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                newImage.at(j + paddingLength).at(i + paddingLength) = m_image.at(j).at(i);
            }
        }

        m_image = newImage;
        m_width = newWidth;
        m_height = newHeight;
    }

    void enhance()
    {
        Image newImage{m_image};

        for (size_t j = 0; j < m_height; ++j)
        {
            for (size_t i = 0; i < m_width; ++i)
            {
                newImage.at(j).at(i) = getEnhancedPixelAt(i, j);
            }
        }

        m_areOutOfBoundsPixelsLit = getEnhancedPixelAt(SOME_OUT_OF_BOUNDS_COORDINATE, SOME_OUT_OF_BOUNDS_COORDINATE);

        m_image = newImage;
    }

    Pixel getEnhancedPixelAt(int x, int y) const
    {
        int enhancementIndex = 0;

        for (int j = y - 1; j <= y + 1; ++j)
        {
            for (int i = x - 1; i <= x + 1; ++i)
            {
                enhancementIndex *= 2;
                enhancementIndex += getCurrentPixelAt(i, j);
            }
        }

        return m_enhancementAlgorithm.at(enhancementIndex);
    }

    Pixel getCurrentPixelAt(int x, int y) const
    {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        {
            return m_areOutOfBoundsPixelsLit;
        }

        return m_image.at(y).at(x);
    }
};

ImageRow parseImageLine(const std::string& imageLine)
{
    ImageRow imageRow;

    for (const char c : imageLine)
    {
        if (c == LIGHT_PIXEL_CHAR)
        {
            imageRow.push_back(true);
        }
        else
        {
            imageRow.push_back(false);
        }
    }

    return imageRow;
}

ImageEnhancer parseImageEnhancementAlgorithmAndInputImageLines(const std::vector<std::string>& imageEnhancementAlgorithmAndInputImageLines)
{
    ImageRow enhancementAlgorithm = parseImageLine(imageEnhancementAlgorithmAndInputImageLines.at(0));

    Image image;

    for (auto lineIter = imageEnhancementAlgorithmAndInputImageLines.cbegin() + 2; lineIter != imageEnhancementAlgorithmAndInputImageLines.cend(); ++lineIter)
    {
        ImageRow imageRow = parseImageLine(*lineIter);
        image.push_back(std::move(imageRow));
    }

    return ImageEnhancer{std::move(enhancementAlgorithm), std::move(image)};
}

unsigned numPixelsLitAfterSmallEnhancement(const std::vector<std::string>& imageEnhancementAlgorithmAndInputImageLines)
{
    ImageEnhancer imageEnhancer = parseImageEnhancementAlgorithmAndInputImageLines(imageEnhancementAlgorithmAndInputImageLines);
    imageEnhancer.enhanceRepatedly(NUM_ENHANCEMENT_STEPS_PART_ONE);
    return imageEnhancer.getNumLitPixels();
}

unsigned numPixelsLitAfterLargeEnhancement(const std::vector<std::string>& imageEnhancementAlgorithmAndInputImageLines)
{
    ImageEnhancer imageEnhancer = parseImageEnhancementAlgorithmAndInputImageLines(imageEnhancementAlgorithmAndInputImageLines);
    imageEnhancer.enhanceRepatedly(NUM_ENHANCEMENT_STEPS_PART_TWO);
    return imageEnhancer.getNumLitPixels();
}

}
}
}