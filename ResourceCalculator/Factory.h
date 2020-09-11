#pragma once

#include <string>
#include <set>

#include <Item.h>
#include <Recipe.h>

namespace ResourceCalculator
{
  class FactoryModules;
  class FactoryCollection;
  class ParamsCollection;

  class Factory: public ItemBase {
  private:
    double _Speed;

    int _CountSlotsForModules;
    int _CountSlotsForRecipes;

    //!Износ от 0.0 до 1.0
    double _Wear;

    //! Мощность буровых установок
    double _Power;

    //!Пиковая мощность потребляемая из электросети
    double _ElectricPeakPower;

    //!Пиковое значение загрязнения
    double _LevelOfPollution;

    KEY_FACTORY _Key;

    std::set<KEY_TYPE_FACTORY> _Types;

  public:

    Factory();

    bool IsAllowedProduction(const Recipe& recipe) const;

    bool IsAllowedMining(const Item& item) const;

    DeclareAndDefinitionProperty( Speed, double )
    DeclareAndDefinitionProperty( CountSlotsForModules, int )
    DeclareAndDefinitionProperty( CountSlotsForRecipes, int )
    DeclareAndDefinitionProperty( Wear, double )
    DeclareAndDefinitionProperty( Power, double )
    DeclareAndDefinitionProperty( LevelOfPollution, double )
    DeclareAndDefinitionProperty( Key, KEY_FACTORY )
    DeclareAndDefinitionProperty( Types, std::set<KEY_TYPE_FACTORY>)

    void FixFactoryModules( FactoryModules & ) const;

    int ReadFromJson( const Json::Value & jsonPr ) override;
    int WriteToJson( Json::Value & jsonPr ) const override;

  };

}
