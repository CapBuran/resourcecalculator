#include "Module.h"
#include "Factory.h"
#include "ParamsCollection.h"

namespace ResourceCalculator {

  Factory::Factory()
  {
    _Speed = 1.0;
    _CountSlotsForModules = 0;
    _CountSlotsForRecipes = 0;
    _Wear = 0.0;
    _PeakPower = 0.0;
    _LevelOfPollution = 0.0;
    _Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _Type = KEY_TYPE_FACTORY::Unknown;
  }

  void Factory::SetParams(const FactoryParams & params)
  {
    _Speed = params.Speed;
    _CountSlotsForModules = params.CountSlotsForModules;
    _CountSlotsForRecipes = params.CountSlotsForRecipes;
    _Wear = params.Wear;
    _PeakPower = params.PeakPower;
    _LevelOfPollution = params.LevelOfPollution;
    _Key = params.Key;
    _Type = params.Type;
  }

  bool Factory::IsAllowedProduction(const ParamsCollection & PC, KEY_RECIPE RecipeId) const
  {
    if(_Type == KEY_TYPE_FACTORY::Unknown) return false;
    const Recipe &recipe = PC.RC.GetData().find(RecipeId)->second;
    if (recipe.GetTypeFactory() != _Type) return false;
    if (_CountSlotsForRecipes < recipe.GetRequired().size()) return false;
    if (_CountSlotsForRecipes < recipe.GetRequired().size()) return false;
    return true;
  }

  void Factory::FixFactoryModules(FactoryModules & FM) const
  {
    FM.SetCountModules(_CountSlotsForModules);
  }

  int Factory::ReadFromJson(const Json::Value & jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    _Speed = jsonPr["Speed"].asDouble();
    _CountSlotsForModules = jsonPr["CountSlotsForModules"].asInt();
    _Wear = jsonPr["Wear"].asDouble();
    _PeakPower = jsonPr["PeakPower"].asDouble();
    _LevelOfPollution = jsonPr["LevelOfPollution"].asDouble();
    _Key = static_cast<KEY_FACTORY>(jsonPr["Key"].asInt64());
    _Type = static_cast<KEY_TYPE_FACTORY>(jsonPr["Type"].asInt64());
    return 0;
  }

  int Factory::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Speed"] = _Speed;
    jsonPr["CountSlotsForModules"] = _CountSlotsForModules;
    jsonPr["CountSlotsForRecipes"] = _CountSlotsForRecipes;
    jsonPr["Wear"] = _Wear;
    jsonPr["PeakPower"] = _PeakPower;
    jsonPr["LevelOfPollution"] = _LevelOfPollution;
    jsonPr["Key"] = static_cast<Json::Value::Int64>(_Key);
    jsonPr["Type"] = static_cast<Json::Value::Int64>(_Type);
    return 0;
  }

}