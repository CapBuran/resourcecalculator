#include "RecipeCollection.h"

namespace ResourceCalculator {

  const std::map<KEY_RECIPE, RecipeResultTree> & ItemResultTree::GetResult() const
  {
    return _Result;
  }

  KEY_ITEM ItemResultTree::GetItemKey() const
  {
    return _ItemKey;
  }

  const std::map<KEY_ITEM, ItemResultTree> & RecipeResultTree::GetResult() const
  {
    return _Result;
  }

  KEY_RECIPE RecipeResultTree::GetRecipeKey() const
  {
    return _RecipeKey;
  }

  ItemResultTree RecipeCollection::BuildTree(KEY_ITEM ItemID, int NestingResults) const
  {
    ItemResultTree RetVal;
    RetVal._ItemKey = ItemID;
    if (NestingResults == 0) return RetVal;
    for (auto &it : _Recipes) {
      for (auto it2 : it.second.GetResult()) {
        if (it2.ItemId == ItemID) {
          RetVal._Result[it.second.GetKey()] = BuildTree(it.second.GetKey(), NestingResults - 1);
        }
      }
    }
    return RetVal;
  }


  void RecipeCollection::Travelling(const RecipeResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
    std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const
  {
    const KEY_RECIPE FindRecipeKey = Tree.GetRecipeKey();

    const std::map<KEY_ITEM, ItemResultTree> & ResultTree = Tree.GetResult();

    for (auto &it : ResultTree) {
      Travelling(it.second, Ansfer, ResultRecipes, ResultItems);
    }

    ResultRecipes.remove(FindRecipeKey);
    ResultRecipes.push_back(FindRecipeKey);

  }


  void RecipeCollection::Travelling(const ItemResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer, 
    std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const
  {
    const KEY_ITEM FindItemKey = Tree.GetItemKey();
    const std::map<KEY_RECIPE, RecipeResultTree> &ResultTree = Tree.GetResult();
    if (ResultTree.size() > 0) {
      auto FindAnsfer = Ansfer.find(FindItemKey);
      if (FindAnsfer != Ansfer.end()) {
        KEY_RECIPE RecipeKeyAnsfer = FindAnsfer->second;
        if (RecipeKeyAnsfer != KEY_RECIPE::ID_RECIPE_PreviouslyProduced) {
          auto FindRecipeTree = ResultTree.find(RecipeKeyAnsfer);
          if (FindRecipeTree != ResultTree.end()) {
            Travelling(FindRecipeTree->second, Ansfer, ResultRecipes, ResultItems);
          }
        }
      } else {
        if (ResultTree.begin()->second.GetRecipeKey() != KEY_RECIPE::ID_RECIPE_PreviouslyProduced) {
          Travelling(ResultTree.begin()->second, Ansfer, ResultRecipes, ResultItems);
        }
      }
    }
    ResultItems.remove(FindItemKey);
    ResultItems.push_back(FindItemKey);
  }



  RecipeResultTree RecipeCollection::BuildTree(KEY_RECIPE RecipeID, int NestingResults) const
  {
    RecipeResultTree RetVal;
    RetVal._RecipeKey = RecipeID;
    if (NestingResults == 0) return RetVal;
    std::map<KEY_RECIPE, Recipe>::const_iterator RP_FIND = _Recipes.find(RecipeID);
    if (RP_FIND != _Recipes.end()){
      for (auto &it : RP_FIND->second.GetRequired()) {
        RetVal._Result[it.ItemId] = BuildTree(it.ItemId, NestingResults - 1);
      }
    }
    return RetVal;
  }

  void RecipeCollection::Build(KEY_ITEM ItemID, const std::map<KEY_ITEM, KEY_RECIPE> SelectRecipe,
                               std::list<KEY_RECIPE> &ResultRecipe, std::set<KEY_ITEM> &ResultItem)
  {
    std::map<KEY_ITEM, KEY_RECIPE>::const_iterator SelectRecipeFind = SelectRecipe.find(ItemID);

    if (SelectRecipeFind == SelectRecipe.end() ) {
      return;
    }

    KEY_RECIPE KR = SelectRecipeFind->second;

    std::map<KEY_RECIPE, Recipe >::const_iterator  Recipe = _Recipes.find(KR);

    if (Recipe == _Recipes.end()) {
      return;
    }

    ResultRecipe.push_front(Recipe->second.GetKey());

    for (auto &Item : Recipe->second.GetRequired() ){
      ResultItem.insert(Item.ItemId);
      Build(Item.ItemId, SelectRecipe, ResultRecipe, ResultItem);
    }

  }

  void RecipeCollection::Add(const Recipe &recipe)
  {
    _Recipes[recipe.GetKey()] = recipe;
  }

  int RecipeCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto it : jsonPr) {
      Recipe ToAdd;
      ToAdd.ReadFromJson(it);
      Add(ToAdd);
    }
    return 0;
  }

  const std::map<KEY_RECIPE, Recipe> &RecipeCollection::GetData() const
  {
    return _Recipes;
  }

  Recipe *RecipeCollection::GetRecipeForEdit(KEY_RECIPE KeyRecipe) {
    std::map<KEY_RECIPE, Recipe>::iterator it = _Recipes.find(KeyRecipe);
    if (it == _Recipes.end()) {
      return nullptr;
    }
    return &it->second;
  }

  KEY_RECIPE RecipeCollection::GetUniqueRecipeKey() const
  {
    unsigned int retval = 0;
    if (_Recipes.size() > 0) {
      while (_Recipes.find(static_cast<KEY_RECIPE>(retval)) != _Recipes.end()){
        retval++;
      }
    }
    return static_cast<KEY_RECIPE>(retval);
  }

  void RecipeCollection::Delete(KEY_RECIPE KeyRecipe) {
    if (_Recipes.find(KeyRecipe) != _Recipes.end()) {
      _Recipes.erase(KeyRecipe);
    }
  }

  int RecipeCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Recipes){
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }
}
