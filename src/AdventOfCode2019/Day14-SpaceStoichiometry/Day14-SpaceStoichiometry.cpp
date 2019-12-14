#include "Day14-SpaceStoichiometry.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <cmath>
#include <deque>
#include <unordered_map>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const char* FUEL = "FUEL";
const char* ORE = "ORE";
}

namespace AdventOfCode
{

struct QuantifiedChemical
{
    std::string name;
    int amount;
};

struct ChemicalReaction
{
    std::vector<QuantifiedChemical> inputs;
    QuantifiedChemical output;
    int currentResidueRemaining;
};

using ChemicalNameToSourceReaction = std::unordered_map<std::string, ChemicalReaction>;

class FuelProducer
{
public:
    FuelProducer(std::vector<ChemicalReaction> reactions)
    {
        for (auto& reaction : reactions)
        {
            m_chemicalNameToSourceReaction.emplace(reaction.output.name, std::move(reaction));
        }
    }

    void produceFuel()
    {
        std::deque<QuantifiedChemical> workOrders;
        workOrders.push_back({FUEL, 1});

        while (!workOrders.empty())
        {
            QuantifiedChemical nextChemicalToProduce = workOrders.front();
            workOrders.pop_front();

            if (nextChemicalToProduce.name == ORE)
            {
                m_oreConsumed += nextChemicalToProduce.amount;
                continue;
            }

            std::vector<QuantifiedChemical> ingredients = calculateIngredients(nextChemicalToProduce);
            std::copy(std::make_move_iterator(ingredients.begin()), std::make_move_iterator(ingredients.end()), std::back_inserter(workOrders));
        }
    }

    int getNumOreConsumed() const
    {
        return m_oreConsumed;
    }

private:
    ChemicalNameToSourceReaction m_chemicalNameToSourceReaction;

    int m_oreConsumed = 0;

    std::vector<QuantifiedChemical> calculateIngredients(const QuantifiedChemical& chemicalToProduce)
    {
        ChemicalReaction& sourceReaction = m_chemicalNameToSourceReaction.at(chemicalToProduce.name);

        std::vector<QuantifiedChemical> ingredients = sourceReaction.inputs;

        if (sourceReaction.currentResidueRemaining >= chemicalToProduce.amount)
        {
            sourceReaction.currentResidueRemaining -= chemicalToProduce.amount;
            return {};
        }

        int amountToProduce = chemicalToProduce.amount - sourceReaction.currentResidueRemaining;

        int numReactionsRequired = std::ceil(amountToProduce / static_cast<double>(sourceReaction.output.amount));

        sourceReaction.currentResidueRemaining = numReactionsRequired * sourceReaction.output.amount - amountToProduce;

        for (auto& ingredient : ingredients)
        {
            ingredient.amount *= numReactionsRequired;
        }

        return ingredients;
    }
};

QuantifiedChemical createQuantifiedChemical(std::string quantifiedChemicalString)
{
    boost::trim(quantifiedChemicalString);

    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, quantifiedChemicalString, boost::is_any_of(", "), boost::token_compress_on);

    if (tokens.size() != 2)
    {
        throw std::runtime_error("Invalid quantified chemical: " + quantifiedChemicalString);
    }

    int amount = std::stoi(tokens.front());

    return QuantifiedChemical{std::move(tokens.back()), amount};
}

std::vector<QuantifiedChemical> createInputs(const std::string& inputString)
{
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, inputString, boost::is_any_of(","));

    std::vector<QuantifiedChemical> quantifiedChemicals;
    for (const auto& quantifiedChemicalString : tokens)
    {
        QuantifiedChemical quantifiedChemical = createQuantifiedChemical(quantifiedChemicalString);
        quantifiedChemicals.push_back(std::move(quantifiedChemical));
    }

    return quantifiedChemicals;
}

ChemicalReaction createChemicalReaction(const std::string& reactionLine)
{
    std::vector<std::string> tokens;
    boost::algorithm::split(tokens, reactionLine, boost::is_any_of("=>"), boost::token_compress_on);

    if (tokens.size() != 2)
    {
        throw std::runtime_error("Invalid line: " + reactionLine);
    }

    std::vector<QuantifiedChemical> inputs = createInputs(tokens.front());
    QuantifiedChemical output = createQuantifiedChemical(tokens.back());

    return ChemicalReaction{std::move(inputs), std::move(output), 0};
}

std::vector<ChemicalReaction> createChemicalReactions(const std::vector<std::string>& reactionLines)
{
    std::vector<ChemicalReaction> chemicalReactions;

    for (const auto& line : reactionLines)
    {
        ChemicalReaction chemicalReaction = createChemicalReaction(line);
        chemicalReactions.push_back(std::move(chemicalReaction));
    }

    return chemicalReactions;
}

int minOreRequiredToProduceFuel(const std::vector<std::string>& reactionLines)
{
    std::vector<ChemicalReaction> chemicalReactions = createChemicalReactions(reactionLines);

    FuelProducer producer{std::move(chemicalReactions)};

    producer.produceFuel();

    return producer.getNumOreConsumed();
}

}
