#include "Recipe.h"

namespace ResourceCalculator {

  Recipe::Recipe()
    : _Time(1.0)
    , _TypeFactory(KEY_TYPE_FACTORY::Unknown)
    , _MainResult(KEY_ITEM::ID_ITEM_NoFind_Item)
  {
  }

  bool Recipe::DeleteItem(KEY_ITEM KeyItem)
  {
    _Required.erase(KeyItem);
    _Result.erase(KeyItem);
    return true;
  }

  bool Recipe::DeleteItems(const std::set<KEY_ITEM>& ItemsKeys)
  {
    for (const KEY_ITEM it: ItemsKeys)
      DeleteItem(it);
    return true;
  }

  int Recipe::ReadFromJson(const Json::Value & jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    _Required.clear();
    for (auto &it : jsonPr["Required"]) {
      CountsItem ToAdd;
      ToAdd.ItemId = static_cast<KEY_ITEM>(it["ItemId"].asInt64());
      ToAdd.Count = it["Count"].asDouble();
      _Required.insert(ToAdd);
    }
    _Result.clear();
    for (auto &it : jsonPr["Result"]) {
      CountsItem ToAdd;
      ToAdd.ItemId = static_cast<KEY_ITEM>(it["ItemId"].asInt64());
      ToAdd.Count = it["Count"].asDouble();
      _Result.insert(ToAdd);
    }
    _TypeFactory = static_cast<KEY_TYPE_FACTORY>(jsonPr["TypeFactory"].asInt64());
    _Time = jsonPr["Time"].asDouble();
    _MainResult = static_cast<KEY_ITEM>(jsonPr["MainResult"].asInt64());
    return 0;
  }

  int Recipe::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    Json::Value jsonRequired = Json::Value(Json::arrayValue);
    for (auto &it : _Required) {
      Json::Value newVal;
      newVal["ItemId"] = static_cast<KEY_TO_Json>(it.ItemId);
      newVal["Count"] = it.Count;
      jsonRequired.append(newVal);
    }
    Json::Value jsonResult = Json::Value(Json::arrayValue);
    for (auto &it : _Result) {
      Json::Value newVal;
      newVal["ItemId"] = static_cast<KEY_TO_Json>(it.ItemId);
      newVal["Count"] = it.Count;
      jsonResult.append(newVal);
    }
    jsonPr["TypeFactory"] = static_cast<Json::Value::Int64>(_TypeFactory);
    jsonPr["Required"] = jsonRequired;
    jsonPr["Result"] = jsonResult;
    jsonPr["MainResult"] = static_cast<Json::Value::Int64>(_MainResult);
    jsonPr["Time"] = _Time;
    return 0;
  }

}