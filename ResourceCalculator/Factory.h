#ifndef FACTORY_H
#define FACTORY_H

#include <string>

#include "Types.h"

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

    //!����� �� 0.0 �� 1.0
    double _Wear;

    //! �������� ������� ���������
    double _Power;

    //!������� �������� ������������ �� �����������
    double _ElectricPeakPower;

    //!������� �������� �����������
    double _LevelOfPollution;

    KEY_FACTORY _Key;

    KEY_TYPE_FACTORY _Type;

  public:

    Factory();

    DeclareAndDefinitionProperty( Speed, double )
    DeclareAndDefinitionProperty( CountSlotsForModules, int )
    DeclareAndDefinitionProperty( CountSlotsForRecipes, int )
    DeclareAndDefinitionProperty( Wear, double )
    DeclareAndDefinitionProperty( Power, double )
    DeclareAndDefinitionProperty( LevelOfPollution, double )
    DeclareAndDefinitionProperty( Key, KEY_FACTORY )
    DeclareAndDefinitionProperty( Type, KEY_TYPE_FACTORY )

    bool IsAllowedProduction( const ParamsCollection & PC, KEY_RECIPE RecipeId ) const;

    void FixFactoryModules( FactoryModules & ) const;

    int ReadFromJson( const Json::Value & jsonPr ) override;
    int WriteToJson( Json::Value & jsonPr ) const override;

  };

}
#endif// FACTORY_H