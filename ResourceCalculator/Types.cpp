#include "Types.h"

namespace ResourceCalculator {

  int ItemBase::ReadFromJson(const Json::Value & jsonPr)
  {
    _Name = jsonPr["Name"]["ru"].asString();
    _IconPath = jsonPr["IconPath"].asString();
    return 0;
  }

  int ItemBase::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr["Name"]["ru"] = _Name;
    jsonPr["Name"]["en"] = "English lang";
    jsonPr["IconPath"] = _IconPath;
    return 0;
  }

}
