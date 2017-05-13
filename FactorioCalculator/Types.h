#ifndef FACTORIES_TYPES
#define FACTORIES_TYPES

#include <string>
#include <list>

namespace FactorioCalculator {

  enum class KEY_RECIPE
  {
    ID_Iron_Plate,
    ID_Cuprum_Plate,
    ID_Sherst,
    ID_Paket1
  };

  enum class KEY_ITEM
  {
    ID_Iron_Plate,
    ID_Cuprum_Plate,
    ID_Sherst,
    ID_Paket1
  };

  enum class KEY_FACTORY
  {
    ID_Iron_Plate,
    ID_Cuprum_Plate,
    ID_Sherst,
    ID_Paket1_Plate
  };

  //typedef int KEY_RECIPE;
  //typedef int KEY_ITEM;
  //typedef int KEY_FACTORY;

  class FactorioItem{
  protected:
    std::string _Name;
    FactorioItem(const std::string &Name);
    virtual ~FactorioItem();

  public:

    inline const std::string& GetName() const{
      return _Name;

    }

  };


}
#endif // !FACTORIES_TYPES
