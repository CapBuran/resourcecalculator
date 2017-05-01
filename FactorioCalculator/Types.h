#include <string>
#include <list>

namespace FactorioCalculator {
  typedef int KEY_RECIPE;

  enum class FactoryType{
    Chemical,
    Assembly,
    Refinery
  };

  class FactorioItem{
  protected:
    std::string _Name;
    FactorioItem(const std::string &Name);
    virtual ~FactorioItem();

  private:

  };


}
