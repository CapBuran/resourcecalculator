#include "Module.h"
#include "ModuleCollection.h"

namespace ResourceCalculator {

  Module::Module()
    : _CoefficientPollution(0.0)
    , _CoefficientSpeed(0.0)
    , _CoefficientProductivity(0.0)
    , _CoefficientConsumption(0.0)
  {
  }

  Module::~Module()
  {
  }

  int Module::ReadFromJson(const Json::Value & jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    _Key                      = static_cast<KEY_MODULE>(jsonPr["Key"].asInt64());
    _CoefficientPollution     = jsonPr["CoefficientPollution"].asDouble() ;
    _CoefficientConsumption   = jsonPr["CoefficientConsumption"].asDouble();
    _CoefficientSpeed         = jsonPr["CoefficientSpeed"].asDouble() ;
    _CoefficientProductivity  = jsonPr["CoefficientProductivity"].asDouble() ;
    return 0;
  }

  int Module::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["Key"]                      = static_cast<KEY_TO_Json>( _Key );
    jsonPr["CoefficientPollution"]     = _CoefficientPollution;
    jsonPr["CoefficientConsumption"] = _CoefficientConsumption;
    jsonPr["CoefficientSpeed"]         = _CoefficientSpeed;
    jsonPr["CoefficientProductivity"]  = _CoefficientProductivity;
    return 0;
  }

  bool FactoryModules::SetCountModules(int Count)
  {
    bool retval = _Modules.size() != Count;
    if (retval) _Modules.resize(Count);
    return retval;
  }

  double FactoryModules::GetSummSpeed(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC[moduleKey];
      retval += module.GetCoefficientSpeed() / 100.0;
    }
    return retval;
  }

  double FactoryModules::GetSummProductivity(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC[moduleKey];
      retval += module.GetCoefficientProductivity() / 100.0;
    }
    return retval;
  }

  double FactoryModules::GetSummEffectiveness(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC[moduleKey];
      retval += module.GetCoefficientConsumption() / 100.0;
    }
    //TODO выставить такие ограничения в настройки
    if (retval > 1.8) retval = 1.8;
    return retval;
  }

  double FactoryModules::GetSummPollution(const ModuleCollection & MC) const
  {
    double retval = 1.0;
    for (KEY_MODULE moduleKey : _Modules) {
      const Module &module = MC[moduleKey];
      retval += module.GetCoefficientPollution() / 100.0;
    }
    return retval;
  }

  void FactoryModules::DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel )
  {
    for ( auto & it : ModulesToDel ) {
      bool ToDel = false;
      for ( auto &itm : _Modules ) {
        if ( itm == it ) {
          itm = KEY_MODULE::ID_CleanSlot;
          break;
        }
      }
    }
  }

  int FactoryModules::ReadFromJson( const Json::Value & jsonPr )
  {
    _Modules.clear();
    for ( auto it : jsonPr ) {
      KEY_MODULE KM = static_cast<KEY_MODULE>( it.asInt64() );
      _Modules.push_back( KM );
    }
    return 0;
  }

  int FactoryModules::WriteToJson( Json::Value & jsonPr ) const
  {
    jsonPr = Json::Value( Json::arrayValue );
    for ( auto& it : _Modules ) {
      jsonPr.append( static_cast<KEY_TO_Json>( it ) );
    }
    return 0;
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