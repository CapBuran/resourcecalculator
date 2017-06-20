#include "FactoryCollection.h"

ResourceCalculator::FactoryCollection::FactoryCollection()
{
}

ResourceCalculator::FactoryCollection::~FactoryCollection()
{
}

void ResourceCalculator::FactoryCollection::ADD(const Factory &factory)
{
  _Factorys[factory.GetKey()] = factory;

}

int ResourceCalculator::FactoryCollection::ReadFromJson(const Json::Value & jsonPr)
{
  return 0;
}

int ResourceCalculator::FactoryCollection::WriteToJson(Json::Value & jsonPr) const
{
  return 0;
}


