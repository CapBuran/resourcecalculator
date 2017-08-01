#include "Item.h"

namespace ResourceCalculator {

  Item::~Item()
  {
  }

  int Item::ReadFromJson(const Json::Value & jsonPr)
  {
    _Key = static_cast<KEY_ITEM>(jsonPr["Key"].asInt64());
    _IsALiquidOrGas = jsonPr["IsALiquidOrGas"].asBool();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  int Item::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["IsALiquidOrGas"] = _IsALiquidOrGas;
    return 0;
  }

}