#pragma once

#include <cstdint>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <memory>
#include  <functional>

#include <json/json.h>

namespace ResourceCalculator
{

  typedef Json::LargestUInt KEY_TO_Json;

  //typedef KEY_TO_Json TYPE_KEY;
  typedef uint64_t TYPE_KEY;

  enum class KEY_RECIPE: TYPE_KEY {
    ID_RECIPE_NoFindRecipe = 0,
//    ID_RECIPE_PreviouslyProduced = 10000001,
//    ID_RECIPE_FindRecipeROOT = 1000000,
    ID_RECIPE_Iron_Plate = 1,
    ID_RECIPE_Cuprum_Plate = 2,
    ID_RECIPE_Sherst = 3,
    ID_RECIPE_science_pack_1 = 4,
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
    ID_ITEM_science_pack_1,
    ID_ITEM_Iron_Plate,
    ID_ITEM_Cuprum_Plate,
    ID_ITEM_Sherst,
    ID_ITEM_Iron_Ruda,
    ID_ITEM_Cuprum_Ruda,
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

  template<typename EnumKey, class T>
  class Indexator
  {
  public:

    Indexator(const std::map<EnumKey, T>& items)
      : _LastGenGen(0)
      , _Size(0)
      , _Items(items)
    {}

    virtual ~Indexator() {}

    EnumKey GetEnumKeyByKey(TYPE_KEY key) const
    {
      auto f = _Index2.find(key);
      if (f != _Index2.end())
      {
        return f->second;
      }

      return static_cast<EnumKey>(0);
    }

    TYPE_KEY GetKeyByEnumKey(EnumKey key) const
    {
      auto f = _Index1.find(key);
      if (f != _Index1.end())
      {
        return f->second;
      }

      return -1;
    }

    EnumKey GetUniqueEnumKey()
    {
      TYPE_KEY retval = _LastGenGen + 1;
      if (_Items.size() > 0) {
        while (_Items.find(static_cast<EnumKey>(retval)) != _Items.end()) {
          retval++;
        }
      }
      _LastGenGen = retval;
      return static_cast<EnumKey>(retval);
    }

    TYPE_KEY Size() const
    {
      return _Size;
    }

  protected:

    void UpdateIndex()
    {
      _Index1.clear();
      _Index2.clear();
      TYPE_KEY k = 0;
      for (const auto& pair : _Items)
      {
        TYPE_KEY keyInt = static_cast<TYPE_KEY>(pair.first);
        if (keyInt == 0) continue;
        _Index1[pair.first] = k;
        _Index2[k] = pair.first;
        k++;
        if (keyInt > _LastGenGen) _LastGenGen = keyInt + 1;
      }
      _Size = k;
    };

    void CopyIndexes(Indexator& retVal) const
    {
      retVal._Index1 = _Index1;
      retVal._Index2 = _Index2;
      retVal._LastGenGen = _LastGenGen;
      retVal._Size = _Size;
    }

  private:

    TYPE_KEY _LastGenGen;
    TYPE_KEY _Size;

    std::map<EnumKey, TYPE_KEY> _Index1;
    std::map<TYPE_KEY, EnumKey> _Index2;
    const std::map<EnumKey, T>& _Items;
  };

}
