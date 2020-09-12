#include "Module.h"
#include "Factory.h"

namespace ResourceCalculator
{

  Factory::Factory()
  {
    _Speed = 1.0;
    _CountSlotsForModules = 0;
    _CountSlotsForRecipes = 1;
    _Wear = 0.0;
    _Power = 0.0;
    _ElectricPeakPower = 0.0;
    _LevelOfPollution = 0.0;
    _Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
  }

  bool Factory::IsAllowedProduction(const Recipe& recipe) const
  {
    if (_Types.size() == 0) return false;
    if ( _Types.count(recipe.GetTypeFactory()) == 0 ) return false;
    if ( _CountSlotsForRecipes < recipe.GetResult().size() ) return false;
    if ( _CountSlotsForRecipes < recipe.GetRequired().size() ) return false;
    return true;
  }

  bool Factory::IsAllowedMining(const Item& item) const
  {
    if (_Types.size() == 0) return false;
    if (item.GetMiningHardness() == 0 || _Power == 0) return true;
    if (item.GetMiningHardness() >= _Power) return false;
    return true;
  }

  void Factory::FixFactoryModules( FactoryModules & FM ) const
  {
    FM.SetCountModules( _CountSlotsForModules );
  }

  int Factory::ReadFromJson( const Json::Value & jsonPr )
  {
    _Types.clear();
    ItemBase::ReadFromJson( jsonPr );
    _Speed = jsonPr["Speed"].asDouble();
    _CountSlotsForModules = jsonPr["CountSlotsForModules"].asInt();
    _CountSlotsForRecipes = jsonPr["CountSlotsForRecipes"].asInt();
    _Wear = jsonPr["Wear"].asDouble();
    _Power = jsonPr["Power"].asDouble();
    _ElectricPeakPower = jsonPr["ElectricPeakPower"].asDouble();
    _LevelOfPollution = jsonPr["LevelOfPollution"].asDouble();
    _Key = static_cast<KEY_FACTORY>( jsonPr["Key"].asInt64() );
    Json::Value jsonPrArray = jsonPr["Types"];
    for (auto& it : jsonPrArray) {
      auto ToAdd = static_cast<KEY_TYPE_FACTORY>(it.asInt64());
      _Types.insert(ToAdd);
    }
    return 0;
  }

  int Factory::WriteToJson( Json::Value & jsonPr ) const
  {
    ItemBase::WriteToJson( jsonPr );
    jsonPr["Speed"] = _Speed;
    jsonPr["CountSlotsForModules"] = _CountSlotsForModules;
    jsonPr["CountSlotsForRecipes"] = _CountSlotsForRecipes;
    jsonPr["Wear"] = _Wear;
    jsonPr["Power"] = _Power;
    jsonPr["ElectricPeakPower"] = _ElectricPeakPower;
    jsonPr["LevelOfPollution"] = _LevelOfPollution;
    jsonPr["Key"] = static_cast<KEY_TO_Json>( _Key );
    Json::Value jsonPrArray = Json::Value(Json::arrayValue);
    for (auto& it : _Types) {
      Json::Value newVal;
      newVal = static_cast<KEY_TO_Json>(it);
      jsonPrArray.append(newVal);
    }
    jsonPr["Types"] = jsonPrArray;
    return 0;
  }

}