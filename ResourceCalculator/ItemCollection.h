#pragma once

#include "Item.h"

namespace ResourceCalculator {
  class RecipeCollection;
  class ItemCollection: public Indexator<KEY_ITEM, Item>
  {
  public:
    ItemCollection(RecipeCollection& RC);
    ItemCollection(const ItemCollection& copy);
    bool Delete(const std::set<KEY_ITEM>& keys) override;
    void CloneFrom(const MyIndexator& other) override;
    const RecipeCollection& GetRecipes() const { return _RC; }
    RecipeCollection& GetRecipes() { return _RC; }
  private:
    RecipeCollection& _RC;
  };
}
