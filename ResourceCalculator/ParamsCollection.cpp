#include <ParamsCollection.h>

namespace ResourceCalculator
{
  ParamsCollection::ParamsCollection()
    : FTC(FC)
    , IC(RC)
  {}

  int ResourceCalculator::ParamsCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    Icons.ReadFromJson(jsonPr["Icons"]);
    IC.ReadFromJson(jsonPr["Items"]);
    FC.ReadFromJson(jsonPr["Factorys"]);
    RC.ReadFromJson(jsonPr["Recipes"]);
    MC.ReadFromJson(jsonPr["Modules"]);
    TC.ReadFromJson(jsonPr["TransportBelts"]);
    FTC.ReadFromJson(jsonPr["FactoryTypes"]);
    return 0;
  }

  int ResourceCalculator::ParamsCollection::WriteToJson(Json::Value & jsonPr) const
  {
    Icons.WriteToJson(jsonPr["Icons"]);
    IC.WriteToJson(jsonPr["Items"]);
    FC.WriteToJson(jsonPr["Factorys"]);
    RC.WriteToJson(jsonPr["Recipes"]);
    MC.WriteToJson(jsonPr["Modules"]);
    TC.WriteToJson(jsonPr["TransportBelts"]);
    FTC.WriteToJson(jsonPr["FactoryTypes"]);
    return 0;
  }
}
