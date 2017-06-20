#ifndef ParamsCollectionH
#define ParamsCollectionH

#include "ItemCollection.h"
#include "FactoryCollection.h"
#include "RecipeCollection.h"

namespace ResourceCalculator {

  class ParamsCollection: public Jsonable
  {
  public:
    FactoryCollection FC;
    RecipeCollection  RC;
    ItemCollection    IC;
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
  };

}

#endif // ParamsCollection
