#include "ParamsCollection.h"

namespace ResourceCalculator
{
ParamsCollection::ParamsCollection() :
  IC(RC), PCC(*this)
{
}

int ResourceCalculator::ParamsCollection::ReadFromJson(const Json::Value & jsonPr)
{
  Icons.ReadFromJson(jsonPr["Icons"]);
  IC.ReadFromJson(jsonPr["Items"]);
  FC.ReadFromJson(jsonPr["Factorys"]);
  FC.ReadFromJsonFactoryTypes(jsonPr["FactoryTypes"]);
  RC.ReadFromJson(jsonPr["Recipes"]);
  MC.ReadFromJson(jsonPr["Modules"]);
  PCC.ReadFromJson(jsonPr["ProductionChains"]);
  TC.ReadFromJson(jsonPr["TransportBelts"]);
  return 0;
}

int ResourceCalculator::ParamsCollection::WriteToJson(Json::Value & jsonPr) const
{
  Icons.WriteToJson(jsonPr["Icons"]);
  IC.WriteToJson(jsonPr["Items"]);
  FC.WriteToJson(jsonPr["Factorys"]);
  FC.WriteToJsonFactoryTypes(jsonPr["FactoryTypes"]);
  RC.WriteToJson(jsonPr["Recipes"]);
  MC.WriteToJson(jsonPr["Modules"]);
  PCC.WriteToJson(jsonPr["ProductionChains"]);
  TC.WriteToJson(jsonPr["TransportBelts"]);
  return 0;
}
}
