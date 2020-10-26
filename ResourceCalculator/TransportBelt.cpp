#include "TransportBelt.h"

namespace ResourceCalculator {

  int TransportBelt::ReadFromJson(const Json::Value & jsonPr)
  {
    _MaxDensity = jsonPr["MaxDensity"].asDouble();
    _Speed = jsonPr["Speed"].asDouble();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  TransportBelt::TransportBelt():
    _Speed(0.0),
    _MaxDensity(0.0)
  {
  }

  double TransportBelt::GetMaxThroughput() const
  {
    return _MaxDensity * _Speed;
  }

  int TransportBelt::WriteToJson(Json::Value & jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    jsonPr["MaxDensity"] = _MaxDensity;
    jsonPr["Speed"] = _Speed;
    return 0;
  }

}