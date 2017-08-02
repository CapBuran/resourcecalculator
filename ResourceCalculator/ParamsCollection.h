#ifndef ParamsCollectionH
#define ParamsCollectionH

#include "ItemCollection.h"
#include "RecipeCollection.h"
#include "ModuleCollection.h"
#include "FactoryCollection.h"
#include "IconCollection.h"

namespace ResourceCalculator {

  class ParamsCollection: public Jsonable
  {
  public:
    ItemCollection    IC;
    RecipeCollection  RC;
    ModuleCollection  MC;
    FactoryCollection FC;
    IconCollection Icons;
    
    bool DeleteRecipe(KEY_RECIPE KeyRecipe);
    bool DeleteItem(KEY_ITEM ItemID);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
  };

}

#endif // ParamsCollection
