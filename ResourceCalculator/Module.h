#ifndef ModuleH
#define ModuleH

#include "Types.h"

namespace ResourceCalculator {
  
  class Module: public ItemBase {
  private:
    KEY_MODULE _Key;

    double _CoefficientPollution;//Коэффициент загрязнения
    double _CoefficientEffectiveness;//Коэффициент эффективности
    double _CoefficientSpeed;//Коэффициент скорости
    double _CoefficientProductivity;//Коэффициент продуктивности

  public:
    DeclareAndDefinitionProperty(Key,                  KEY_MODULE)
    DeclareAndDefinitionProperty(CoefficientPollution,     double)
    DeclareAndDefinitionProperty(CoefficientEffectiveness, double)
    DeclareAndDefinitionProperty(CoefficientSpeed,         double)
    DeclareAndDefinitionProperty(CoefficientProductivity,  double)

    Module();
    ~Module();
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

  };
  
  class  ModuleCollection;

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
    void DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel );
    int ReadFromJson( const Json::Value & jsonPr ) override;
    int WriteToJson( Json::Value & jsonPr ) const override;
  };


}

#endif
