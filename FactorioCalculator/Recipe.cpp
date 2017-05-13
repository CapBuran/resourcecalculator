#include "Recipe.h"

namespace FactorioCalculator {

  Recipe::Recipe(
    const std::string &Name, KEY_RECIPE Key, double Time,
    std::list<CountsItem> const &Required, const std::list<CountsItem> &Result
  ):
    FactorioItem(Name), _Key(Key), _Time(Time), _Required(Required), _Result(Result)
  {
  }

  Recipe::~Recipe()
  {
  }

}