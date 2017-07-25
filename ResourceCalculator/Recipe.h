#ifndef RecipeH
#define RecipeH

#include "Types.h"

namespace ResourceCalculator {

  struct CountsItem{
    KEY_ITEM ItemId;
    double   Count;
  };

  class Recipe: public ItemBase{
  private:
    double      _Time;
    KEY_RECIPE  _Key;
    std::list<CountsItem> _Required;
    std::list<CountsItem> _Result;
    KEY_TYPE_FACTORY _TypeFactory;

  public:

    Recipe();

    DeclareAndDefinitionProperty(Key,         KEY_RECIPE)
    DeclareAndDefinitionProperty(Time,        double)
    DeclareAndDefinitionProperty(Required,    std::list<CountsItem>)
    DeclareAndDefinitionProperty(Result,      std::list<CountsItem>)
    DeclareAndDefinitionProperty(TypeFactory, KEY_TYPE_FACTORY)

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

  };

}
#endif//RecipeH
