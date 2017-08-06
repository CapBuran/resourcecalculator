#include "Module.h"
#include "Factory.h"
#include "ParamsCollection.h"

namespace ResourceCalculator
{

  Factory::Factory()
  {
    _Speed = 1.0;
    _CountSlotsForModules = 0;
    _CountSlotsForRecipes = 0;
    _Wear = 0.0;
    _Power = 0.0;
    _ElectricPeakPower = 0.0;
    _LevelOfPollution = 0.0;
    _Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _Type = KEY_TYPE_FACTORY::Unknown;
  }

  bool Factory::IsAllowedProduction( const ParamsCollection & PC, KEY_RECIPE RecipeId ) const
  {
    if ( _Type == KEY_TYPE_FACTORY::Unknown ) return false;
    const Recipe &recipe = PC.RC.GetData().find( RecipeId )->second;
    if ( recipe.GetTypeFactory() != _Type ) return false;
    if ( _CountSlotsForRecipes < recipe.GetRequired().size() ) return false;
    if ( _CountSlotsForRecipes < recipe.GetRequired().size() ) return false;
    return true;
  }

  void Factory::FixFactoryModules( FactoryModules & FM ) const
  {
    FM.SetCountModules( _CountSlotsForModules );
  }

  int Factory::ReadFromJson( const Json::Value & jsonPr )
  {
    ItemBase::ReadFromJson( jsonPr );
    _Speed = jsonPr["Speed"].asDouble();
    _CountSlotsForModules = jsonPr["CountSlotsForModules"].asInt();
    _Wear = jsonPr["Wear"].asDouble();
    _Power = jsonPr["Power"].asDouble();
    _ElectricPeakPower = jsonPr["ElectricPeakPower"].asDouble();
    _LevelOfPollution = jsonPr["LevelOfPollution"].asDouble();
    _Key = static_cast<KEY_FACTORY>( jsonPr["Key"].asInt64() );
    _Type = static_cast<KEY_TYPE_FACTORY>( jsonPr["Type"].asInt64() );
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
    jsonPr["Type"] = static_cast<KEY_TO_Json>( _Type );
    return 0;
  }

}