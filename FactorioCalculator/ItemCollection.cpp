#include <assert.h>
#include "ItemCollection.h"

namespace FactorioCalculator{

  ItemCollection::ItemCollection()
  {
  }

  ItemCollection::~ItemCollection()
  {
  }

  void ItemCollection::ADD(const Item &Item)
  {
    _Items[Item.GetKey()] = Item;
  }

  int ItemCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto it: jsonPr){
      KEY_TO_Json Key  = it["Key"].asInt64();
      std::string Name = it["Name"]["ru"].asString();
      Item ToAdd(Name, static_cast<KEY_ITEM>(Key));
      ADD(ToAdd);
    }
    return 0;
  }

  int ItemCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Items){
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

  const Item & ItemCollection::GetItem(KEY_ITEM key) const
  {
    std::string f;
    static Item d(f, key);
    return d;
    //return _Items.at(key);
    //return _Items[key];

  }

}
