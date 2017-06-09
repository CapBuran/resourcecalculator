#ifndef RecipeCollectionH
#define RecipeCollectionH

#include <set>

#include "Recipe.h"

namespace FactorioCalculator{

  class RecipeCollection;
  struct RecipeResultTree;

  struct ItemResultTree
  {
  private:
    std::map<KEY_RECIPE, RecipeResultTree> _Result;
  public:
    const std::map<KEY_RECIPE, RecipeResultTree> & GetResult() const;
    friend RecipeCollection;
    friend std::pair<const KEY_RECIPE, RecipeResultTree>;
  };

  struct RecipeResultTree
  {
  private:
    std::map<KEY_ITEM, ItemResultTree> _Result;
  public:
    const std::map<KEY_ITEM, ItemResultTree> & GetResult() const;
    friend RecipeCollection;
    friend std::pair<const KEY_ITEM, ItemResultTree>;
  };

  class RecipeCollection: public Jsonable
  {
  public:

    ItemResultTree   BuildTree(KEY_ITEM ItemID,     int NestingResults) const;
    RecipeResultTree BuildTree(KEY_RECIPE RecipeID, int NestingResults) const;

    void ADD(const Recipe &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_RECIPE, Recipe> _Recipes;
 
  };

}

#endif // !RecipeCollectionH
