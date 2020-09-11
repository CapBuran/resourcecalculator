#pragma once

#include <set>
#include <map>
#include <algorithm>

#include "Recipe.h"
#include "FactoryCollection.h"

namespace ResourceCalculator
{
  class RecipeCollection: public Jsonable, public Indexator<KEY_RECIPE, Recipe>
  {
  public:
    RecipeCollection(const FactoryTypeCollection& FTC);
    RecipeCollection(const RecipeCollection& copy);
    RecipeCollection& operator= (const RecipeCollection& rc);
    virtual ~RecipeCollection();

    void Add(const Recipe&);
    void Delete(const std::set<KEY_ITEM>& ItemsKeysToDel);
    void Delete(const std::set<KEY_RECIPE>& RecipsKeysToDel);

    Recipe *GetRecipeForEdit(KEY_RECIPE KeyRecipe);
    const Recipe *GetRecipe(KEY_RECIPE KeyRecipe) const;

    int ReadFromJson(const Json::Value& jsonPr) override;
    int WriteToJson(Json::Value& jsonPr) const override;

    const FactoryTypeCollection& GetFactoryTypes() const;

  private:
    std::map<KEY_RECIPE, Recipe> _Recipes;
    const FactoryTypeCollection& _FTC;
  };

}
