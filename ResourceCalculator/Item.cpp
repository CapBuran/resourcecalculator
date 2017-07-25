#include "Item.h"

namespace ResourceCalculator {

  Item::~Item()
  {
  }

  int Item::ReadFromJson(const Json::Value & jsonPr)
  {
    _Key = static_cast<KEY_ITEM>( jsonPr["Key"].asInt64());
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  int Item::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    return 0;
  }

}