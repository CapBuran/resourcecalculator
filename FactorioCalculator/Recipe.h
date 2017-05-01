
#include "Types.h"

namespace FactorioCalculator{
  class Recipe: public FactorioItem{
  private:
    FactoryType Factory;
    double Time;
    std::list<KEY_RECIPE> _Required;
    std::list<KEY_RECIPE> _Result;
  public:
    void SetRequired(std::list<KEY_RECIPE> const &Required);
    void SetResult(std::list<KEY_RECIPE> const &Result);
    Recipe(std::string const &Name);
    ~Recipe();
  };

}
