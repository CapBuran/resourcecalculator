#include "FactoryCollection.h"

namespace ResourceCalculator {

  FactoryCollection::FactoryCollection()
  {
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
    return _Factorys.find(Key)->second;
  }

  const std::map<KEY_FACTORY, Factory>& FactoryCollection::GetFactorys() const
  {
    return _Factorys;
  }

  int FactoryCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int FactoryCollection::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

}
