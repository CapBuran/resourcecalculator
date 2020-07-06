#ifndef FACTORIES_TYPES
#define FACTORIES_TYPES

#include <stdint.h>
#include <string>
#include <set>
#include <list>
#include <map>
#include <json/json.h>

namespace ResourceCalculator
{

  typedef Json::LargestUInt KEY_TO_Json;

  //typedef KEY_TO_Json TYPE_KEY;
  typedef uint64_t TYPE_KEY;

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
    ID_RECIPE_science_pack_2 = 13,
    ID_RECIPE_heavy_oil_cracking = 14,
    ID_RECIPE_light_oil_cracking = 15,
    ID_RECIPE_basic_oil_processing = 16,
    ID_RECIPE_coal_liquefaction = 17,
    ID_RECIPE_advanced_oil_processing = 18,
    ID_RECIPE_plastic_bar = 19,
    ID_RECIPE_advanced_circuit = 20
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
    ID_ITEM_science_pack_2,
    ID_ITEM_steam,
    ID_ITEM_light_oil,
    ID_ITEM_heavy_oil,
    ID_ITEM_petroleum_gas,
    ID_ITEM_coal,
    ID_ITEM_plastic_bar,
    ID_ITEM_advanced_circuit
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

  enum class KEY_MODULE : TYPE_KEY {
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

  enum class KEY_TRANSPORT_BELT : TYPE_KEY {
    Unknown,
    ID_TransportBelt,
    ID_FastTransportBelt,
    ID_ExpressTransportBelt
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

#define DefinitionPropertyReadOnly(Name, Type, ClassName) \
  Type ClassName::Get##Name() const { return _##Name; }

#define DeclareAndDefinitionPropertyReadOnly(Name, Type) \
  inline Type Get##Name() const { return _##Name; }

#define DeclareAndDefinitionRefReadOnly(Name, Type) \
  inline const Type& Get##Name() const { return _##Name; }

#define DeclarePropertyReadOnly(Name, Type) \
  Type Get##Name() const;

  class ItemBase: public Jsonable {
  protected:
    std::string _Name;
    std::string _IconKey;
    inline virtual ~ItemBase()
    {
    }

  public:

    DeclareAndDefinitionProperty( Name, std::string )
    DeclareAndDefinitionProperty( IconKey, std::string )

    virtual int ReadFromJson( const Json::Value & jsonPr ) override;
    virtual int WriteToJson( Json::Value & jsonPr ) const override;

  };

}
#endif // !FACTORIES_TYPES
