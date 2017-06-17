#include "RecipeCollection.h"

namespace FactorioCalculator{

  const std::map<KEY_RECIPE, RecipeResultTree> & ItemResultTree::GetResult() const
  {
    return _Result;
  }

  const std::map<KEY_ITEM, ItemResultTree> & RecipeResultTree::GetResult() const
  {
    return _Result;
  }

  ItemResultTree RecipeCollection::BuildTree(KEY_ITEM ItemID, int NestingResults) const
  {
    ItemResultTree RetVal;
    if (NestingResults == 0) return RetVal;
      for (auto &it : _Recipes) {
      RecipeParams RP = it.second.GetRecipeParams();
      for (auto it : RP.Result) {
        if (it.ItemId == ItemID) {
          RetVal._Result[RP.Key] = BuildTree(RP.Key, NestingResults - 1);
        }
      }
    }
    return RetVal;
  }

  RecipeResultTree RecipeCollection::BuildTree(KEY_RECIPE RecipeID, int NestingResults) const
  {
    RecipeResultTree RetVal;
    if (NestingResults == 0) return RetVal;
    std::map<KEY_RECIPE, Recipe>::const_iterator RP_FIND = _Recipes.find(RecipeID);
    if (RP_FIND != _Recipes.end()){
      RecipeParams RP = RP_FIND->second.GetRecipeParams();
      for (auto &it : RP.Required) {
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

    RecipeParams RP = Recipe->second.GetRecipeParams();

    ResultRecipe.push_front(RP.Key);

    for (auto &Item : RP.Required ){
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
      RecipeParams RP;
      RP.ReadFromJson(it);
      KEY_TO_Json Key  = it["Key"].asInt64();
      std::string Name = it["Name"]["ru"].asString();
      Recipe ToAdd(Name, RP);
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
