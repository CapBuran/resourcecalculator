#include "Factory.h"

namespace ResourceCalculator {

  Factory::Factory(const std::string &Name, double Speed, double Power):
    FactorioItem(Name)
  {
  }

  Factory::Factory():
    FactorioItem("")
  {
  }

  Factory::Factory(const Factory &factory):
    FactorioItem(factory.GetName())
  {
  }

  Factory::~Factory()
  {
  }

  KEY_FACTORY Factory::GetKey() const
  {
    return _Key;
  }

  int Factory::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Factory::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

}