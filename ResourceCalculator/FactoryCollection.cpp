#include "FactoryCollection.h"

namespace ResourceCalculator {

  FactoryCollection::FactoryCollection()
  {
    _NoFindFactory.SetKey(KEY_FACTORY::ID_ITEM_NoFind_Factory);
    _NoFindFactory.SetSpeed(0.0);
    _NoFindFactory.SetCountSlotsForModules(0);
    _NoFindFactory.SetCountSlotsForRecipes(0);
    _NoFindFactory.SetWear(1.0);
    _NoFindFactory.SetPeakPower(INFINITY);
    _NoFindFactory.SetLevelOfPollution(INFINITY);
    _NoFindFactory.SetKey(KEY_FACTORY::ID_ITEM_NoFind_Factory);
    _NoFindFactory.SetKey(KEY_FACTORY::ID_ITEM_NoFind_Factory);
    _NoFindFactory.SetKey(KEY_FACTORY::ID_ITEM_NoFind_Factory);
  }

  FactoryCollection::~FactoryCollection()
  {
  }

  void FactoryCollection::ADD(const Factory factory)
  {
    _Factorys[factory.GetKey()] = factory;
  }

  const Factory & FactoryCollection::GetFactory(KEY_FACTORY Key) const
  {
    if (Key == KEY_FACTORY::ID_ITEM_NoFind_Factory) return _NoFindFactory;
    return _Factorys.find(Key)->second;
  }

  const std::map<KEY_FACTORY, Factory>& FactoryCollection::GetFactorys() const
  {
    return _Factorys;
  }

  int FactoryCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    _Factorys.clear();
    for (auto it : jsonPr) {
      Factory factory;
      factory.ReadFromJson(it);
      _Factorys[factory.GetKey()] = factory;
    }
    return 0;
  }

  int FactoryCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Factorys) {
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

}
