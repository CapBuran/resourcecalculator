#include "Types.h"

namespace FactorioCalculator  {

  class Item: public FactorioItem{
  private:
    const KEY_ITEM _Key;
  public:
    Item(std::string const &Name, KEY_ITEM Key);
    ~Item();
  };

}
