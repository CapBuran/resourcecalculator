#ifndef FACTORIES_TYPES
#define FACTORIES_TYPES

#include <string>
#include <set>
#include <list>
#include <map>
#include "json/json.h"

namespace ResourceCalculator
{

  typedef Json::LargestUInt KEY_TO_Json;

  typedef KEY_TO_Json TYPE_KEY;

  enum class KEY_RECIPE: TYPE_KEY {
    ID_RECIPE_NoFindRecipe = 0,
    ID_RECIPE_PreviouslyProduced = 1,
    ID_RECIPE_FindRecipeROOT = 2,
    ID_RECIPE_Iron_Plate = 3,
    ID_RECIPE_Cuprum_Plate = 4,
    ID_RECIPE_Sherst = 5,
    ID_RECIPE_science_pack_1 = 6,
    ID_RECIPE_electronic_circuit = 7,
    ID_RECIPE_copper_cable = 10,
    ID_RECIPE_transport_belt = 11,
    ID_RECIPE_inserter = 12,
    ID_RECIPE_science_pack_2 = 13
  };

  enum class KEY_ITEM: TYPE_KEY {
    ID_ITEM_NoFind_Item,
    ID_ITEM_Iron_Plate,
    ID_ITEM_Cuprum_Plate,
    ID_ITEM_Iron_Ruda,
    ID_ITEM_Cuprum_Ruda,
    ID_ITEM_Sherst,
    ID_ITEM_science_pack_1,
    ID_ITEM_Oil,
    ID_ITEM_Water,
    ID_ITEM_electronic_circuit,
    ID_ITEM_fast_inserter,
    ID_ITEM_copper_cable,
    ID_ITEM_fast_transport_belt,
    ID_ITEM_transport_belt,
    ID_ITEM_inserter,
    ID_ITEM_science_pack_2
  };

  enum class KEY_TYPE_FACTORY: TYPE_KEY {
    Unknown,
    Assembly,
    ChemicalPlant,
    Refinery,
    Furnace,
    ElectrolysisPlant
  };

  enum class KEY_FACTORY: TYPE_KEY {
    ID_ITEM_NoFind_Factory,
    ID_FACTORY_Assembly1,
    ID_FACTORY_Assembly2,
    ID_FACTORY_Assembly3,
    ID_FACTORY_PechKamenaya,
    ID_FACTORY_PechStalnaya,
    ID_FACTORY_PechElectro1
  };

  enum class KEY_MODULE: TYPE_KEY {
    ID_CleanSlot,
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

#define DeclareProperty(Name, Type) \
  Type Get##Name() const; \
  void Set##Name(Type);

#define DeclareAndDefinitionProperty(Name, Type) \
  inline Type Get##Name() const { return _##Name; } \
  inline void Set##Name(Type Name) { _##Name = Name; }

#define DefinitionProperty(Name, Type, ClassName) \
  Type ClassName::Get##Name() const { return _##Name; } \
  void ClassName::Set##Name(Type Name) { _##Name = Name; }

#define DeclareAndDefinitionPropertyReadOnly(Name, Type) \
  inline Type Get##Name() const { return _##Name; }

#define DeclareAndDefinitionRefReadOnly(Name, Type) \
  inline const Type& Get##Name() const { return _##Name; }

#define DeclarePropertyReadOnly(Name, Type) \
  Type Get##Name() const;

  class Jsonable {
  protected:
    inline bool JsonCheckIsEmptyField( const Json::Value &obj_json, std::string obj_path = "", bool critical = false ) const
    {
      if ( obj_json.empty() ) {
        if ( critical )
          throw ( "\"" + obj_path + "\" field is empty.\n" );
        return true;
      }
      return false;
    }
  public:
    virtual int ReadFromJson( const Json::Value & jsonPr ) = 0;
    virtual int WriteToJson( Json::Value & jsonPr ) const = 0;
  };

  class ItemBase: public Jsonable {
  protected:
    std::string _Name;
    std::string _IconPath;

    inline virtual ~ItemBase()
    {
    }

  public:

    DeclareAndDefinitionProperty( Name, std::string )
    DeclareAndDefinitionProperty( IconPath, std::string )

    virtual int ReadFromJson( const Json::Value & jsonPr ) override;
    virtual int WriteToJson( Json::Value & jsonPr ) const override;

  };

}
#endif // !FACTORIES_TYPES
