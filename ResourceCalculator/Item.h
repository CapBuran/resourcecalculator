#ifndef ItemH
#define ItemH

#include "Types.h"

namespace ResourceCalculator {
  
  class Item: public ItemBase {
  private:
    KEY_ITEM _Key;
    bool _IsALiquidOrGas;
    double _MiningHardness;
  public:
    Item();
    DeclareAndDefinitionProperty(Key, KEY_ITEM)
    DeclareAndDefinitionProperty(IsALiquidOrGas, bool)
    DeclareAndDefinitionProperty(MiningHardness, double)
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

}

#endif// ItemH
