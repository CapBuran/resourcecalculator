#include "Item.h"

namespace FactorioCalculator {
  Item::Item():
    FactorioItem("")
  {
  }

  Item::Item(const std::string &Name, KEY_ITEM Key):
    FactorioItem(Name), _Key(Key)
  {
  }

  Item::Item(const Item & item):
    FactorioItem(item.GetName()), _Key(item._Key)
  {
  }

  //Item & Item::operator=(const Item & right)
  //{
  //  if (this == &right) {
  //    return *this;
  //  }
  //  _Key = right._Key;
  //  _Name = right._Name;
  //  return *this;
  //}

  Item::~Item()
  {
  }

  KEY_ITEM Item::GetKey() const
  {
    return _Key;
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