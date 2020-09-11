#include "RecipeCollection.h"
#include <algorithm>

namespace ResourceCalculator {
RecipeCollection::RecipeCollection(const FactoryTypeCollection& FTC)
  : _Recipes()
  , Indexator<KEY_RECIPE, Recipe>(_Recipes)
  , _FTC(FTC)
{
}

RecipeCollection::~RecipeCollection()
{
}

RecipeCollection::RecipeCollection(const RecipeCollection& copy)
  : _Recipes()
  , Indexator<KEY_RECIPE, Recipe>(_Recipes)
  , _FTC(copy._FTC)
{
  *this = copy;
}

RecipeCollection& RecipeCollection::operator=(const RecipeCollection& rc)
{
  if (this != &rc)
  {
    _Recipes = rc._Recipes;
    rc.CopyIndexes(*this);
  }
  return *this;
}

void RecipeCollection::Add(const Recipe &recipe)
{
  _Recipes[recipe.GetKey()] = recipe;
  UpdateIndex();
}

int RecipeCollection::ReadFromJson(const Json::Value & jsonPr)
{
  for (auto it : jsonPr) {
    Recipe ToAdd;
    ToAdd.ReadFromJson(it);
    TYPE_KEY AddKey = static_cast<TYPE_KEY>(ToAdd.GetKey());
    Add(ToAdd);
  }
  UpdateIndex();
  return 0;
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
    for (auto &recipe: _Recipes) {
      recipe.second.DeleteItem(ItemID);
    }
  }
  UpdateIndex();
}

void RecipeCollection::Delete(const std::set<KEY_RECIPE>& RecipsKeysToDel)
{
  for (KEY_RECIPE RecipeID: RecipsKeysToDel) {
    _Recipes.erase(RecipeID);
  }
  UpdateIndex();
}

Recipe *RecipeCollection::GetRecipeForEdit(KEY_RECIPE KeyRecipe)
{
  std::map<KEY_RECIPE, Recipe>::iterator it = _Recipes.find(KeyRecipe);
  if (it == _Recipes.end()) {
    return nullptr;
  }
  return &it->second;
}


int RecipeCollection::WriteToJson(Json::Value & jsonPr) const
{
  jsonPr = Json::Value(Json::arrayValue);
  for (auto& it: _Recipes) {
    Json::Value newVal;
    it.second.WriteToJson(newVal);
    jsonPr.append(newVal);
  }
  return 0;
}

const FactoryTypeCollection& RecipeCollection::GetFactoryTypes() const
{
  return _FTC;
}

}
