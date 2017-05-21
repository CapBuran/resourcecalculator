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
    _Items.push_back(Item);
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
      it.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

}
