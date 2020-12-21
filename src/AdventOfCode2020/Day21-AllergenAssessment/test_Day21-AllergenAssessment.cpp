#include "Day21-AllergenAssessment.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include "CppUnitTest.h"
__END_LIBRARIES_DISABLE_WARNINGS

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace AoC = AdventOfCode;

TEST_CLASS(Day21AllergenAssessment)
{
public:

    TEST_METHOD(numAppearancesOfIngredientsNotContainingAllergens_SimpleTests)
    {
        Assert::AreEqual(5, AoC::numAppearancesOfIngredientsNotContainingAllergens(m_lines));
    }

    TEST_METHOD(canonicalDangerousIngredientList_SimpleTests)
    {
        Assert::AreEqual(std::string{"mxmxvkd,sqjhc,fvjkl"}, AoC::canonicalDangerousIngredientList(m_lines));
    }

private:
    std::vector<std::string> m_lines =
    {
        "mxmxvkd kfcds sqjhc nhms (contains dairy, fish)",
        "trh fvjkl sbzzf mxmxvkd (contains dairy)",
        "sqjhc fvjkl (contains soy)",
        "sqjhc mxmxvkd sbzzf (contains fish)"
    };
};
