#ifndef FACTORIES_TYPES
#define FACTORIES_TYPES

#include <string>
#include <list>
#include <map>
#include "json/json.h"

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

  typedef Json::LargestUInt KEY_TO_Json;

  class Jsonable {
  protected:
    inline bool JsonCheckIsEmptyField(const Json::Value &obj_json, std::string obj_path = "", bool critical = false) const
    {
      if (obj_json.empty()) {
        if (critical)
          throw ("\"" + obj_path + "\" field is empty.\n");
        return true;
      }
      return false;
    }
  public:
    virtual int ReadFromJson(const Json::Value & jsonPr) = 0;
    virtual int WriteToJson(Json::Value & jsonPr) const = 0;
  };

  class FactorioItem {
  protected:
    std::string _Name;
    FactorioItem(const std::string &Name);
    virtual ~FactorioItem();

  public:

    inline const std::string& GetName() const
    {
      return _Name;
    }

  };

}
#endif // !FACTORIES_TYPES
