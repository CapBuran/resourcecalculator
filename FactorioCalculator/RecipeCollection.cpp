#include "RecipeCollection.h"

namespace FactorioCalculator{

  RecipeCollection::RecipeCollection()
  {
  }

  RecipeCollection::~RecipeCollection()
  {
  }

  int RecipeCollection::BuildTableRecipe(KEY_ITEM Item, std::list<std::list<CountsItem>>& Result)
  {
    return 0;
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
