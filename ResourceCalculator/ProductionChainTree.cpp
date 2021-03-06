#include "ProductionChainTree.h"

namespace ResourceCalculator
{

  FullItemTree::FullItemTree(const ParamsCollection& PC)
    : _PC(PC)
    , _noFoundItem(KEY_ITEM::ID_ITEM_NoFind_Item, _ChildrensNoRecipes, false)
    , _noFoundRecipe(KEY_RECIPE::ID_RECIPE_NoFindRecipe, _ChildrensNoFoundItems, false)
  {
    Rebuild();
  }

  FullItemTree::~FullItemTree()
  {
  }

  void FullItemTree::Rebuild()
  {
    _childrensItems.clear();
    _childrensRecipes.clear();
    _items.clear();
    _recipes.clear();
    
    {
      const TYPE_KEY Size = _PC.IC.Size();
      _childrensItems.resize(static_cast<size_t>(Size));
      for (TYPE_KEY i = 0; i < Size; i++)
      {
        auto ki = _PC.IC(i);
        _items.insert({ki, std::move(ItemNode::Ptr(new ItemNode(ki, _childrensItems[i], true)))});
      }
      //Indexator<KEY_ITEM, ItemNode::Ptr>::UpdateIndex();
    }

    {
      const TYPE_KEY Size = _PC.RC.Size();
      _childrensRecipes.resize(Size);
      for (TYPE_KEY i = 0; i < Size; i++)
      {
        auto kr = _PC.RC(i);
        auto& recipe = _PC.RC[kr];
        auto required = recipe.GetRequired();
        auto results = recipe.GetResult();
        auto& childrensRecipes = _childrensRecipes[i];

        childrensRecipes.resize(required.size());
        std::copy(required.begin(), required.end(), childrensRecipes.begin());
        
        for (auto item_counter: results)
        {
          TYPE_KEY ii = _PC.IC(item_counter.ItemId);
          auto& childrensItems = _childrensItems[ii];
          _childrensItems[_PC.IC(item_counter.ItemId)].push_back(kr);
        }
        _recipes.insert({kr, std::move(RecipeNode::Ptr(new RecipeNode(kr, _childrensRecipes[i], true)))});
      }
      //Indexator<KEY_RECIPE, RecipeNode::Ptr>::UpdateIndex();
    }
  }

  void FullItemTree::CloneTo(FullItemTree& tree) const
  {
    tree._items.clear();
    tree._recipes.clear();

    for (auto& it : _items)
    {
      tree._items.insert({ it.first, std::move(ItemNode::Ptr(new ItemNode(*it.second))) });
    }

    for (auto& it : _recipes)
    {
      tree._recipes.insert({ it.first, std::move(RecipeNode::Ptr(new RecipeNode(*it.second))) });
    }

    tree._childrensItems = _childrensItems;
    tree._childrensRecipes = _childrensRecipes;

    //tree.Indexator<KEY_ITEM, ItemNode::Ptr>::UpdateIndex();
    //tree.Indexator<KEY_RECIPE, RecipeNode::Ptr>::UpdateIndex();
  }

}

