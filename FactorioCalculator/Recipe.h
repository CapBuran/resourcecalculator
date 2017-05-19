
#include "Types.h"

namespace FactorioCalculator{

  struct CountsItem{
    KEY_ITEM ItemId;
    double   Count;
  };

  struct RecipeParams: public Jsonable {
    std::list<CountsItem> 
      Required,
      Result;
    std::list<KEY_FACTORY> 
      FactoryAllowed;
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

  class Recipe: public FactorioItem, public Jsonable {
  private:
    const double     _Time;
    const KEY_RECIPE _Key;
    const std::list<CountsItem> _Required;
    const std::list<CountsItem> _Result;
    std::list<KEY_FACTORY> _FactoryAllowed;
  public:
    Recipe(std::string const &Name, KEY_RECIPE Key, double Time, const RecipeParams &Params);

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    ~Recipe();
  };

}
