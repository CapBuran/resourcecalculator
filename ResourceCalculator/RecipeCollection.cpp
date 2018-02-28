#include "RecipeCollection.h"
#include <algorithm>

namespace ResourceCalculator {

const std::map<KEY_RECIPE, RecipeResultTree> & ItemResultTree::GetResult() const
{
  return _Result;
}

KEY_ITEM ItemResultTree::GetItemKey() const
{
  return _ItemKey;
}

const std::map<KEY_ITEM, ItemResultTree> & RecipeResultTree::GetResult() const
{
  return _Result;
}

KEY_RECIPE RecipeResultTree::GetRecipeKey() const
{
  return _RecipeKey;
}

RecipeResultTree RecipeCollection::BuildTree(KEY_RECIPE RecipeID, int NestingResults, std::list<KEY_ITEM> &ListRequest, std::list<KEY_ITEM> &ListRequestResourceOnly) const
{
  RecipeResultTree RetVal;
  RetVal._RecipeKey = RecipeID;
  if (NestingResults == 0) return RetVal;
  std::map<KEY_RECIPE, Recipe>::const_iterator RP_FIND = _Recipes.find(RecipeID);
  if (RP_FIND != _Recipes.end()) {
    for (auto &it : RP_FIND->second.GetRequired()) {
      RetVal._Result[it.ItemId] = BuildTree(it.ItemId, NestingResults - 1, ListRequest, ListRequestResourceOnly);
    }
  }
  return RetVal;
}

void RecipeCollection::Travelling(
  const RecipeResultTree & Tree, int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer,
  ResultElement<KEY_RECIPE>& ResultRecipes, ResultElement<KEY_ITEM>& ResultItems) const
{
  const KEY_RECIPE FindRecipeKey = Tree.GetRecipeKey();
  const std::map<KEY_ITEM, ItemResultTree> & ResultTree = Tree.GetResult();
  for (auto &it : ResultTree) {
    Travelling(it.second, Nesting + 1, Ansfer, ResultRecipes, ResultItems);
  }
  ResultRecipes.AddLevel(FindRecipeKey, Nesting);
}

void RecipeCollection::Travelling(
  const ItemResultTree & Tree, int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer,
  ResultElement<KEY_RECIPE>& ResultRecipes, ResultElement<KEY_ITEM>& ResultItems) const
{
  const KEY_ITEM FindItemKey = Tree.GetItemKey();
  const std::map<KEY_RECIPE, RecipeResultTree> &ResultTree = Tree.GetResult();
  if (ResultTree.size() > 0) {
    auto FindAnsfer = Ansfer.find(FindItemKey);
    if (FindAnsfer != Ansfer.end()) {
      KEY_RECIPE RecipeKeyAnsfer = FindAnsfer->second;
      if (RecipeKeyAnsfer != KEY_RECIPE::ID_RECIPE_PreviouslyProduced) {
        auto FindRecipeTree = ResultTree.find(RecipeKeyAnsfer);
        if (FindRecipeTree != ResultTree.end()) {
          Travelling(FindRecipeTree->second, Nesting + 1, Ansfer, ResultRecipes, ResultItems);
        }
      }
    } else {
      for (auto ResulltID : ResultTree) {
        Travelling(ResulltID.second, Nesting + 1, Ansfer, ResultRecipes, ResultItems);
      }
    }
  }
  ResultItems.AddLevel(FindItemKey, Nesting);
}

void RecipeCollection::Travelling(const RecipeResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                                  std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const
{
  ResultElement<KEY_RECIPE> ResultRecipes1;
  ResultElement<KEY_ITEM> ResultItems1;
  Travelling(Tree, 1, Ansfer, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

void RecipeCollection::Travelling(const ItemResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                                  std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const
{
  ResultElement<KEY_RECIPE> ResultRecipes1;
  ResultElement<KEY_ITEM> ResultItems1;
  Travelling(Tree, 1, Ansfer, ResultRecipes1, ResultItems1);
  ResultRecipes = ResultRecipes1.GetList();
  ResultItems = ResultItems1.GetList();
}

RecipeCollection::RecipeCollection():
  _LastGenGey(0)
{
}

ItemResultTree RecipeCollection::BuildTree(KEY_ITEM ItemID, int NestingResults, std::list<KEY_ITEM> &ListRequest, std::list<KEY_ITEM> &ListRequestResourceOnly) const
{
  ItemResultTree RetVal;
  RetVal._ItemKey = ItemID;
  if (NestingResults == 0) return RetVal;
  bool IsFindOK = false;
  for (auto &it : _Recipes) {
    for (auto it2 : it.second.GetResult()) {
      if (it2.ItemId == ItemID) {
        RetVal._Result[it.second.GetKey()] = BuildTree(it.second.GetKey(), NestingResults - 1, ListRequest, ListRequestResourceOnly);
        std::list<KEY_ITEM>::iterator findIter = std::find(ListRequest.begin(), ListRequest.end(), ItemID);
        if (findIter == ListRequest.end()) {
          ListRequest.push_back(ItemID);
          IsFindOK = true;
        }
      }
    }
  }
  std::list<KEY_ITEM>::iterator findIter = std::find(ListRequestResourceOnly.begin(), ListRequestResourceOnly.end(), ItemID);
  if (!IsFindOK && findIter == ListRequestResourceOnly.end()) {
    ListRequestResourceOnly.push_back(ItemID);
  }
  for (auto it : ListRequest) {
    ListRequestResourceOnly.remove(it);
  }
  return RetVal;
}

void RecipeCollection::Add(const Recipe &recipe)
{
  _Recipes[recipe.GetKey()] = recipe;
}

int RecipeCollection::ReadFromJson(const Json::Value & jsonPr)
{
  for (auto it : jsonPr) {
    Recipe ToAdd;
    ToAdd.ReadFromJson(it);
    TYPE_KEY AddKey = static_cast<TYPE_KEY>(ToAdd.GetKey());
    if (_LastGenGey < AddKey) _LastGenGey = AddKey;
    Add(ToAdd);
  }
  return 0;
}

const std::map<KEY_RECIPE, Recipe> &RecipeCollection::GetData() const
{
  return _Recipes;
}

std::map<KEY_RECIPE, Recipe> &RecipeCollection::GetDataForEdit()
{
  return _Recipes;
}

const Recipe *RecipeCollection::GetRecipe(KEY_RECIPE KeyRecipe) const
{
  std::map<KEY_RECIPE, Recipe>::const_iterator it = _Recipes.find(KeyRecipe);
  if (it == _Recipes.end()) {
    return nullptr;
  }
  return &it->second;
}

void RecipeCollection::Delete(const std::set<KEY_ITEM>& ItemsKeysToDel)
{
  for (KEY_ITEM ItemID : ItemsKeysToDel) {
    for (auto &recipe : _Recipes) {
      recipe.second.DeleteItem(ItemID);
    }
  }
}

void RecipeCollection::Delete(const std::set<KEY_RECIPE>& RecipsKeysToDel)
{
  for (KEY_RECIPE RecipeID : RecipsKeysToDel) {
    _Recipes.erase(RecipeID);
  }
}

Recipe *RecipeCollection::GetRecipeForEdit(KEY_RECIPE KeyRecipe)
{
  std::map<KEY_RECIPE, Recipe>::iterator it = _Recipes.find(KeyRecipe);
  if (it == _Recipes.end()) {
    return nullptr;
  }
  return &it->second;
}

KEY_RECIPE RecipeCollection::GetUniqueRecipeKey()
{
  TYPE_KEY retval = _LastGenGey + 1;
  if (_Recipes.size() > 0) {
    while (_Recipes.find(static_cast<KEY_RECIPE>(retval)) != _Recipes.end()) {
      retval++;
    }
  }
  _LastGenGey = retval;
  return static_cast<KEY_RECIPE>(retval);
}

int RecipeCollection::WriteToJson(Json::Value & jsonPr) const
{
  jsonPr = Json::Value(Json::arrayValue);
  for (auto& it : _Recipes) {
    Json::Value newVal;
    it.second.WriteToJson(newVal);
    jsonPr.append(newVal);
  }
  return 0;
}
}
