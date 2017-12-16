#include "Module.h"
#include "ModuleCollection.h"

namespace ResourceCalculator {

  Module::Module(): 
    _Key(KEY_MODULE::ID_CleanSlot),
    _CoefficientPollution(0.0),
    _CoefficientSpeed(0.0),
    _CoefficientProductivity(0.0)
  {
  }

  Module::~Module()
  {
  }

  int Module::ReadFromJson(const Json::Value & jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    _Key = static_cast<KEY_MODULE>(jsonPr["Key"].asInt64());
    return 0;
  }

  int Module::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    return 0;
  }

  bool FactoryModules::SetCountModules(int Count)
  {
    bool retval = _Modules.size() > Count;
    if (retval) _Modules.resize(Count);
    return retval;
  }

  double FactoryModules::GetSummSpeed(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.GetCoefficientSpeed();
    }
    return retval;
  }

  double FactoryModules::GetSummProductivity(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.GetCoefficientProductivity();
    }
    return retval;
  }

  double FactoryModules::GetSummEffectiveness(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.GetCoefficientEffectiveness();
    }
    //TODO выставить такие ограничения в настройки
    if (retval > 1.8) retval = 1.8;
    return retval;
  }

  double FactoryModules::GetSummPollution(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.GetCoefficientPollution();
    }
    return retval;
  }

  const std::vector<KEY_MODULE> FactoryModules::GetModules() const
  {
    return _Modules;
  }

  bool FactoryModules::SetModule(int IndexSlot, KEY_MODULE key)
  {
    bool retvalue = _Modules.size() > IndexSlot;
    if (retvalue) _Modules[IndexSlot] = key;
    return retvalue;
  }

}