#ifndef ProductionChainTree_H
#define ProductionChainTree_H

#include <memory>
#include <ParamsCollection.h>

namespace ResourceCalculator
{
  class FullItemTree;

  enum class TreeNodeType
  {
    ITEM,
    RECIPE
  };

  template <typename KeyType, typename RelatedKey, TreeNodeType type>
  class TreeNode
  {
  public:
    using Ptr = std::unique_ptr<TreeNode<KeyType, RelatedKey, type> >;
    //const TreeNodeType Type;
    const KeyType MyKey;
    const std::vector<RelatedKey>& Childrens;
    const bool Found;
    TYPE_KEY Size() const { return static_cast<TYPE_KEY>(Childrens.size());}
    operator bool() const { return Found; }
  private:
    TreeNode(KeyType myID, const std::vector<RelatedKey>& childrens, bool found)
      : MyKey(myID)
      , Childrens(childrens)
      , Found(found)
      //, Type(type)
    {}
    friend FullItemTree;
  };

  using ItemNode = TreeNode<KEY_ITEM, KEY_RECIPE, TreeNodeType::ITEM>;
  using RecipeNode = TreeNode<KEY_RECIPE, KEY_ITEM, TreeNodeType::RECIPE>;

  class FullItemTree
  {
  public:
    FullItemTree(const ParamsCollection& PC);
    ~FullItemTree();
    void Rebuild();
    void CloneTo(FullItemTree& tree) const;
    const ParamsCollection& GetPC() const { return _PC; }
    const ItemNode& operator[] (KEY_ITEM key) const
    {
      auto it = _items.find(key);
      if (it == _items.end()) return _noFoundItem;
      return *it->second;
    }
    const RecipeNode& operator[] (KEY_RECIPE key) const
    {
      auto it = _recipes.find(key);
      if (it == _recipes.end()) return _noFoundRecipe;
      return *it->second;
    }
  private:
    const ItemNode _noFoundItem;
    const RecipeNode _noFoundRecipe;
    const std::vector<KEY_ITEM> _ChildrensNoFoundItems;
    const std::vector<KEY_RECIPE> _ChildrensNoRecipes;
    std::vector<std::vector<KEY_RECIPE> > _childrensItems;
    std::vector<std::vector<KEY_ITEM> > _childrensRecipes;
    const ParamsCollection& _PC;
    std::map<KEY_ITEM, ItemNode::Ptr> _items;
    std::map<KEY_RECIPE, RecipeNode::Ptr> _recipes;
  };

}

#endif //ProductionChainTree_H