#ifndef ItemCollectionH
#define ItemCollectionH

#include "Item.h"

namespace ResourceCalculator {

  class ItemCollection: public Jsonable
  {
  public:
    ItemCollection();
    ~ItemCollection();

    void ADD(const Item &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_ITEM, Item> _Items;
    
    const Item &GetItem(KEY_ITEM key) const;

  };

}

#endif // !ItemCollectionH
