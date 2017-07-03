#ifndef FACTORY_H
#define FACTORY_H

#include <string>

#include "Types.h"


namespace ResourceCalculator {

  class FactoryModules;
  class ParamsCollection;

  class Factory : public ItemBase, public Jsonable {
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

    TYPE_FACTORY _Type;

    Factory();
    Factory(const Factory &recipe);

  public:

    Factory(std::string const &Name, double Speed, double Power = 1.0);
    ~Factory();

    KEY_FACTORY GetKey() const;

    double GetSpeed() const;
    double GetPeakPower() const;
    double GetLevelOfPollution() const;
    double GetWear() const;

    bool IsAllowedProduction(const ParamsCollection & PC, KEY_RECIPE RecipeId) const;

    void FixFactoryModules(FactoryModules &) const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    friend std::pair<const KEY_FACTORY, Factory>;

  };

}
#endif// FACTORY_H