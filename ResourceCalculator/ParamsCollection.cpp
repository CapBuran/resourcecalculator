#include "ParamsCollection.h"

int ResourceCalculator::ParamsCollection::ReadFromJson(const Json::Value & jsonPr)
{
  Icons.ReadFromJson(jsonPr["Icons"]);
  IC.ReadFromJson(jsonPr["Items"]);
  FC.ReadFromJson(jsonPr["Factorys"]);
  RC.ReadFromJson(jsonPr["Recipes"]);
  return 0;
}

int ResourceCalculator::ParamsCollection::WriteToJson(Json::Value & jsonPr) const
{
  Icons.WriteToJson(jsonPr["Icons"]);
  IC.WriteToJson(jsonPr["Items"]);
  FC.WriteToJson(jsonPr["Factorys"]);
  RC.WriteToJson(jsonPr["Recipes"]);
  return 0;
}
