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

FullItemTree::~FullItemTree()
{
}

ItemResultTree* FullItemTree::FactoryItemTree(KEY_ITEM id, KEY_RECIPE parent ) const
{
  auto found = _itemKeyToKeyIndex.find(id);
  if (found != _itemKeyToKeyIndex.end())
  {
    TYPE_KEY index = found->second;
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
    TYPE_KEY index = found->second;
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
    TYPE_KEY index = found->second;
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
    TYPE_KEY index = found->second;
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

  {
    const TYPE_KEY Size = _PC.IC.Size();
    _items.resize(Size);
    _childrensItems.resize(Size);
    for (TYPE_KEY i = 0; i < Size; i++)
    {
      KEY_ITEM ki = _PC.IC.GetEnumKeyByKey(i);
      _itemKeyToKeyIndex[ki] = i;
      _items[i].reset(new ItemResultTree(ki, KEY_RECIPE::ID_RECIPE_NoFindRecipe, _childrensItems[i], TreeBaseType::ITEM));
    }
  }

  {
    const TYPE_KEY Size = _PC.RC.Size();
    _recipes.resize(Size);
    _childrensRecipes.resize(Size);
    for (TYPE_KEY i = 0; i < Size; i++)
    {
      KEY_RECIPE kr = _PC.RC.GetEnumKeyByKey(i);
      _recipeKeyToKeyIndex[kr] = i;
      const Recipe& recipe = *_PC.RC.GetRecipe(kr);
      auto& results = recipe.GetRequired();
      auto& required = recipe.GetResult();
      _childrensRecipes[i].resize(results.size());
      std::copy(results.begin(), results.end(), _childrensRecipes[i].begin());
      for (auto item_counter: required)
      {
        _childrensItems[_itemKeyToKeyIndex[item_counter]].push_back(kr);
      }
      _recipes[i].reset(new RecipeResultTree(kr, KEY_ITEM::ID_ITEM_NoFind_Item, _childrensRecipes[i], TreeBaseType::RECIPE));
    }
  }
}

}

