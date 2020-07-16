#include "RecipeCollection.h"
#include <algorithm>

namespace ResourceCalculator {

RecipeCollection::RecipeCollection():
  _LastGenGey(0)
{
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
