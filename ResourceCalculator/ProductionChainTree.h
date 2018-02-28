#ifndef ProductionChainTree_H
#define ProductionChainTree_H

#include "ParamsCollection.h"

namespace ResourceCalculator {

struct RecipeResultTree2;

struct ItemResultTree2
{
private:
  std::map<KEY_RECIPE, RecipeResultTree2> _Result;
public:
  const KEY_ITEM ItemKey;
  const ParamsCollection &PC;
  ItemResultTree2(const ParamsCollection &PC, KEY_ITEM ItemKey);
  ItemResultTree2 Build(
    KEY_ITEM ItemID, int NestingResults,
    std::list<KEY_ITEM> &ListRequest,
    std::list<KEY_ITEM> &ListRequestResourceOnly
  );
  inline const std::map<KEY_RECIPE, RecipeResultTree2> & GetResult() const
  {
    return _Result;
  }
};

struct RecipeResultTree2
{
private:
  
  std::map<KEY_ITEM, ItemResultTree2> _Result;
public:
  const KEY_RECIPE RecipeKey;
  const ParamsCollection &PC;
  RecipeResultTree2(const ParamsCollection &PC, KEY_RECIPE RecipeKey);
  RecipeResultTree2 BuildTree(
    KEY_RECIPE RecipeID,
    int NestingResults,
    std::list<KEY_ITEM> &ListRequest,
    std::list<KEY_ITEM> &ListRequestResourceOnly
  ) const;
  inline const std::map<KEY_ITEM, ItemResultTree2> & GetResult() const
  {
    return _Result;
  }
};
}

#endif //ProductionChainTree_H