#ifndef ParamsCollectionH
#define ParamsCollectionH

#include "ItemCollection.h"
#include "RecipeCollection.h"
#include "ModuleCollection.h"
#include "FactoryCollection.h"
#include "IconCollection.h"
#include "ProductionChainCollection.h"

namespace ResourceCalculator {
  
  class ParamsCollection: public Jsonable
  {
  public:
    RecipeCollection  RC;
    ItemCollection    IC;
    FactoryCollection FC;
    ModuleCollection  MC;
    IconCollection Icons;
    ProductionChainCollection PCC;

    ParamsCollection();
    
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
  };

}

#endif // ParamsCollection
