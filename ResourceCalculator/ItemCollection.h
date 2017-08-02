#ifndef ItemCollectionH
#define ItemCollectionH

#include "Item.h"

namespace ResourceCalculator {

  class ParamsCollection;

  class ItemCollection: public Jsonable
  {
  public:
    ItemCollection();
    ~ItemCollection();

    void ADD(const Item);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const std::map<KEY_ITEM, Item> &GetData() const;

    Item *GetItemForEdit(KEY_ITEM KeyRecipe);
    const Item *GetItem(KEY_ITEM key) const;

    KEY_ITEM GetUniqueRecipeKey() const;

  private:
    bool DeleteItem(KEY_ITEM KeyItem);

    std::map<KEY_ITEM, Item> _Items;
    
    friend ParamsCollection;

  };

}

#endif // !ItemCollectionH
