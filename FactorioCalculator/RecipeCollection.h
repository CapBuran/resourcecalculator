#ifndef RecipeCollectionH
#define RecipeCollectionH

#include "Recipe.h"

namespace FactorioCalculator{

  class RecipeCollection;

  struct RecipeResultTree
  {
  private:
    KEY_ITEM   _ItemKey;
    std::map<KEY_RECIPE, RecipeResultTree> _Result;
  public:
    const std::map<KEY_RECIPE, RecipeResultTree> & GetResult() const;
    friend RecipeCollection;
    friend std::pair<const KEY_RECIPE, RecipeResultTree>;
  };

  class RecipeCollection: public Jsonable
  {
  public:

    RecipeResultTree BuildTreeRecipe(KEY_ITEM Item, int NestingResults) const;

    void ADD(const Recipe &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_RECIPE, Recipe> _Recipes;
 
  };

}

#endif // !RecipeCollectionH
