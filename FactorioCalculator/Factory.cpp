#include "Factory.h"

namespace FactorioCalculator {

  Factory::Factory(const std::string &Name, double Speed, double Power):
    FactorioItem(Name)
  {
  }

  Factory::~Factory()
  {
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