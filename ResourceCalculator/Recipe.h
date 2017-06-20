#ifndef RecipeH
#define RecipeH

#include "Types.h"

namespace ResourceCalculator {

  struct CountsItem{
    KEY_ITEM ItemId;
    double   Count;
  };

  struct RecipeParams: public Jsonable {
    KEY_RECIPE Key;
    double     Time;
    std::list<CountsItem> 
      Required,
      Result;
    std::list<KEY_FACTORY> 
      FactoryAllowed;
    KEY_FACTORY CurrentFactory;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

  class Recipe: public FactorioItem, public Jsonable{
  private:
    double      _Time;
    KEY_RECIPE  _Key;
    std::list<CountsItem> _Required;
    std::list<CountsItem> _Result;
    std::list<KEY_FACTORY> _FactoryAllowed;
    KEY_FACTORY _CurrentFactory;
    Recipe();
    Recipe(const Recipe &recipe);

  public:
    Recipe(std::string const &Name, const RecipeParams &Params);
    ~Recipe();

    RecipeParams GetRecipeParams() const;
    KEY_RECIPE GetKey() const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    friend std::pair<const KEY_RECIPE, Recipe>;
  };

}
#endif//RecipeH
