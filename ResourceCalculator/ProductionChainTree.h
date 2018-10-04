#ifndef ProductionChainTree_H
#define ProductionChainTree_H

#include <vector>
#include <list>
#include <map>

#include "ParamsCollection.h"

namespace ResourceCalculator {

template <typename key_type> class ResultElement;

class RecipeResultTree;

class ItemResultTree
{
private:
  std::vector<KEY_RECIPE> _Result;
  void _Travelling(int Nesting,
                   const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes,
                   ResultElement<KEY_RECIPE>& RetRecipes, ResultElement<KEY_ITEM>& RetItems) const;

public:
  const KEY_RECIPE Parent;
  const KEY_ITEM   ItemID;
  const ParamsCollection &PC;
  ItemResultTree(const ParamsCollection &PC, KEY_ITEM ItemID, KEY_RECIPE Parent = KEY_RECIPE::ID_RECIPE_NoFindRecipe);
  KEY_RECIPE GetChildren(size_t ID) const;
  size_t GetCountChildrens() const;
  void Travelling(int Nesting,
                  const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes,
                  std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;
  friend RecipeResultTree;
};

class RecipeResultTree
{
private:
  void _Travelling(int Nesting,
                   const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes,
                   ResultElement<KEY_RECIPE>& RetRecipes, ResultElement<KEY_ITEM>& RetItems) const;
public:
  const KEY_ITEM   Parent;
  const KEY_RECIPE RecipeID;
  const ParamsCollection &PC;
  RecipeResultTree(const ParamsCollection &PC, KEY_RECIPE RecipeID, KEY_ITEM Parent = KEY_ITEM::ID_ITEM_NoFind_Item);
  KEY_ITEM GetChildren(size_t ID) const;
  size_t GetCountChildrens() const;
  void Travelling(int Nesting,
                  const std::map<KEY_ITEM, KEY_RECIPE>& AnsferItems, std::map<KEY_RECIPE, KEY_ITEM>& AnsferRecipes,
                  std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;
  friend ItemResultTree;
};

}

#endif //ProductionChainTree_H