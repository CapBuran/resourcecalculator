#pragma once

#include <Recipe.h>
#include <FactoryCollection.h>

namespace ResourceCalculator
{
  class RecipeCollection: public Indexator<KEY_RECIPE, Recipe>
  {
  public:
    RecipeCollection() = default;
    RecipeCollection(const RecipeCollection& copy);
    bool DeleteItems(const std::set<KEY_ITEM>& keys);
  };
}
