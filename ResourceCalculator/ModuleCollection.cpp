#include "ModuleCollection.h"

namespace ResourceCalculator {

  ModuleCollection::ModuleCollection()
  {
  }

  ModuleCollection::~ModuleCollection()
  {

  }

  const Module & ModuleCollection::GetModule(KEY_MODULE ModuleKey) const
  {
    return _Modules.find(ModuleKey)->second;
  }

  int ModuleCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int ModuleCollection::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

}
