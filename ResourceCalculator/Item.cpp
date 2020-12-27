#include "Item.h"

namespace ResourceCalculator {

  int Item::ReadFromJson(const Json::Value & jsonPr)
  {
    _IsALiquidOrGas = jsonPr["IsALiquidOrGas"].asBool();
    _MiningHardness = jsonPr["MiningHardness"].asDouble();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  Item::Item():
    _IsALiquidOrGas(false),
    _MiningHardness(0.0),
    _Count(0.0)
  {
  }

  int Item::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["IsALiquidOrGas"] = _IsALiquidOrGas;
    jsonPr["MiningHardness"] = _MiningHardness;
    return 0;
  }

}