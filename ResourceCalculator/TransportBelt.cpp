#include "TransportBelt.h"

namespace ResourceCalculator {

  int TransportBelt::ReadFromJson(const Json::Value & jsonPr)
  {
    _Key = static_cast<KEY_TRANSPORT_BELT>(jsonPr["Key"].asInt64());
    _MaxDensity = jsonPr["MaxDensity"].asDouble();
    _Speed = jsonPr["Speed"].asDouble();
    ItemBase::ReadFromJson(jsonPr);
    return 0;
  }

  TransportBelt::TransportBelt():
    _Key(KEY_TRANSPORT_BELT::Unknown),
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
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["MaxDensity"] = _MaxDensity;
    jsonPr["Speed"] = _Speed;
    return 0;
  }

}