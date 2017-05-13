#include <string>

#include "Types.h"

namespace FactorioCalculator {
  class Factory : public FactorioItem{
  private:
    double _Speed;
    double _Power;
    
    KEY_FACTORY _Key;

  public:

    Factory(std::string const &Name, double Speed, double Power = 1.0);
    ~Factory();



  };

}
