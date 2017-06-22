#ifndef FACTORIES_TYPES
#define FACTORIES_TYPES

#include <string>
#include <list>
#include <map>
#include "json/json.h"

namespace ResourceCalculator {

  enum class KEY_RECIPE : unsigned int
  {
    ID_RECIPE_NoFindRecipe,
    ID_RECIPE_FindRecipeROOT,
    ID_RECIPE_Iron_Plate,
    ID_RECIPE_Cuprum_Plate,
    ID_RECIPE_Sherst,
    ID_RECIPE_Paket1,
    ID_RECIPE_Paket1_2
  };

  enum class KEY_ITEM : unsigned int
  {
    ID_ITEM_Iron_Plate,
    ID_ITEM_Cuprum_Plate,
    ID_ITEM_Iron_Ruda,
    ID_ITEM_Cuprum_Ruda,
    ID_ITEM_Sherst,
    ID_ITEM_Paket1
  };

  enum class KEY_FACTORY : unsigned int
  {
    ID_FACTORY_Assembly1,
    ID_FACTORY_Assembly2,
    ID_FACTORY_Assembly3,
    ID_FACTORY_PechKamenaya,
    ID_FACTORY_PechStalnaya
  };

  enum class KEY_MODULE : unsigned int
  {
    ID_Productivity_1,
    ID_Productivity_2,
    ID_Productivity_3,
    ID_Efficiency_1,
    ID_Efficiency_2,
    ID_Efficiency_3,
    ID_Speed_1,
    ID_Speed_2,
    ID_Speed_3
  };

  //typedef unsigned int KEY_RECIPE;
  //typedef unsigned int KEY_ITEM;
  //typedef unsigned int KEY_FACTORY;

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
