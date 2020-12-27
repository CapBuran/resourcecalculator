#include "TransportBelt.h"

namespace ResourceCalculator {

  TransportBelt::TransportBelt():
    Speed(0.0),
    MaxDensity(0.0)
  {
  }

  double TransportBelt::GetMaxThroughput() const
  {
    return MaxDensity * Speed;
  }

  int TransportBelt::ReadFromJson(const Json::Value& jsonPr)
  {
    MaxDensity = jsonPr["MaxDensity"].asDouble();
    Speed = jsonPr["Speed"].asDouble();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  int TransportBelt::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["MaxDensity"] = MaxDensity;
    jsonPr["Speed"] = Speed;
    return 0;
  }

}