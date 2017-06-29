#ifndef ParamsCollectionH
#define ParamsCollectionH

#include "ItemCollection.h"
#include "RecipeCollection.h"
#include "ModuleCollection.h"
#include "FactoryCollection.h"

namespace ResourceCalculator {

  class ParamsCollection: public Jsonable
  {
  public:
    ItemCollection    IC;
    RecipeCollection  RC;
    ModuleCollection  MC;
    FactoryCollection FC;
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
  };

}

#endif // ParamsCollection
