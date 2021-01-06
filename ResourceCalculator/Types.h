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

  const static TYPE_KEY NoFoundKey = static_cast<TYPE_KEY>(-1);

  enum class KEY_RECIPE: TYPE_KEY {
    ID_RECIPE_NoFindRecipe = 0,
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
    ID_ITEM_Iron_Ruda,
    ID_ITEM_Cuprum_Ruda,
    ID_ITEM_Iron_Plate,
    ID_ITEM_Cuprum_Plate,
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
    AssemblyFluid,
    ChemicalPlant,
    Refinery,
    Furnace
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

  template<typename EnumKey>
  class ItemBase : public Jsonable {
  protected:
    std::string _Name;
    std::string _IconKey;
    EnumKey _Key = static_cast<EnumKey>(NoFoundKey);
  public:
    virtual ~ItemBase() {}
    operator bool() const { return _Key != static_cast<EnumKey>(0); }
    DeclareAndDefinitionProperty( Key, EnumKey)
    DeclareAndDefinitionProperty( Name, std::string )
    DeclareAndDefinitionProperty( IconKey, std::string )
    int ItemBase::ReadFromJson(const Json::Value& jsonPr)
    {
      _Key = static_cast<EnumKey>(jsonPr["Key"].asInt64());
      _Name = jsonPr["Name"]["ru"].asString();
      _IconKey = jsonPr["IconPath"].asString();
      return 0;
    }
    int ItemBase::WriteToJson(Json::Value& jsonPr) const
    {
      jsonPr["Key"] = static_cast<Json::Value::Int64>(_Key);
      jsonPr["Name"]["ru"] = _Name;
      jsonPr["Name"]["en"] = "English lang";
      jsonPr["IconPath"] = _IconKey;
      return 0;
    }
  };

  template<typename EnumKey, class T>
  class Indexator: public Jsonable
  {
  public:

    using MyIndexator = Indexator<EnumKey, T>;

    EnumKey NewKey()
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

    const T& operator[] (EnumKey KeyItem) const
    {
      auto it = _Items.find(KeyItem);
      if (it == _Items.end()) return _NotFound;
      return it->second;
    }

    T& operator[] (EnumKey KeyItem)
    {
      auto it = _Items.find(KeyItem);
      if (it == _Items.end()) return _NotFound;
      return it->second;
    }

    EnumKey operator()(TYPE_KEY key) const
    {
      auto f = _Index2.find(key);
      if (f != _Index2.end()) return f->second;
      return static_cast<EnumKey>(NoFoundKey);
    }

    TYPE_KEY operator()(EnumKey key) const
    {
      auto f = _Index1.find(key);
      if (f != _Index1.end()) return f->second;
      return NoFoundKey;
    }

    std::set<TYPE_KEY> ConvertToIndex(std::set<EnumKey> keys) const
    {
      std::set<TYPE_KEY> retval;
      for(EnumKey k: keys)
      {
        TYPE_KEY Index1 = operator()(k);
        if (Index1 != NoFoundKey)
          retval.insert(Index1);
      }
      return retval;
    }

    std::set<EnumKey> ConvertToKey(std::set<TYPE_KEY> indexes) const
    {
      std::set<EnumKey> retval;
      for (TYPE_KEY k : indexes)
      {
        EnumKey Index1 = operator()(k);
        if (static_cast<TYPE_KEY>(Index1) != NoFoundKey)
          retval.insert(Index1);
      }
      return retval;
    }

    virtual ~Indexator() {}

    virtual int ReadFromJson(const Json::Value& jsonPr) override
    {
      _Items.clear();
      //FillNoFindItem(_NotFound);
      for (auto it: jsonPr) {
        T toRead;
        toRead.ReadFromJson(it);
        _Items[toRead.GetKey()] = toRead;
      }
      UpdateIndex();
      return 0;
    }

    virtual int WriteToJson(Json::Value& jsonPr) const override
    {
      jsonPr = Json::Value(Json::arrayValue);
      for (auto& it: _Items) {
        Json::Value newVal;
        it.second.WriteToJson(newVal);
        jsonPr.append(newVal);
      }
      return 0;
    }

    std::map<EnumKey, T> GetByConditions(std::function<bool(const T&)> func) const
    {
      std::map<EnumKey, T> retval;
      for (const auto& item : _Items)
        if (func(item.second))
          retval[item.first] = item.second;
      return retval;
    }

    virtual void Add(const std::map<EnumKey, T>& adds)
    {
      for (auto& it : adds)
        _Items[it.first] = it.second;
      UpdateIndex();
    }

    virtual void Add(const std::set<T>& adds)
    {
      for (auto& it : adds)
        _Items[it.GetKey()] = it;
      UpdateIndex();
    }

    virtual void Add(const T& add)
    {
      _Items[add.GetKey()] = add;
      UpdateIndex();
    }

    virtual bool Delete(const std::set<EnumKey>& keys)
    {
      for (auto& it: keys) {
        _Items.erase(it);
      }
      UpdateIndex();
      return true;
    }

    void IteratorForAllItem(std::function<bool(T&)> func)
    {
      for (auto& it: _Items)
        func(it.second);
    }

    virtual void CloneFrom(const MyIndexator& other)
    {
      *this = other;
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

  private:

    TYPE_KEY _LastGenGen = 0;
    TYPE_KEY _Size = 0;
    T _NotFound;

    std::map<EnumKey, TYPE_KEY> _Index1;
    std::map<TYPE_KEY, EnumKey> _Index2;
    std::map<EnumKey, T> _Items;
  };

}
