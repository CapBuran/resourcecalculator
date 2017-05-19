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

  void RecipeCollection::ADD(const Recipe &)
  {
  }

  bool JsonCheckIsEmptyField(const Json::Value &obj_json, std::string obj_path = "", bool critical = false)
  {
    if (obj_json.empty()) {
      if (critical)
        throw ("\"" + obj_path + "\" field is empty.\n");
      return true;
    }
    return false;
  }


  int RecipeCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    if (!JsonCheckIsEmptyField(jsonPr["icntl"]))
    {
      Json::ArrayIndex size = jsonPr.size();
      for (Json::ArrayIndex i = 0; i < size; i++) {
        const Json::Value & jsonPr = jsonPr["Recipe"][std::to_string(i)];
        if (!JsonCheckIsEmptyField(jsonPr)) {
          RecipeParams Params;
          //Params.
          //Recipe ToADDVal(;
        }
        if (!JsonCheckIsEmptyField(jsonPr["Recipe"][std::to_string(i)]))
          ;// icntl[i] = jsonPr["iparm"][std::to_string(i)].asInt();
      }
    }
    return 0;
  }

  int RecipeCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Recipes){
      Json::Value newVal;
      it.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

}
