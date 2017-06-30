#include "Module.h"
#include "ModuleCollection.h"

namespace ResourceCalculator {

  Module::Module(): 
    ItemBase(""), _Key(KEY_MODULE::ID_CleanSlot),
    _CoefficientPollution(0.0),
    _CoefficientSpeed(0.0),
    _CoefficientProductivity(0.0)
  {
  }

  double Module::FactorProductivity() const
  {
    return 0.0;
  }

  double Module::FactorSpeed() const
  {
    return 0.0;
  }

  double Module::FactorEffectiveness() const
  {
    return 0.0;
  }

  double Module::FactorPollution() const
  {
    return _CoefficientPollution;
  }

  Module::Module(const std::string &Name, KEY_MODULE Key):
    ItemBase(Name), _Key(Key),
    _CoefficientPollution(0.0),
    _CoefficientSpeed(0.0),
    _CoefficientProductivity(0.0)
  {
  }

  //Module::Module(const Module & Module):
  //  ItemBase(Module.GetName()), _Key(Module._Key)
  //{
  //}

  //Module & Module::operator=(const Module & right)
  //{
  //  if (this == &right) {
  //    return *this;
  //  }
  //  _Key = right._Key;
  //  _Name = right._Name;
  //  return *this;
  //}

  Module::~Module()
  {
  }

  KEY_MODULE Module::GetKey() const
  {
    return _Key;
  }

  int Module::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Module::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["Name"]["ru"] = _Name;
    jsonPr["Name"]["en"] = "English lang";
    return 0;
  }




  FactoryModules::FactoryModules(KEY_FACTORY factoryID) :
    FactoryID(factoryID)
  {
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
      retval += module.FactorSpeed();
    }
    return retval;
  }

  double FactoryModules::GetSummProductivity(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.FactorProductivity();
    }
    return retval;
  }

  double FactoryModules::GetSummEffectiveness(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC.GetModule(moduleKey);
      retval += module.FactorEffectiveness();
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
      retval += module.FactorPollution();
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