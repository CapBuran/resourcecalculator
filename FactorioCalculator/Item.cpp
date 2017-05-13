#include "Item.h"

namespace FactorioCalculator {

  Item::Item(const std::string &Name, KEY_ITEM Key):
    FactorioItem(Name), _Key(Key)
  {
  }

  Item::~Item()
  {
  }

}