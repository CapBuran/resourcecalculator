#pragma once
#include <Types.h>

namespace ResourceCalculator
{
  class TransportBelt: public ItemBase<KEY_TRANSPORT_BELT>
  {
  public:
    double MaxDensity;
    double Speed;
    TransportBelt();
    double GetMaxThroughput() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

  using TransportBeltCollection = Indexator<KEY_TRANSPORT_BELT, TransportBelt>;
}
