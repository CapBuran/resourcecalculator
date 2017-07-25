#ifndef ItemH
#define ItemH

#include "Types.h"



namespace ResourceCalculator {
  
  class Item: public ItemBase {
  private:
    KEY_ITEM _Key;
    bool _IsALiquidOrGas;
  public:

    ~Item();
    
    DeclareAndDefinitionProperty(Key, KEY_ITEM)

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

}

#endif ItemH