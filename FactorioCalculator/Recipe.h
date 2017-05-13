
#include "Types.h"

namespace FactorioCalculator{

  struct CountsItem{
    KEY_ITEM ItemId;
    double   Count;
  };

  class Recipe: public FactorioItem{
  private:
    const double     _Time;
    const KEY_RECIPE _Key;
    const std::list<CountsItem> _Required;
    const std::list<CountsItem> _Result;
    std::list<KEY_FACTORY> _FactoryAllowed;
  public:
    Recipe(std::string const &Name, KEY_RECIPE Key, double Time, std::list<CountsItem> const &Required, const std::list<CountsItem> &Result);
    ~Recipe();
  };

}
