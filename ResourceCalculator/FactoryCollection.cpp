#include "FactoryCollection.h"

namespace ResourceCalculator {

  FactoryCollection::FactoryCollection():
    _NoFindFactory("No find factory", 0.0, 0.0)
  {
    _NoFindFactory._Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _NoFindFactory._Speed = 0.0;
    _NoFindFactory._CountSlotsForModules = 0;
    _NoFindFactory._CountSlotsForRecipes = 0;
    _NoFindFactory._Wear = 1.0;
    _NoFindFactory._PeakPower = INFINITY;
    _NoFindFactory._LevelOfPollution = INFINITY;
    _NoFindFactory._Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _NoFindFactory._Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _NoFindFactory._Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
  }

  FactoryCollection::~FactoryCollection()
  {
  }

  void FactoryCollection::ADD(const Factory &factory)
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
