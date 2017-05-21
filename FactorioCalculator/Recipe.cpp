#include "Recipe.h"

namespace FactorioCalculator{

  Recipe::Recipe(std::string const & Name, const RecipeParams & Params):
    FactorioItem(Name), _Key(Params.Key), _Time(Params.Time),
    _Required(Params.Required), _Result(Params.Result)
  {
  }

  RecipeParams Recipe::GetRecipeParams() const
  {
    RecipeParams Retval;
    Retval.Key = _Key;
    Retval.Time = _Time;
    Retval.Required = _Required;
    Retval.Result = _Result;
    Retval.FactoryAllowed = _FactoryAllowed;
    return Retval;
  }

  int Recipe::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Recipe::WriteToJson(Json::Value & jsonPr) const
  {
    RecipeParams Retval = GetRecipeParams();
    Retval.WriteToJson(jsonPr);
    jsonPr["Name"]["ru"] = _Name;
    jsonPr["Name"]["en"] = "English lang";
    return 0;
  }

  Recipe::~Recipe()
  {
  }

  int RecipeParams::ReadFromJson(const Json::Value & jsonPr)
  {
    Required.clear();
    for (auto &it : jsonPr["Required"]) {
      CountsItem ToAdd;
      ToAdd.ItemId = static_cast<KEY_ITEM>(it["ItemId"].asInt64());
      ToAdd.Count = it["Count"].asDouble();
      Required.push_back(ToAdd);
    }
    Result.clear();
    for (auto &it : jsonPr["Result"]) {
      CountsItem ToAdd;
      ToAdd.ItemId = static_cast<KEY_ITEM>(it["ItemId"].asInt64());
      ToAdd.Count = it["Count"].asDouble();
      Result.push_back(ToAdd);
    }
    FactoryAllowed.clear();
    for (auto &it : jsonPr["FactoryAllowed"]) {
      KEY_FACTORY KF = static_cast<KEY_FACTORY>(it["KEY_FACTORY"].asInt64());
      FactoryAllowed.push_back(KF);
    }
    Key = jsonPr["Key"].asInt64();
    Time = jsonPr["Time"].asDouble();
    return 0;
  }

  int RecipeParams::WriteToJson(Json::Value & jsonPr) const
  {
    Json::Value jsonRequired = Json::Value(Json::arrayValue);
    for (auto &it : Required) {
      Json::Value newVal;
      newVal["ItemId"] = static_cast<KEY_TO_Json>(it.ItemId);
      newVal["Count"] = it.Count;
      jsonRequired.append(newVal);
    }
    Json::Value jsonResult = Json::Value(Json::arrayValue);
    for (auto &it : Result) {
      Json::Value newVal;
      newVal["ItemId"] = static_cast<KEY_TO_Json>(it.ItemId);
      newVal["Count"] = it.Count;
      jsonResult.append(newVal);
    }
    Json::Value jsonFactoryAllowed = Json::Value(Json::arrayValue);
    for (auto &it : FactoryAllowed) {
      Json::Value newVal;
      newVal["FactoryId"] = static_cast<KEY_TO_Json>(it);
      jsonFactoryAllowed.append(newVal);
    }
    jsonPr["Required"] = jsonRequired;
    jsonPr["Result"] = jsonResult;
    jsonPr["FactoryAllowed"] = jsonFactoryAllowed;
    jsonPr["Key"] = Key;
    jsonPr["Time"] = Time;
    return 0;
  }

}