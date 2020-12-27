#include "RecipeCollection.h"

namespace ResourceCalculator
{
  RecipeCollection::RecipeCollection(const RecipeCollection& copy)
  {
    CloneFrom(copy);
  }

  bool RecipeCollection::DeleteItems(const std::set<KEY_ITEM>& ItemsKeysToDel)
  {
    IteratorForAllItem( [&](Recipe& recipe) { return (recipe.DeleteItems(ItemsKeysToDel)); });
    return true;
  }
}
