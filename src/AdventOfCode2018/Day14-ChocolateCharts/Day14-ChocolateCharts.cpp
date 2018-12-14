#include "Day14-ChocolateCharts.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <vector>
#include <sstream>
#include <iterator>
__END_LIBRARIES_DISABLE_WARNINGS

namespace
{
const unsigned NUM_ADDITIONAL_RECIPES = 10;
}

namespace AdventOfCode
{

std::string scoresOfTenRecipesAfterSeveralRecipes(unsigned numRecipes)
{
    std::vector<unsigned> recipes{3, 7};
    size_t elfOneIndex = 0;
    size_t elfTwoIndex = 1;

    while(recipes.size() < numRecipes + NUM_ADDITIONAL_RECIPES)
    {
        unsigned nextRecipe = recipes[elfOneIndex] + recipes[elfTwoIndex];

        if (nextRecipe >= 10)
        {
            recipes.push_back(1);
            nextRecipe -= 10;

            if (recipes.size() == numRecipes + NUM_ADDITIONAL_RECIPES)
            {
                break;
            }
        }

        recipes.push_back(nextRecipe);

        elfOneIndex = (elfOneIndex + recipes[elfOneIndex] + 1) % recipes.size();
        elfTwoIndex = (elfTwoIndex + recipes[elfTwoIndex] + 1) % recipes.size();
    }

    std::ostringstream recipesStream;
    std::copy(recipes.end() - NUM_ADDITIONAL_RECIPES, recipes.end(), std::ostream_iterator<unsigned>(recipesStream));

    return recipesStream.str();
}

}
