#include "Day21-AllergenAssessment.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/algorithm/string.hpp>

#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
__END_LIBRARIES_DISABLE_WARNINGS

using SortedStringVector = std::vector<std::string>;

namespace AdventOfCode
{

struct Food
{
    std::vector<std::string> ingredients;
    std::vector<std::string> markedAllergens;
};

class FoodListAnalyzer
{
public:
    FoodListAnalyzer(std::vector<Food> foods)
        : m_foods{std::move(foods)}
    {

    }

    void determineIngredientAllergenRelationship()
    {
        for (const auto& food : m_foods)
        {
            processIngredientAllergenRelationshipForFood(food);
        }
    }

    int getNumAppearancesOfIngredientsNotContainingAllergens() const
    {
        const std::unordered_set<std::string> excludedIngredients = getIngredientsPossiblyContainingAllergens();

        return std::accumulate(m_foods.cbegin(), m_foods.cend(), 0, [&excludedIngredients](int outerAcc, const auto& food)
                               {
                                   const auto& ingredients = food.ingredients;
                                   return outerAcc + std::accumulate(ingredients.cbegin(), ingredients.cend(), 0, [&excludedIngredients](int innerAcc, const auto& ingredient)
                                                                     {
                                                                         return innerAcc + (excludedIngredients.find(ingredient) == excludedIngredients.cend());
                                                                     });
                               });
    }

private:
    const std::vector<Food> m_foods;
    std::unordered_map<std::string, SortedStringVector> m_allergenToPossibleIngredients;

    void processIngredientAllergenRelationshipForFood(const Food& food)
    {
        for (const auto& allergen : food.markedAllergens)
        {
            updateIngredientConstraintsForAllergen(allergen, food.ingredients);
        }
    }

    void updateIngredientConstraintsForAllergen(const std::string& allergen, std::vector<std::string> possibleIngredients)
    {
        std::sort(possibleIngredients.begin(), possibleIngredients.end());
        auto insertionResult = m_allergenToPossibleIngredients.emplace(allergen, possibleIngredients);
        const auto wasInserted = insertionResult.second;
        if (wasInserted)
        {
            return;
        }

        auto& foundElement = *insertionResult.first;
        auto& preexistingIngredientConstraints = foundElement.second;

        SortedStringVector combinedIngredientConstraints;
        std::set_intersection(preexistingIngredientConstraints.cbegin(), preexistingIngredientConstraints.cend(),
                              possibleIngredients.cbegin(), possibleIngredients.cend(),
                              std::back_inserter(combinedIngredientConstraints));

        preexistingIngredientConstraints = combinedIngredientConstraints;
    }

    std::unordered_set<std::string> getIngredientsPossiblyContainingAllergens() const
    {
        std::unordered_set<std::string> ingredientsPossiblyContainingAllergens;

        for (const auto& allergenAndPossibleIngredients : m_allergenToPossibleIngredients)
        {
            for (const auto& possibleIngredient : allergenAndPossibleIngredients.second)
            {
                ingredientsPossiblyContainingAllergens.insert(possibleIngredient);
            }
        }

        return ingredientsPossiblyContainingAllergens;
    }
};

Food parseFoodLine(const std::string& foodLine)
{
    const std::regex foodLineRegex{R"(((?:[\w]+\s)+)\(contains\s((?:[\w]+(?:,\s)?)+)\))"};
    std::smatch matches;
    std::regex_search(foodLine, matches, foodLineRegex);

    if (matches.size() != 3)
    {
        throw std::runtime_error("Invalid food line: " + foodLine);
    }

    const std::string ingredientsString = matches[1];
    std::vector<std::string> ingredients;
    boost::split(ingredients, ingredientsString, boost::is_any_of(" "));

    const std::string markedAllergensString = matches[2];
    std::vector<std::string> markedAllergens;
    boost::split(markedAllergens, markedAllergensString, boost::is_any_of(", "), boost::token_compress_on);

    return Food{ingredients, markedAllergens};
}


std::vector<Food> parseFoodLines(const std::vector<std::string>& foodLines)
{
    std::vector<Food> foods;

    for(const auto& line : foodLines)
    {
        Food food = parseFoodLine(line);
        foods.push_back(std::move(food));
    }

    return foods;
}

int numAppearancesOfIngredientsNotContainingAllergens(const std::vector<std::string>& foodLines)
{
    std::vector<Food> foods = parseFoodLines(foodLines);
    FoodListAnalyzer foodListAnalyzer(std::move(foods));
    foodListAnalyzer.determineIngredientAllergenRelationship();
    return foodListAnalyzer.getNumAppearancesOfIngredientsNotContainingAllergens();
}

}
