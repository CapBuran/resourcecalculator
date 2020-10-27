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

  class TreeNodeBase
  {
  public:
    const TreeNodeType Type;
    TreeNodeBase(TreeNodeType type)
      : Type(type)
    {}
    virtual TYPE_KEY Size() const = 0;
    virtual ~TreeNodeBase() {};
  protected:
    static const std::string& GetEmptyString();
  };

  template <typename KeyType, typename RelatedKey, TreeNodeType type>
  class TreeNode : public TreeNodeBase
  {
  public:
    using Ptr = std::unique_ptr<TreeNode<KeyType, RelatedKey, type> >;
    const KeyType MyKey;
    const std::vector<RelatedKey>& Childrens;
    TYPE_KEY Size() const override {
      return static_cast<TYPE_KEY>(Childrens.size());
    }
  private:
    TreeNode(KeyType myID, const std::vector<RelatedKey>& childrens)
      : MyKey(myID)
      , Childrens(childrens)
      , TreeNodeBase(type)
    {}
    friend FullItemTree;
  };

  using ItemNode = TreeNode<KEY_ITEM, KEY_RECIPE, TreeNodeType::ITEM>;
  using RecipeNode = TreeNode<KEY_RECIPE, KEY_ITEM, TreeNodeType::RECIPE>;

  class FullItemTree
    : public Indexator<KEY_ITEM, ItemNode::Ptr>
    , public Indexator<KEY_RECIPE, RecipeNode::Ptr>
  {
  public:
    FullItemTree(const ParamsCollection& PC);
    ~FullItemTree();
    void Rebuild();
    void CloneTo(FullItemTree& tree) const;
    const ParamsCollection& GetPC() const { return _PC; }
  private:
    const ItemNode _noFoundItem;
    const RecipeNode _noFoundRecipe;
    std::vector<std::vector<KEY_RECIPE> > _childrensItems;
    std::vector<std::vector<KEY_ITEM> > _childrensRecipes;
    const ParamsCollection& _PC;
    std::map<KEY_ITEM, ItemNode::Ptr> _items;
    std::map<KEY_RECIPE, RecipeNode::Ptr> _recipes;
  };

}

#endif //ProductionChainTree_H