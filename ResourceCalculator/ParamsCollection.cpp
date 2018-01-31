#include "ParamsCollection.h"

namespace ResourceCalculator
{
  ParamsCollection::ParamsCollection():
    IC( RC )
  {
  }

  int ResourceCalculator::ParamsCollection::ReadFromJson( const Json::Value & jsonPr )
  {
    Icons.ReadFromJson( jsonPr["Icons"] );
    IC.ReadFromJson( jsonPr["Items"] );
    FC.ReadFromJson( jsonPr["Factorys"] );
    FC.ReadFromJsonFactoryTypes( jsonPr["FactoryTypes"] );
    RC.ReadFromJson( jsonPr["Recipes"] );
    MC.ReadFromJson( jsonPr["Modules"] );
    return 0;
  }

  int ResourceCalculator::ParamsCollection::WriteToJson( Json::Value & jsonPr ) const
  {
    Icons.WriteToJson( jsonPr["Icons"] );
    IC.WriteToJson( jsonPr["Items"] );
    FC.WriteToJson( jsonPr["Factorys"] );
    FC.WriteToJsonFactoryTypes( jsonPr["FactoryTypes"] );
    RC.WriteToJson( jsonPr["Recipes"] );
    MC.WriteToJson( jsonPr["Modules"] );
    return 0;
  }
}
