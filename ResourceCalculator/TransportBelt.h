#ifndef TransportBeltH
#define TransportBeltH

#include "Types.h"

namespace ResourceCalculator {
  
  class TransportBelt: public ItemBase<KEY_TRANSPORT_BELT>
  {
  private:
    double _MaxDensity;
    double _Speed;
  public:
    TransportBelt();
    DeclareAndDefinitionProperty(MaxDensity, double)
    DeclareAndDefinitionProperty(Speed, double)
    double GetMaxThroughput() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

}

#endif// TransportBeltH
