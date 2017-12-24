#ifndef ModuleCollectionH
#define ModuleCollectionH

#include "Module.h"

namespace ResourceCalculator {

  class ModuleCollection: public Jsonable
  {
  public:
    ModuleCollection();
    ~ModuleCollection();
    const std::map<KEY_MODULE, Module>& GetModules() const;
    const Module & GetModule(KEY_MODULE ModuleKey) const;
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
    void AddModules( const std::map<KEY_MODULE, Module>& ModulesToAdd );
    void DeleteModules(const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel );
    KEY_MODULE GetUniqueModuleKey();

  private:
    KEY_MODULE _LastRetvalUniqueModuleKey;
    std::map<KEY_MODULE, Module> _Modules;
  };

}

#endif // !ItemCollectionH
