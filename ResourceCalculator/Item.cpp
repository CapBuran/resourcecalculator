#include "Item.h"

namespace ResourceCalculator {

  int Item::ReadFromJson(const Json::Value & jsonPr)
  {
    _Key = static_cast<KEY_ITEM>(jsonPr["Key"].asInt64());
    _IsALiquidOrGas = jsonPr["IsALiquidOrGas"].asBool();
    _MiningHardness = jsonPr["MiningHardness"].asDouble();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  Item::Item():
    _Key(KEY_ITEM::ID_ITEM_NoFind_Item),
    _IsALiquidOrGas(false),
    _MiningHardness(0.0)
  {
  }

  int Item::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["IsALiquidOrGas"] = _IsALiquidOrGas;
    jsonPr["MiningHardness"] = _MiningHardness;
    return 0;
  }

}