#include "Item.h"

namespace FactorioCalculator {

  Item::Item(const std::string &Name, KEY_ITEM Key):
    FactorioItem(Name), _Key(Key)
  {
  }

  Item::~Item()
  {
  }

  int Item::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Item::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["Name"]["ru"] = _Name;
    jsonPr["Name"]["en"] = "English lang";
    return 0;
  }

}