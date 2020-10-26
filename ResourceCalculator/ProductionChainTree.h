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
    const RelatedKey ParentKey;
    const std::vector<RelatedKey>& Childrens;
    TYPE_KEY Size() const override {
      return static_cast<TYPE_KEY>(Childrens.size());
    }
  private:
    TreeNode(KeyType myID, const std::vector<RelatedKey>& childrens, RelatedKey parent = static_cast<RelatedKey>(0))
      : MyKey(myID)
      , ParentKey(parent)
      , Childrens(childrens)
      , TreeNodeBase(type)
    {}
    //TreeNode(const TreeNode<KeyType, RelatedKey, type>& original, RelatedKey parent)
    //  : MyKey(original.MyKey)
    //  , ParentKey(parent)
    //  , Childrens(original.Childrens)
    //  , TreeNodeBase(original.Type)
    //{}
    //TreeNode(const TreeNode<KeyType, RelatedKey, type>& original)
    //  : MyKey(original.MyKey)
    //  , ParentKey(original.ParentKey)
    //  , Childrens(original.Childrens)
    //  , TreeNodeBase(original.Type)
    //{}
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
    //ItemNode* FactoryItemTree(KEY_ITEM id, KEY_RECIPE parent) const;
    //RecipeNode* FactoryRecipeTree(KEY_RECIPE id, KEY_ITEM parent) const;
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