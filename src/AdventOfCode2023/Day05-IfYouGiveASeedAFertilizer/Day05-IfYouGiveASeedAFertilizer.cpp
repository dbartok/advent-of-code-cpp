#include "Day05-IfYouGiveASeedAFertilizer.h"

#include <AdventOfCodeCommon/Utils.h>

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <map>
#include <unordered_set>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2023
{
namespace Day05
{

struct MappingRange
{
    int64_t destinationFirstValue;
    int64_t sourceFirstValue;
    int64_t length;
};

using RawMapping = std::vector<MappingRange>;

class Mapping
{
public:
    Mapping()
    {

    }

    Mapping(const RawMapping& rawMapping)
    {
        m_compressedMapping = convertRawMappingToCompressedMapping(rawMapping);
    }

    void absorb(const Mapping& otherMapping)
    {
        if (m_compressedMapping.empty())
        {
            m_compressedMapping = otherMapping.m_compressedMapping;
            return;
        }

        std::vector<int64_t> newMappingIntervalLowerBounds = getNewLowerBoundsForAbsorbtion(otherMapping);

        IntervalLowerBoundToOffsetMap newCompressedMapping;

        // Add all lower bounds along with their new overall offsets to new map
        for (const int64_t newLowerBound : newMappingIntervalLowerBounds)
        {
            const int64_t originalMappedValue = getMappedValue(newLowerBound);
            const int64_t overallMappedValueWhenCombiningBothMappings = otherMapping.getMappedValue(originalMappedValue);
            const int64_t newOverallOffset = overallMappedValueWhenCombiningBothMappings - newLowerBound;

            newCompressedMapping.emplace(newLowerBound, newOverallOffset);
        }

        m_compressedMapping = newCompressedMapping;
    }

    int64_t getMappedValue(int64_t value) const
    {
        const auto upperBoundIter = m_compressedMapping.upper_bound(value);

        if (upperBoundIter == m_compressedMapping.cbegin())
        {
            return value;
        }

        const auto lowerBoundIter = std::prev(upperBoundIter);

        return value + lowerBoundIter->second;
    }

    std::vector<int64_t> getAllLowerBounds() const
    {
        std::vector<int64_t> allLowerBounds;

        for (const auto& mappingElement : m_compressedMapping)
        {
            allLowerBounds.push_back(mappingElement.first);
        }

        return allLowerBounds;
    }

private:
    // Concise representation of a mapping that only stores the lowest element where the mapping changes, along with the offset for that element
    // E.g., take the following mapping:
    // 0 -> 1 (change to 1 offset)
    // 1 -> 2
    // 2 -> 3
    // 3 -> 0 (change to -3 offset)
    // 4 -> 4 (change to 0 offset)
    // 5 -> 5
    // ...
    //
    // Concise representation in the form of element + offset pairs: [{0, 1}, {3, -3}, {4, 0}]
    using IntervalLowerBoundToOffsetMap = std::map<int64_t, int64_t>;

    IntervalLowerBoundToOffsetMap m_compressedMapping;


    std::vector<int64_t> getNewLowerBoundsForAbsorbtion(const Mapping& otherMapping) const
    {
        std::vector<int64_t> newMappingIntervalLowerBounds;

        // Carry forward existing lower bounds
        for (const auto& existingMappingElement : m_compressedMapping)
        {
            newMappingIntervalLowerBounds.push_back(existingMappingElement.first);
        }

        // Add new lower bounds based on the mapping that is absorbed
        for (const auto& otherMappingElement : otherMapping.m_compressedMapping)
        {
            const int64_t lowerBoundRelativeToOtherMapping = otherMappingElement.first;
            const int64_t lowerBoundRelativeToExistingOverallMapping = getReverseMappedValue(lowerBoundRelativeToOtherMapping);

            newMappingIntervalLowerBounds.push_back(lowerBoundRelativeToExistingOverallMapping);
        }

        return newMappingIntervalLowerBounds;
    }

    int64_t getReverseMappedValue(int64_t value) const
    {
        IntervalLowerBoundToOffsetMap compressedReverseLookupMapping;

        for (const auto& mappingElement : m_compressedMapping)
        {
            const int64_t invertedIntervalStart = mappingElement.first + mappingElement.second;
            const int64_t invertedOffset = -mappingElement.second;

            compressedReverseLookupMapping.emplace(invertedIntervalStart, invertedOffset);
        }

        Mapping reverseMapping;
        reverseMapping.m_compressedMapping = compressedReverseLookupMapping;
        return reverseMapping.getMappedValue(value);
    }

    static IntervalLowerBoundToOffsetMap convertRawMappingToCompressedMapping(const RawMapping& rawMapping)
    {
        IntervalLowerBoundToOffsetMap compressedMapping;

        for (const auto& mappingRange : rawMapping)
        {
            const int64_t offset = mappingRange.destinationFirstValue - mappingRange.sourceFirstValue;

            // Insert with overwrite to erase any default mapping
            compressedMapping[mappingRange.sourceFirstValue] = offset;

            // Insert without overwrite to only add the default mapping if there isn't a different mapping already
            compressedMapping.emplace(mappingRange.sourceFirstValue + mappingRange.length, 0);
        }

        return compressedMapping;
    }
};

class Almanac
{
public:
    Almanac(std::vector<int64_t> seeds, std::vector<Mapping> mappings)
        : m_seeds{std::move(seeds)}
        , m_mappings{std::move(mappings)}
    {

    }

    void buildOverallMapping()
    {
        for (const auto& mapping : m_mappings)
        {
            m_overallMapping.absorb(mapping);
        }
    }

    int64_t getLowestLocationNumberCorrespondingToSeed() const
    {
        const auto seedWithLowestLocationNumberIter = std::min_element(m_seeds.cbegin(), m_seeds.cend(), [this](const auto& lhs, const auto& rhs)
                                                                       {
                                                                           return this->m_overallMapping.getMappedValue(lhs) < this->m_overallMapping.getMappedValue(rhs);
                                                                       });

        return this->m_overallMapping.getMappedValue(*seedWithLowestLocationNumberIter);
    }

    int64_t getLowestLocationNumberCorrespondingToSeedWithSeedRanges() const
    {
        const std::unordered_set<int64_t> interestingSeedValues = getInterestingSeedValuesForSeedRanges();

        const auto seedWithLowestLocationNumberIter = std::min_element(interestingSeedValues.cbegin(), interestingSeedValues.cend(), [this](const auto& lhs, const auto& rhs)
                                                                       {
                                                                           return this->m_overallMapping.getMappedValue(lhs) < this->m_overallMapping.getMappedValue(rhs);
                                                                       });

        return this->m_overallMapping.getMappedValue(*seedWithLowestLocationNumberIter);
    }

private:
    std::vector<int64_t> m_seeds;
    std::vector<Mapping> m_mappings;

    Mapping m_overallMapping;

    std::unordered_set<int64_t> getInterestingSeedValuesForSeedRanges() const
    {
        const std::vector<int64_t> overallMappingLowerBounds = m_overallMapping.getAllLowerBounds();

        auto overallMappingLowerBoundIter = overallMappingLowerBounds.cbegin();
        auto seedIter = m_seeds.cbegin();

        std::unordered_set<int64_t> interestingSeedValues;

        while (overallMappingLowerBoundIter != overallMappingLowerBounds.cend() && seedIter != m_seeds.cend())
        {
            const int64_t overallMappingLowerBound = *overallMappingLowerBoundIter;

            const int64_t seedRangeStart = *seedIter;
            const int64_t seedRangeEnd = seedRangeStart + *std::next(seedIter) - 1;
            interestingSeedValues.insert(seedRangeStart);
            interestingSeedValues.insert(seedRangeEnd);

            if (seedRangeStart > overallMappingLowerBound)
            {
                ++overallMappingLowerBoundIter;
            }
            else if (overallMappingLowerBound >= seedRangeStart && overallMappingLowerBound <= seedRangeEnd)
            {
                interestingSeedValues.insert(overallMappingLowerBound);
                ++overallMappingLowerBoundIter;
            }
            else
            {
                seedIter += 2;
            }
        }

        return interestingSeedValues;
    }
};

using MappingTextSection = std::vector<std::string>;

std::vector<int64_t> parseSeedsLine(const std::string& seedsLine)
{
    std::vector<std::string> tokens;
    boost::split(tokens, seedsLine, boost::is_any_of(":"));

    std::vector<int64_t> seeds;
    AdventOfCode::Utils::splitStringIntoTypedVector(tokens.at(1), seeds);
    return seeds;
}

MappingRange parseMappingRangeLine(const std::string& mappingRangeLine)
{
    std::vector<int64_t> mappingRangeNumbers;
    AdventOfCode::Utils::splitStringIntoTypedVector(mappingRangeLine, mappingRangeNumbers);
    return {mappingRangeNumbers.at(0), mappingRangeNumbers.at(1), mappingRangeNumbers.at(2)};
}

Mapping parseMappingTextSection(const MappingTextSection& mappingTextSection)
{
    RawMapping rawMapping;

    for (int i = 1; i < mappingTextSection.size(); ++i)
    {
        MappingRange mappingRange = parseMappingRangeLine(mappingTextSection.at(i));
        rawMapping.push_back(std::move(mappingRange));
    }

    return Mapping{std::move(rawMapping)};
}

Almanac parseAlmanacLines(const std::vector<std::string>& almanacLines)
{
    std::vector<int64_t> seeds = parseSeedsLine(almanacLines.front());

    const std::vector<std::string> allMappingLines{almanacLines.cbegin() + 2, almanacLines.cend()};

    std::vector<MappingTextSection> mappingTextSections;
    boost::split(mappingTextSections, allMappingLines, [](const auto& elem) { return elem.empty(); });

    std::vector<Mapping> mappings;

    for (const auto& mappingTextSection : mappingTextSections)
    {
        Mapping mapping = parseMappingTextSection(mappingTextSection);
        mappings.push_back(std::move(mapping));
    }

    return {seeds, mappings};
}

int64_t lowestLocationNumberCorrespondingToSeed(const std::vector<std::string>& almanacLines)
{
    Almanac almanac = parseAlmanacLines(almanacLines);

    almanac.buildOverallMapping();

    return almanac.getLowestLocationNumberCorrespondingToSeed();
}

int64_t lowestLocationNumberCorrespondingToSeedWithSeedRanges(const std::vector<std::string>& almanacLines)
{
    Almanac almanac = parseAlmanacLines(almanacLines);

    almanac.buildOverallMapping();

    return almanac.getLowestLocationNumberCorrespondingToSeedWithSeedRanges();
}

}
}
}