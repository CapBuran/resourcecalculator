#include "ProductionChainTree.h"

namespace ResourceCalculator
{
ItemResultTree2::ItemResultTree2(const ParamsCollection & _PC, KEY_ITEM ItemKey_) :
  PC(_PC), ItemKey(ItemKey_)
{

}

ItemResultTree2 ItemResultTree2::Build(
  KEY_ITEM ItemID, int NestingResults,
  std::list<KEY_ITEM>& ListRequest,
  std::list<KEY_ITEM>& ListRequestResourceOnly)
{
  KEY_ITEM f = KEY_ITEM::ID_ITEM_NoFind_Item;
  ItemResultTree2 RetVal(PC, f);
  return RetVal;
}

RecipeResultTree2::RecipeResultTree2(const ParamsCollection & _PC, KEY_RECIPE RecipeKey_) :
  PC(_PC), RecipeKey(RecipeKey_)
{
}

RecipeResultTree2 RecipeResultTree2::BuildTree(
  KEY_RECIPE RecipeID, int NestingResults,
  std::list<KEY_ITEM>& ListRequest,
  std::list<KEY_ITEM>& ListRequestResourceOnly) const
{
  KEY_RECIPE f = KEY_RECIPE::ID_RECIPE_NoFindRecipe;
  RecipeResultTree2 RetVal(PC, f);
  return RetVal;
}

}