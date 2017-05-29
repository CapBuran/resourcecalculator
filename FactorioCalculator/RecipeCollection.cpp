#include "RecipeCollection.h"

namespace FactorioCalculator{

  //int RecipeCollection::BuildListRecipe(KEY_ITEM Item, std::list<Recipe>& Result)
  //{

  //  for (auto &recipe : _Recipes) {
  //    RecipeParams RP = recipe.second.GetRecipeParams();
  //    for (auto &f : RP.Required) {
  //      std::list<Recipe> ResultOne;
  //      BuildListRecipe(f.ItemId, ResultOne);
  //    }
  //  }
  //  
  //  return 0;
  //}

  const std::map<KEY_RECIPE, RecipeResultTree> & RecipeResultTree::GetResult() const
  {
    return _Result;
  }


  RecipeResultTree RecipeCollection::BuildTreeRecipe(KEY_ITEM Item, int NestingResults) const
  {
    RecipeResultTree RetVal;
    RetVal._ItemKey = Item;
    if (NestingResults == 0) {
      return RetVal;
    }

    for (auto &recipe : _Recipes) {
      const RecipeParams RP = recipe.second.GetRecipeParams();
      for (const CountsItem &Result : RP.Result) {
        if (Result.ItemId == Item){
          for (const CountsItem &Required : RP.Required) {
            RecipeResultTree AddVal = BuildTreeRecipe(Required.ItemId, NestingResults - 1);
            AddVal._ItemKey = Required.ItemId;
            RetVal._Result[RP.Key] = AddVal;
          }
        }

      }
    }

    return RetVal;
  }

  void RecipeCollection::ADD(const Recipe &recipe)
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
      ADD(ToAdd);
    }
    return 0;
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
