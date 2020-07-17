#ifndef ItemCollectionH
#define ItemCollectionH

#include "Item.h"

namespace ResourceCalculator {

  class RecipeCollection;
  class ParamsCollection;

  class ItemCollection: public Jsonable
  {
  public:
    ItemCollection( RecipeCollection& RC );
    ~ItemCollection();
    void Delete( const std::set<KEY_ITEM>& ItemsKeysToDel );
    void Add( const std::map<KEY_ITEM, Item >& ItemsToAdd);
    int ReadFromJson(const Json::Value& jsonPr) override;
    int WriteToJson(Json::Value& jsonPr) const override;
    const std::map<KEY_ITEM, Item>& GetData() const;
    KEY_ITEM GetUniqueItemKey();
    const Item *GetItem(KEY_ITEM KeyItem) const;
    const Item *GetItemByID(int KeyItem) const;
    int GetItemIdByKey(KEY_ITEM KeyItem) const;
  private:
    TYPE_KEY _LastGenGen;
    std::map<KEY_ITEM, Item> _Items;
    RecipeCollection& _RC;
  };

}

#endif // !ItemCollectionH
