#include "Module.h"

namespace ResourceCalculator {

  Module::Module(): 
    ItemBase("")
  {
  }

  Module::Module(const std::string &Name, KEY_MODULE Key):
    ItemBase(Name), _Key(Key)
  {
  }

  Module::Module(const Module & Module):
    ItemBase(Module.GetName()), _Key(Module._Key)
  {
  }

  //Module & Module::operator=(const Module & right)
  //{
  //  if (this == &right) {
  //    return *this;
  //  }
  //  _Key = right._Key;
  //  _Name = right._Name;
  //  return *this;
  //}

  Module::~Module()
  {
  }

  KEY_MODULE Module::GetKey() const
  {
    return _Key;
  }

  int Module::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Module::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr["Key"] = static_cast<KEY_TO_Json>(_Key);
    jsonPr["Name"]["ru"] = _Name;
    jsonPr["Name"]["en"] = "English lang";
    return 0;
  }

}