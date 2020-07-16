#include "ProductionChainTree.h"

namespace ResourceCalculator
{

FullItemTree::FullItemTree(const ParamsCollection& PC)
  : _PC(PC)
  , _noFoundItem(KEY_ITEM::ID_ITEM_NoFind_Item, KEY_RECIPE::ID_RECIPE_NoFindRecipe, {}, TreeBaseType::ITEM)
  , _noFoundRecipe(KEY_RECIPE::ID_RECIPE_NoFindRecipe, KEY_ITEM::ID_ITEM_NoFind_Item, {}, TreeBaseType::RECIPE)
{
  Rebuild();
}

ItemResultTree* FullItemTree::FactoryItemTree(KEY_ITEM id, KEY_RECIPE parent) const
{
  auto found = _itemKeyToKeyIndex.find(id);
  if (found != _itemKeyToKeyIndex.end())
  {
    int index = found->second;
    if (0 <= index && index < _items.size())
    {
      if (_items[index])
      {
        const ItemResultTree& F = *_items[index];
        return new ItemResultTree(F, parent);
      }
      else
      {
        return nullptr;
      }
    }
  }

  return nullptr;
}

RecipeResultTree* FullItemTree::FactoryRecipeTree(KEY_RECIPE id, KEY_ITEM parent) const
{
  auto found = _recipeKeyToKeyIndex.find(id);
  if (found != _recipeKeyToKeyIndex.end())
  {
    int index = found->second;
    if (0 <= index && index < _items.size())
    {
      if (_items[index])
      {
        const RecipeResultTree& F = *_recipes[index];
        return new RecipeResultTree(F, parent);
      }
      else
      {
        return nullptr;
      }
    }
  }

  return nullptr;
}

const ItemResultTree& FullItemTree::GetRootItemTree(KEY_ITEM id) const
{
  auto found = _itemKeyToKeyIndex.find(id);
  if (found != _itemKeyToKeyIndex.end())
  {
    int index = found->second;
    if (0 <= index && index < _items.size())
    {
      if (_items[index])
      {
        return *_items[index];
      }
      else
      {
        return _noFoundItem;
      }
    }
  }

  return _noFoundItem;
}

const RecipeResultTree& FullItemTree::GetRootRecipeTree(KEY_RECIPE id) const
{
  auto found = _recipeKeyToKeyIndex.find(id);
  if (found != _recipeKeyToKeyIndex.end())
  {
    int index = found->second;
    if (0 <= index && index < _recipes.size())
    {
      if (_recipes[index])
      {
        return *_recipes[index];
      }
      else
      {
        return _noFoundRecipe;
      }
    }
  }
  return _noFoundRecipe;
}

void FullItemTree::Rebuild()
{
  _itemKeyToKeyIndex.clear();
  _recipeKeyToKeyIndex.clear();
  _childrensItems.clear();
  _childrensRecipes.clear();
  _items.clear();
  _recipes.clear();

  int index = 0;

  const std::map<KEY_RECIPE, Recipe>& recipes = _PC.RC.GetData();
  const std::map<KEY_ITEM, Item>& items = _PC.IC.GetData();

  _items.resize(items.size());
  _childrensItems.resize(items.size());
  _recipes.resize(recipes.size());
  _childrensRecipes.resize(recipes.size());

  for (const auto& item : items)
  {
    _itemKeyToKeyIndex[item.first] = index;
    _items[index].reset(new ItemResultTree(item.first, KEY_RECIPE::ID_RECIPE_NoFindRecipe, _childrensItems[index], TreeBaseType::ITEM));
    index++;
  }

  index = 0;

  for (const auto& recipe: recipes)
  {
    _recipeKeyToKeyIndex[recipe.first] = index;
    auto& results = recipe.second.GetRequired();
    auto& required = recipe.second.GetResult();
    _childrensRecipes[index].resize(results.size());
    std::copy(results.begin(), results.end(), _childrensRecipes[index].begin());
    for (auto item_counter: required)
    {
      _childrensItems[_itemKeyToKeyIndex[item_counter]].push_back(recipe.first);
    }
    _recipes[index].reset(new RecipeResultTree(recipe.first, KEY_ITEM::ID_ITEM_NoFind_Item, _childrensRecipes[index], TreeBaseType::RECIPE));
    index++;
  }
}

}

