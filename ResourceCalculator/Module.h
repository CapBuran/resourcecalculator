#ifndef ModuleH
#define ModuleH

#include "Types.h"

namespace ResourceCalculator {
  
  class Module: public ItemBase, public Jsonable {
  private:
    KEY_MODULE _Key;
    Module();
    Module(const Module &Module) = delete;
    Module& operator=(const Module& right) = delete;

    double _CoefficientPollution;//Коэффициент загрязнения
    double _CoefficientEffectiveness;//Коэффициент эффективности
    double _CoefficientSpeed;//Коэффициент скорости
    double _CoefficientProductivity;//Коэффициент продуктивности

  public:
    double FactorProductivity() const;
    double FactorSpeed() const;
    double FactorEffectiveness() const;
    double FactorPollution() const;
    Module(std::string const &Name, KEY_MODULE Key);
    ~Module();
    KEY_MODULE GetKey() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
    friend std::pair<const KEY_MODULE, Module>;
  };
  
  class  ModuleCollection;

  class FactoryModules {
  private:
    std::vector < KEY_MODULE > _Modules;
    FactoryModules() = delete;
    FactoryModules(KEY_FACTORY FactoryID);
    FactoryModules(const FactoryModules &) = delete;
  public:
    const KEY_FACTORY FactoryID;
    const std::vector < KEY_MODULE > GetModules() const;
    bool SetModule(int IndexSlot, KEY_MODULE key);
    bool SetCountModules(int Count);
    double GetSummSpeed(const ModuleCollection &MC) const;
    double GetSummProductivity(const ModuleCollection &MC) const;
    double GetSummEffectiveness(const ModuleCollection &MC) const;
    double GetSummPollution(const ModuleCollection &MC) const;
  };


}

#endif
