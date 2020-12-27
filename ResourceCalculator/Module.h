#pragma once

#include <Types.h>

namespace ResourceCalculator
{
  
  class Module: public ItemBase<KEY_MODULE>
  {
  private:
    double _CoefficientPollution;//Коэффициент загрязнения
    double _CoefficientSpeed;//Коэффициент скорости
    double _CoefficientProductivity;//Коэффициент продуктивности
    double _CoefficientConsumption;//Коэффициент потребления

  public:
    DeclareAndDefinitionProperty(CoefficientPollution,     double)
    DeclareAndDefinitionProperty(CoefficientSpeed,         double)
    DeclareAndDefinitionProperty(CoefficientProductivity,  double)
    DeclareAndDefinitionProperty(CoefficientConsumption,   double)

    Module();
    ~Module();
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

  };

  using ModuleCollection = Indexator<KEY_MODULE, Module>;
  
  class FactoryModules: public Jsonable {
  private:
    std::vector < KEY_MODULE > _Modules;
  public:
    const std::vector < KEY_MODULE > GetModules() const;
    bool SetModule(int IndexSlot, KEY_MODULE key);
    bool SetCountModules(int Count);
    double GetSummSpeed(const ModuleCollection &MC) const;
    double GetSummProductivity(const ModuleCollection &MC) const;
    double GetSummEffectiveness(const ModuleCollection &MC) const;
    double GetSummPollution(const ModuleCollection &MC) const;
    void DeleteModules( const std::set<KEY_MODULE>& ModulesToDel );
    int ReadFromJson( const Json::Value & jsonPr ) override;
    int WriteToJson( Json::Value & jsonPr ) const override;
  };

}
