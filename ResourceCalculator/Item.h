#ifndef ItemH
#define ItemH

#include "Types.h"

namespace ResourceCalculator {
  
  class Item: public ItemBase, public Jsonable {
  private:
    KEY_ITEM _Key;
    bool _IsALiquidOrGas;
    Item();
    Item(const Item &item);
    //Item& operator=(const Item& right);
  public:
    Item(std::string const &Name, KEY_ITEM Key);
    ~Item();
    KEY_ITEM GetKey() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
    friend std::pair<const KEY_ITEM, Item>;
  };

}

#endif ItemH