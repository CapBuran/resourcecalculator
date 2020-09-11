#ifndef ModuleCollectionH
#define ModuleCollectionH

#include "Module.h"

namespace ResourceCalculator {

  class ModuleCollection: public Jsonable, public Indexator<KEY_MODULE, Module>
  {
  public:
    ModuleCollection();
    virtual ~ModuleCollection();

    const Module& GetModule(KEY_MODULE ModuleKey) const;

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
    
    void Add( const std::map<KEY_MODULE, Module>& ModulesToAdd );
    void Delete(const std::set<KEY_MODULE>& ModulesToDel );

  private:
    std::map<KEY_MODULE, Module> _Modules;
  };

}

#endif // !ItemCollectionH
