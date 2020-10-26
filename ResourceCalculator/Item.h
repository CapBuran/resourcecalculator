#ifndef ItemH
#define ItemH

#include "Types.h"

namespace ResourceCalculator {
  
  class Item: public ItemBase<KEY_ITEM>
  {
  private:
    bool _IsALiquidOrGas;
    double _MiningHardness;
  public:
    Item();
    DeclareAndDefinitionProperty(IsALiquidOrGas, bool)
    DeclareAndDefinitionProperty(MiningHardness, double)
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

}

#endif// ItemH
