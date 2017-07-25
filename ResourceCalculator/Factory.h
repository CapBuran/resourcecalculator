#ifndef FACTORY_H
#define FACTORY_H

#include <string>

#include "Types.h"


namespace ResourceCalculator {

  class FactoryModules;
  class FactoryCollection;
  class ParamsCollection;

  struct FactoryParams{
    double Speed;
    int CountSlotsForModules;
    int CountSlotsForRecipes;
    //!Износ от 0.0 до 1.0
    double Wear;
    //!Пиковая мощность потребляемая из электросети
    double PeakPower;
    //!Пиковое значение загрязнения
    double LevelOfPollution;
    KEY_FACTORY Key;
    KEY_TYPE_FACTORY Type;
  };

  class Factory : public ItemBase {
  private:
    double _Speed;

    int _CountSlotsForModules;
    int _CountSlotsForRecipes;

    //!Износ от 0.0 до 1.0
    double _Wear;
    
    //!Пиковая мощность потребляемая из электросети
    double _PeakPower;
    
    //!Пиковое значение загрязнения
    double _LevelOfPollution;

    KEY_FACTORY _Key;

    KEY_TYPE_FACTORY _Type;

  public:

    Factory();

    DeclareAndDefinitionProperty(Speed,  double)
    DeclareAndDefinitionProperty(CountSlotsForModules, int)
    DeclareAndDefinitionProperty(CountSlotsForRecipes, int)
    DeclareAndDefinitionProperty(Wear,   double)
    DeclareAndDefinitionProperty(PeakPower, double)
    DeclareAndDefinitionProperty(LevelOfPollution, double)
    DeclareAndDefinitionProperty(Key,  KEY_FACTORY)
    DeclareAndDefinitionProperty(Type, KEY_TYPE_FACTORY)

    void SetParams(const FactoryParams &params);

    bool IsAllowedProduction(const ParamsCollection & PC, KEY_RECIPE RecipeId) const;

    void FixFactoryModules(FactoryModules &) const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;


  };

}
#endif// FACTORY_H