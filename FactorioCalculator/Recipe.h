#ifndef RecipeH
#define RecipeH

#include "Types.h"

namespace FactorioCalculator{

  struct CountsItem{
    KEY_ITEM ItemId;
    double   Count;
  };

  struct RecipeParams: public Jsonable {
    KEY_TO_Json Key;
    double      Time;
    std::list<CountsItem> 
      Required,
      Result;
    std::list<KEY_FACTORY> 
      FactoryAllowed;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };


  class Recipe: public FactorioItem, public Jsonable{
  private:
    const double      _Time;
    const KEY_TO_Json _Key;
    const std::list<CountsItem> _Required;
    const std::list<CountsItem> _Result;
    std::list<KEY_FACTORY> _FactoryAllowed;
  public:
    Recipe(std::string const &Name, const RecipeParams &Params);
    RecipeParams GetRecipeParams() const;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
    ~Recipe();
  };

}
#endif//RecipeH
