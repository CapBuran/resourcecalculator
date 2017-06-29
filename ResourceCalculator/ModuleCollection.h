#ifndef ModuleCollectionH
#define ModuleCollectionH

#include "Module.h"

namespace ResourceCalculator {

  class ModuleCollection: public Jsonable
  {
  public:
    ModuleCollection();
    ~ModuleCollection();

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    //std::map<KEY_ITEM, Item> _Items;
    
    //const Item &GetItem(KEY_ITEM key) const;

  };

}

#endif // !ItemCollectionH
