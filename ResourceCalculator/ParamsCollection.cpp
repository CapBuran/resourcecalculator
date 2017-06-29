#include "ParamsCollection.h"

int ResourceCalculator::ParamsCollection::ReadFromJson(const Json::Value & jsonPr)
{
  IC.ReadFromJson(jsonPr["Items"]);
  FC.ReadFromJson(jsonPr["Factorys"]);
  RC.ReadFromJson(jsonPr["Recipes"]);
  return 0;
}

int ResourceCalculator::ParamsCollection::WriteToJson(Json::Value & jsonPr) const
{
  IC.WriteToJson(jsonPr["Items"]);
  FC.WriteToJson(jsonPr["Factorys"]);
  RC.WriteToJson(jsonPr["Recipes"]);
  return 0;
}
