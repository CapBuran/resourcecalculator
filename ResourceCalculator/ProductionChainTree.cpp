#include "ProductionChainTree.h"

namespace ResourceCalculator
{

ItemResultTree2::ItemResultTree2(const ParamsCollection &_PC, KEY_ITEM _ItemID, KEY_RECIPE _Parent):
  PC(_PC), ItemID(_ItemID), Parent(_Parent)
{
  const std::map<KEY_RECIPE, Recipe> & Recipes = PC.RC.GetData();
  std::set<KEY_RECIPE> result_all;
  for (auto &r: Recipes){
    const std::set<CountsItem> &result = r.second.GetResult();
    for (auto &c: result){
      if (c.ItemId == ItemID) {
        result_all.insert(r.first);
      }
    }
  }
  _Result.resize(result_all.size());
  std::copy(result_all.begin(), result_all.end(), _Result.begin());
}

KEY_RECIPE ItemResultTree2::GetChildren(size_t ID) const
{
  return _Result[ID];
}

size_t ItemResultTree2::GetCountChildrens() const
{
  return _Result.size();
}

void ItemResultTree2::Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, std::list<KEY_RECIPE>& ResultRecipes, std::list<KEY_ITEM>& ResultItems) const
{
  ResultElement2<KEY_RECIPE> ResultRecipes1;
  ResultElement2<KEY_ITEM> ResultItems1;
  _Travelling(Nesting, Ansfer, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

void ItemResultTree2::_Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, ResultElement2<KEY_RECIPE>& RetRecipes, ResultElement2<KEY_ITEM>& RetItems) const
{
  if (Nesting <= 0) return;
  for (KEY_RECIPE RecipeID: _Result){
    RecipeResultTree2 RRT(PC, RecipeID, ItemID);
    RRT._Travelling(Nesting - 1, Ansfer, RetRecipes, RetItems);
  }
  RetItems.AddLevel(ItemID, Nesting);
}

RecipeResultTree2::RecipeResultTree2(const ParamsCollection & _PC, KEY_RECIPE _RecipeID, KEY_ITEM _Parent):
  PC(_PC), RecipeID(_RecipeID), Parent(_Parent)
{
}

KEY_ITEM RecipeResultTree2::GetChildren(size_t ID) const
{
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return KEY_ITEM::ID_ITEM_NoFind_Item;
  const std::set<CountsItem> &ResultItems = recipe->GetResult();
  if (ResultItems.size() <= ID) return KEY_ITEM::ID_ITEM_NoFind_Item;
  std::set<CountsItem>::const_iterator it = ResultItems.begin();
  for (size_t i = 0; i < ID; i++, it++);
  return it->ItemId;
}

size_t RecipeResultTree2::GetCountChildrens() const
{
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return 0;
  return recipe->GetResult().size();
}

void RecipeResultTree2::Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, std::list<KEY_RECIPE>& ResultRecipes, std::list<KEY_ITEM>& ResultItems) const
{
  ResultElement2<KEY_RECIPE> ResultRecipes1;
  ResultElement2<KEY_ITEM> ResultItems1;
  _Travelling(Nesting, Ansfer, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

void RecipeResultTree2::_Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer,
  ResultElement2<KEY_RECIPE>& RetRecipes, ResultElement2<KEY_ITEM>& RetItems) const
{
  if (Nesting <= 0) return;
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return;
  const std::set<CountsItem> &RequiredItems = recipe->GetRequired();
  for (auto &it : RequiredItems) {
    ItemResultTree2 IRT(PC, it.ItemId, RecipeID);
    IRT._Travelling(Nesting - 1, Ansfer, RetRecipes, RetItems);
  }
  RetRecipes.AddLevel(RecipeID, Nesting);
}


}

