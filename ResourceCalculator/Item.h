#include "Types.h"

namespace ResourceCalculator {
  
  class Item: public FactorioItem, public Jsonable {
  private:
    KEY_ITEM _Key;
    Item();
    Item(const Item &item);
  public:
    Item(std::string const &Name, KEY_ITEM Key);
    //Item& operator=(const Item& right);
    ~Item();
    KEY_ITEM GetKey() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
    friend std::pair<const KEY_ITEM, Item>;
  };

}
