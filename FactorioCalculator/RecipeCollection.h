#ifndef RecipeCollectionH
#define RecipeCollectionH

#include "Recipe.h"

namespace FactorioCalculator{

  class RecipeCollection: public Jsonable
  {
  public:
    RecipeCollection();
    ~RecipeCollection();
    int BuildTableRecipe(KEY_ITEM Item, std::list<std::list<CountsItem> > &Result);

    void ADD(const Recipe &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_RECIPE, Recipe> _Recipes;

  };

}

#endif // !RecipeCollectionH
