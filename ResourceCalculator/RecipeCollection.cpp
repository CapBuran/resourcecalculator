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
    rc.CloneTo(*this);
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

bool RecipeCollection::Delete(const std::set<KEY_ITEM>& ItemsKeysToDel)
{
  const auto oldSize = _Recipes.size();
  for (KEY_ITEM ItemID : ItemsKeysToDel) {
    for (auto &recipe: _Recipes) {
      recipe.second.DeleteItem(ItemID);
    }
  }
  UpdateIndex();
  return oldSize > _Recipes.size();
}

bool RecipeCollection::Delete(const std::set<KEY_RECIPE>& RecipsKeysToDel)
{
  const auto oldSize = _Recipes.size();
  for (KEY_RECIPE RecipeID: RecipsKeysToDel) {
    _Recipes.erase(RecipeID);
  }
  UpdateIndex();
  return oldSize > _Recipes.size();
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
