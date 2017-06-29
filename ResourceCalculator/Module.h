#ifndef ModuleH
#define ModuleH

#include "Types.h"

namespace ResourceCalculator {
  
  class Module: public ItemBase, public Jsonable {
  private:
    KEY_MODULE _Key;
    bool _IsALiquidOrGas;
    Module();
    Module(const Module &Module);
  public:
    Module(std::string const &Name, KEY_MODULE Key);
    //Module& operator=(const Module& right);
    ~Module();
    KEY_MODULE GetKey() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
    friend std::pair<const KEY_MODULE, Module>;
  };



}

#endif
