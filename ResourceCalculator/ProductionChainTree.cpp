#include "ProductionChainTree.h"

namespace ResourceCalculator
{

template <typename key_type>
class ResultElement
{
private:
  std::map <key_type, int> _Levels;
public:
  inline ResultElement()
  {
  }
  inline void AddLevel(key_type Type, int Level)
  {
    auto f = _Levels.find(Type);
    if (f == _Levels.end()) {
      _Levels[Type] = Level;
    }
    else {
      if (f->second > Level) f->second = Level;
    }
  }
  inline std::list <key_type> GetList() const
  {
    std::list <key_type> Result;
    int Min = 0, Max = 0;
    if (_Levels.size() > 0) {
      Min = _Levels.begin()->second;
      Max = Min;
    }
    for (auto IT : _Levels) {
      if (IT.second > Max) Max = IT.second;
      if (IT.second < Min) Min = IT.second;
    }
    for (int i = Min; i <= Max; i++) {
      for (auto IT : _Levels) {
        if (i == IT.second) {
          auto F = std::find(Result.begin(), Result.end(), IT.first);
          if (F == Result.end()) {
            Result.push_back(IT.first);
          }
        }
      }
    }
    return Result;
  }
};

ItemResultTree::ItemResultTree(const ParamsCollection &_PC, KEY_ITEM _ItemID, KEY_RECIPE _Parent):
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

KEY_RECIPE ItemResultTree::GetChildren(size_t ID) const
{
  return _Result[ID];
}

size_t ItemResultTree::GetCountChildrens() const
{
  return _Result.size();
}

void ItemResultTree::Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes, std::list<KEY_RECIPE>& ResultRecipes, std::list<KEY_ITEM>& ResultItems) const
{
  ResultElement<KEY_RECIPE> ResultRecipes1;
  ResultElement<KEY_ITEM> ResultItems1;
  _Travelling(Nesting, AnsferItems, AnsferRecipes, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

void ItemResultTree::_Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes, ResultElement<KEY_RECIPE>& RetRecipes, ResultElement<KEY_ITEM>& RetItems) const
{
  if (Nesting <= 0) return;
  for (KEY_RECIPE RecipeID: _Result){
    AnsferRecipes[RecipeID] = ItemID;
    auto &IT = AnsferItems.find(ItemID);
    if (IT != AnsferItems.end()) {
      if (IT->second == KEY_RECIPE::ID_RECIPE_FindRecipeROOT) {
        //RetItems.AddLevel(ItemID, 1);
        //continue;
        Nesting = 2;
      }
    }
    RecipeResultTree RRT(PC, RecipeID, ItemID);
    RRT._Travelling(Nesting - 1, AnsferItems, AnsferRecipes, RetRecipes, RetItems);
  }
  RetItems.AddLevel(ItemID, Nesting);
}

RecipeResultTree::RecipeResultTree(const ParamsCollection & _PC, KEY_RECIPE _RecipeID, KEY_ITEM _Parent):
  PC(_PC), RecipeID(_RecipeID), Parent(_Parent)
{
}

KEY_ITEM RecipeResultTree::GetChildren(size_t ID) const
{
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return KEY_ITEM::ID_ITEM_NoFind_Item;
  const std::set<CountsItem> &ResultItems = recipe->GetResult();
  if (ResultItems.size() <= ID) return KEY_ITEM::ID_ITEM_NoFind_Item;
  std::set<CountsItem>::const_iterator it = ResultItems.begin();
  for (size_t i = 0; i < ID; i++, it++);
  return it->ItemId;
}

size_t RecipeResultTree::GetCountChildrens() const
{
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return 0;
  return recipe->GetResult().size();
}

void RecipeResultTree::Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes, std::list<KEY_RECIPE>& ResultRecipes, std::list<KEY_ITEM>& ResultItems) const
{
  ResultElement<KEY_RECIPE> ResultRecipes1;
  ResultElement<KEY_ITEM> ResultItems1;
  _Travelling(Nesting, AnsferItems, AnsferRecipes, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

void RecipeResultTree::_Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes, ResultElement<KEY_RECIPE>& RetRecipes, ResultElement<KEY_ITEM>& RetItems) const
{
  if (Nesting <= 0) return;
  const Recipe* recipe = PC.RC.GetRecipe(RecipeID);
  if (recipe == nullptr) return;
  const std::set<CountsItem> &RequiredItems = recipe->GetRequired();
  for (auto &it : RequiredItems) {
    ItemResultTree IRT(PC, it.ItemId, RecipeID);
    IRT._Travelling(Nesting - 1, AnsferItems, AnsferRecipes, RetRecipes, RetItems);
  }
  RetRecipes.AddLevel(RecipeID, Nesting);
}


}

