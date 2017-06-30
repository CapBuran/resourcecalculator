#ifndef FACTORY_H
#define FACTORY_H

#include <string>

#include "Types.h"


namespace ResourceCalculator {

  class FactoryModules;

  class Factory : public ItemBase, public Jsonable {
  private:
    double _Speed;
    double _Power;

    int _CountSlotsForModules;
    int _CountSlotsForRecipes;

    //!����� �� 0.0 �� 1.0
    double _Wear;
    
    //!������� �������� ������������ �� �����������
    double _PeakPower;
    
    //!������� �������� �����������
    double _LevelOfPollution;

    KEY_FACTORY _Key;

    TYPE_FACTORY _Type;

    Factory();
    Factory(const Factory &recipe);

  public:

    Factory(std::string const &Name, double Speed, double Power = 1.0);
    ~Factory();

    KEY_FACTORY GetKey() const;

    void FixFactoryModules(FactoryModules &) const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    friend std::pair<const KEY_FACTORY, Factory>;

  };

}
#endif// FACTORY_H