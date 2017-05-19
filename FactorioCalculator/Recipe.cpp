#include "Recipe.h"

namespace FactorioCalculator{

  Recipe::Recipe(
    const std::string &Name, KEY_RECIPE Key, double Time, const RecipeParams &Params
  ):
    FactorioItem(Name), _Key(Key), _Time(Time), _Required(Params.Required), _Result(Params.Result)
  {
  }

  int Recipe::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Recipe::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

  Recipe::~Recipe()
  {
  }

  int RecipeParams::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int RecipeParams::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

}