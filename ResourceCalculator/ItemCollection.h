#pragma once

#include "Item.h"

namespace ResourceCalculator {

  class RecipeCollection;
  class ParamsCollection;

  class ItemCollection: public Jsonable, public Indexator<KEY_ITEM, Item>
  {
  public:
    ItemCollection(RecipeCollection& RC);
    ItemCollection(const ItemCollection& copy);
    ~ItemCollection();
    ItemCollection& operator= (const ItemCollection& ic);

    const RecipeCollection& GetRecipes() const;
    RecipeCollection& GetRecipes();

    void Delete( const std::set<KEY_ITEM>& ItemsKeysToDel);
    void Add( const std::map<KEY_ITEM, Item >& ItemsToAdd);

    const Item* GetItem(KEY_ITEM KeyItem) const;
    Item* GetItemForEdit(KEY_ITEM KeyItem);

    int ReadFromJson(const Json::Value& jsonPr) override;
    int WriteToJson(Json::Value& jsonPr) const override;

  private:
    std::map<KEY_ITEM, Item> _Items;
    RecipeCollection& _RC;
  };

}
