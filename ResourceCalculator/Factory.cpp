#include "Module.h"
#include "Factory.h"
#include "ParamsCollection.h"

namespace ResourceCalculator {

  Factory::Factory(const std::string &Name, double Speed, double Power):
    ItemBase(Name)
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

  Factory::Factory():
    ItemBase("")
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

  Factory::Factory(const Factory &factory):
    ItemBase(factory.GetName()), _Wear(0.0)
  {
    _Speed = factory._Speed;
    _PeakPower = factory._PeakPower;
    _LevelOfPollution = factory._LevelOfPollution;
    _CountSlotsForModules = factory._CountSlotsForModules;
    _CountSlotsForRecipes = factory._CountSlotsForRecipes;
    _Wear = factory._Wear;
    _Key = factory._Key;
    _Type = factory._Type;
  }

  Factory::~Factory()
  {
  }

  KEY_FACTORY Factory::GetKey() const
  {
    return _Key;
  }

  double Factory::GetSpeed() const
  {
    return _Speed;
  }

  double Factory::GetPeakPower() const
  {
    return _PeakPower;
  }

  double Factory::GetLevelOfPollution() const
  {
    return _LevelOfPollution;
  }

  double Factory::GetWear() const
  {
    return _Wear;
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
    const RecipeParams RP = recipe.GetRecipeParams();
    
    if (RP.TypeFactory != _Type) return false;

    if (_CountSlotsForRecipes < RP.Required.size()) return false;
    if (_CountSlotsForRecipes < RP.Result.size()) return false;

    return true;
  }

  void Factory::FixFactoryModules(FactoryModules & FM) const
  {
    FM.SetCountModules(_CountSlotsForModules);
  }

  int Factory::ReadFromJson(const Json::Value & jsonPr)
  {
    _Speed = jsonPr["Speed"].asDouble();
    _CountSlotsForModules = jsonPr["CountSlotsForModules"].asInt64();
    _CountSlotsForRecipes = jsonPr["CountSlotsForRecipes"].asInt64();
    _Wear = jsonPr["Wear"].asDouble();
    _PeakPower = jsonPr["PeakPower"].asDouble();
    _LevelOfPollution = jsonPr["LevelOfPollution"].asDouble();
    _Key = static_cast<KEY_FACTORY>(jsonPr["Key"].asInt64());
    _Type = static_cast<KEY_TYPE_FACTORY>(jsonPr["Type"].asInt64());
    return 0;
  }

  int Factory::WriteToJson(Json::Value & jsonPr) const
  {
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