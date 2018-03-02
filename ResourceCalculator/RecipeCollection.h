#ifndef RecipeCollectionH
#define RecipeCollectionH

#include <set>
#include <map>
#include <algorithm>

#include "Recipe.h"

namespace ResourceCalculator {

  class RecipeCollection: public Jsonable
  {
  public:

    RecipeCollection();

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const std::map<KEY_RECIPE, Recipe> &GetData() const;
    std::map<KEY_RECIPE, Recipe> &GetDataForEdit();

    Recipe *GetRecipeForEdit(KEY_RECIPE KeyRecipe);
    const Recipe *GetRecipe(KEY_RECIPE KeyRecipe) const;

    void Delete( const std::set<KEY_ITEM>& ItemsKeysToDel );
    void Delete( const std::set<KEY_RECIPE>& RecipsKeysToDel );

    KEY_RECIPE GetUniqueRecipeKey();

    void Add(const Recipe &);

  private:
    TYPE_KEY _LastGenGey;
    std::map<KEY_RECIPE, Recipe> _Recipes;

  };

}

#endif // !RecipeCollectionH
