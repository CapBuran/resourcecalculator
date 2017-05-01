#include "Recipe.h"

namespace FactorioCalculator {
  void Recipe::SetRequired(std::list<KEY_RECIPE> const & Required)
  {
    _Required = Required;
  }

  void Recipe::SetResult(std::list<KEY_RECIPE> const & Result)
  {
    _Result = Result;
  }

  Recipe::Recipe(const std::string &Name):
    FactorioItem(Name)
  {
  }

  Recipe::~Recipe()
  {
  }

}