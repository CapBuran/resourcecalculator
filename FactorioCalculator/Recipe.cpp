#include "Recipe.h"

namespace FactorioCalculator{

  static RecipeParams RecipeParamsDefault;

  Recipe::Recipe():
    FactorioItem(""),_Key(RecipeParamsDefault.Key), _Time(RecipeParamsDefault.Time),
    _Required(RecipeParamsDefault.Required), _Result(RecipeParamsDefault.Result)
  {

  }

  Recipe::Recipe(std::string const & Name, const RecipeParams & Params):
    FactorioItem(Name), _Key(Params.Key), _Time(Params.Time),
    _Required(Params.Required), _Result(Params.Result), _CurrentFactory(Params.CurrentFactory)
  {
  }

  Recipe::Recipe(const Recipe & item):
    FactorioItem(item.GetName())
  {
  }

  RecipeParams Recipe::GetRecipeParams() const
  {
    RecipeParams Retval;
    Retval.Key            = _Key;
    Retval.Time           = _Time;
    Retval.Required       = _Required;
    Retval.Result         = _Result;
    Retval.FactoryAllowed = _FactoryAllowed;
    Retval.CurrentFactory = _CurrentFactory;
    return Retval;
  }

  KEY_RECIPE Recipe::GetKey() const
  {
    return _Key;
  }

  int Recipe::ReadFromJson(const Json::Value & jsonPr)
  {
    _Name = jsonPr["Name"]["ru"].asString();
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
    Key = static_cast<KEY_RECIPE>(jsonPr["Key"].asInt64());
    CurrentFactory = static_cast<KEY_FACTORY>(jsonPr["CurrentFactory"].asInt64());
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
    jsonPr["Key"] = static_cast<Json::Value::Int64>(Key);
    jsonPr["Time"] = Time;
    jsonPr["CurrentFactory"] = static_cast<Json::Value::Int64>(CurrentFactory);
    return 0;
  }

}