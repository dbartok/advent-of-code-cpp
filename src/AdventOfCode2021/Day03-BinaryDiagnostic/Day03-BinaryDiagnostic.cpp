#include "Day03-BinaryDiagnostic.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <stdexcept>
#include <array>
#include <functional>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{
namespace Year2021
{
namespace Day03
{

using ZeroAndOneFrequencies = std::array<int, 2>;

class DiagnosticReportAnalyzer
{
public:
    DiagnosticReportAnalyzer(std::vector<std::string> diagnosticReportLines)
        : m_diagnosticReportLines(std::move(diagnosticReportLines))
    {
    
    }

    void populateFrequenciesForAllPositions()
    {
        for (size_t i = 0; i < m_diagnosticReportLines.at(0).size(); ++i)
        {
            populateFrequenciesForPosition(i);
        }
    }

    int getSubmarinePowerConsumption() const
    {
        const int gammaRate = getBitExtractedNumber([](const auto& frequencies)
                                                    {
                                                        if (frequencies.at(0) > frequencies.at(1))
                                                        {
                                                            return 0;
                                                        }
                                                        return 1;
                                                    });

        const int epsilonRate = getBitExtractedNumber([](const auto& frequencies)
                                                      {
                                                          if (frequencies.at(0) > frequencies.at(1))
                                                          {
                                                              return 1;
                                                          }
                                                          return 0;
                                                      });

        return gammaRate * epsilonRate;
    }

private:
    std::vector<std::string> m_diagnosticReportLines;
    std::vector<ZeroAndOneFrequencies> m_perPositionFrequencies;

    void populateFrequenciesForPosition(size_t index)
    {
        m_perPositionFrequencies.resize(std::max(m_perPositionFrequencies.size(), index + 1));
        auto& frequencies = m_perPositionFrequencies.at(index);

        for (const auto& diagnosticReportLine : m_diagnosticReportLines)
        {
            char diagnosticReportChar = diagnosticReportLine.at(index);
            int binaryDigit = diagnosticReportChar - '0';
            if (!(binaryDigit == 0 || binaryDigit == 1))
            {
                throw std::runtime_error("Invalid binary digit: " + diagnosticReportChar);
            }
            ++frequencies.at(binaryDigit);
        }
    }

    int getBitExtractedNumber(const std::function<int(ZeroAndOneFrequencies)>& frequencySelectorFunc) const
    {
        std::string extractedNumberString;

        int extractedNumber = 0;

        for (const auto& frequencies : m_perPositionFrequencies)
        {
            extractedNumber <<= 1;
            const int extractedDigit = frequencySelectorFunc(frequencies);
            extractedNumber += extractedDigit;
        }

        return extractedNumber;
    }
};

int submarinePowerConsumption(const std::vector<std::string>& diagnosticReportLines)
{
    DiagnosticReportAnalyzer analyzer{diagnosticReportLines};
    analyzer.populateFrequenciesForAllPositions();
    return analyzer.getSubmarinePowerConsumption();
}

}
}
}