#ifndef ProductionChainTree_H
#define ProductionChainTree_H

#include <ParamsCollection.h>

namespace ResourceCalculator
{

class FullItemTree;

enum class TreeBaseType
{
  ITEM,
  RECIPE
};

class TreeBase
{
public:
  const TreeBaseType Type;
  TreeBase(TreeBaseType type)
    : Type(type)
  {}
  virtual int GetCountChildrens() const = 0;
  virtual ~TreeBase() {};
};

template <typename KeyType, typename RelatedKey>
class Tree: public TreeBase
{
public:
  const KeyType MyKey;
  const RelatedKey ParentKey;
  const std::vector<RelatedKey>& Childrens;
private:
  Tree(KeyType myID, RelatedKey parent, const std::vector<RelatedKey>& childrens, TreeBaseType type)
    : MyKey(myID)
    , ParentKey(parent)
    , Childrens(childrens)
    , TreeBase(type)
  {
  }
  Tree(const Tree<KeyType, RelatedKey>& original, RelatedKey parent)
    : MyKey(original.MyKey)
    , ParentKey(parent)
    , Childrens(original.Childrens)
    , TreeBase(original.Type)
  {
  }
  int GetCountChildrens() const override { return static_cast<int>(Childrens.size()); }
  friend FullItemTree;
};

using ItemResultTree = Tree<KEY_ITEM, KEY_RECIPE>;
using RecipeResultTree = Tree<KEY_RECIPE, KEY_ITEM>;

class FullItemTree
{
public:
  FullItemTree(const ParamsCollection& PC);
  ItemResultTree* FactoryItemTree(KEY_ITEM id, KEY_RECIPE parent = KEY_RECIPE::ID_RECIPE_NoFindRecipe) const;
  RecipeResultTree* FactoryRecipeTree(KEY_RECIPE id, KEY_ITEM parent = KEY_ITEM::ID_ITEM_NoFind_Item) const;
  const ItemResultTree& GetRootItemTree(KEY_ITEM id) const;
  const RecipeResultTree& GetRootRecipeTree(KEY_RECIPE id) const;
  void Rebuild();
  const ParamsCollection& GetPC() const { return _PC; }
private:
  const ParamsCollection& _PC;

  ItemResultTree _noFoundItem;
  RecipeResultTree _noFoundRecipe;
  std::map<KEY_ITEM, int> _itemKeyToKeyIndex;
  std::map<KEY_RECIPE, int> _recipeKeyToKeyIndex;

  std::vector<std::vector<KEY_RECIPE> > _childrensItems;
  std::vector<std::vector<KEY_ITEM> > _childrensRecipes;

  std::vector< std::unique_ptr<ItemResultTree> > _items;
  std::vector< std::unique_ptr<RecipeResultTree> > _recipes;
};


}

#endif //ProductionChainTree_H