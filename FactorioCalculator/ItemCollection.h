#ifndef ItemCollectionH
#define ItemCollectionH

#include "Item.h"

namespace FactorioCalculator{

  class ItemCollection: public Jsonable
  {
  public:
    ItemCollection();
    ~ItemCollection();

    void ADD(const Item &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::list<Item> _Items;

  };

}

#endif // !ItemCollectionH
