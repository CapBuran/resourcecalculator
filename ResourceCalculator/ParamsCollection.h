#ifndef ParamsCollectionH
#define ParamsCollectionH

#include "ItemCollection.h"
#include "RecipeCollection.h"
#include "ModuleCollection.h"
#include "FactoryCollection.h"
#include "IconCollection.h"
#include "TransportBeltCollection.h"

namespace ResourceCalculator
{
  class ParamsCollection: public Jsonable
  {
  public:
    FactoryCollection FC;
    RecipeCollection  RC;
    ItemCollection    IC;
    ModuleCollection  MC;
    IconCollection Icons;
    TransportBeltCollection TC;

    ParamsCollection();
    
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
  };
}

#endif // ParamsCollection
