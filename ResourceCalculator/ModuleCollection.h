#ifndef ModuleCollectionH
#define ModuleCollectionH

#include "Module.h"

namespace ResourceCalculator {

  class ModuleCollection: public Jsonable
  {
  public:
    ModuleCollection();
    ~ModuleCollection();
    const Module & GetModule(KEY_MODULE ModuleKey) const;
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_MODULE, Module> _Modules;
  };

}

#endif // !ItemCollectionH
