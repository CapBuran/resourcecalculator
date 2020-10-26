#include "FactoryCollection.h"

namespace ResourceCalculator
{
  FactoryTypeCollection::FactoryTypeCollection(FactoryCollection& owner)
    : _TypesFactory()
    , Indexator<KEY_TYPE_FACTORY, FactoryType>(_TypesFactory)
    , _Owner(owner)
  {
    _UNKNOWN_FactoryType.IconPath = "__base__/graphics/icons/coin.png";
    _TypesFactory[KEY_TYPE_FACTORY::Unknown] = _UNKNOWN_FactoryType;
  }

  FactoryTypeCollection::~FactoryTypeCollection()
  {
  }

  void FactoryTypeCollection::CloneTo(FactoryTypeCollection& ref) const
  {
    ref._UNKNOWN_FactoryType = _UNKNOWN_FactoryType;
    ref._TypesFactory = _TypesFactory;
    Indexator<KEY_TYPE_FACTORY, FactoryType>::CloneTo(ref);
  }

  FactoryCollection::FactoryCollection()
    : _Factorys()
    , Indexator<KEY_FACTORY, Factory>(_Factorys)
  {
    _NoFindFactory.SetKey( KEY_FACTORY::ID_ITEM_NoFind_Factory );
    _NoFindFactory.SetSpeed( 0.0 );
    _NoFindFactory.SetCountSlotsForModules( 0 );
    _NoFindFactory.SetCountSlotsForRecipes( 0 );
    _NoFindFactory.SetWear( 1.0 );
    _NoFindFactory.SetPower( 2E200 );
    _NoFindFactory.SetLevelOfPollution( 2E200 );
    _Types = std::make_unique<FactoryTypeCollection>(*this);
  }

  FactoryCollection::~FactoryCollection()
  {
  }

  FactoryCollection& FactoryCollection::operator=(const FactoryCollection& fc)
  {
    if (this != &fc)
    {
      fc.CloneTo(*this);
      fc._Types->CloneTo(*_Types);
      _Factorys = fc._Factorys;
      _NoFindFactory= fc._NoFindFactory;
    }
    return *this;
  }

  FactoryCollection::FactoryCollection(const FactoryCollection& copy)
    : _Factorys()
    , Indexator<KEY_FACTORY, Factory>(_Factorys)
  {
    *this = copy;
  }


  void FactoryTypeCollection::AddFactorysTypes(std::map<KEY_TYPE_FACTORY, FactoryType> factoryTypes)
  {
    for (auto& it: factoryTypes) {
      _TypesFactory[it.first] = it.second;
    }
    UpdateIndex();
  }

  void FactoryCollection::AddFactorys(std::map<KEY_FACTORY, Factory> factorys)
  {
    for (auto& it : factorys) {
      _Factorys[it.first] = it.second;
    }
    UpdateIndex();
  }

  void FactoryTypeCollection::DeleteFactorysTypes(std::set<KEY_TYPE_FACTORY> FactoryTypesKey)
  {
    for (auto& it : FactoryTypesKey) {
      if (it != KEY_TYPE_FACTORY::Unknown) {
        _TypesFactory.erase(it);
      }
    }
    UpdateIndex();
  }

  void FactoryCollection::DeleteFactorys(std::set<KEY_FACTORY> factoresKey)
  {
    for (auto& it: factoresKey) {
      _Factorys.erase(it);
    }
    UpdateIndex();
  }

  const FactoryType& FactoryTypeCollection::GetFactoryType(KEY_TYPE_FACTORY Key) const
  {
    if (Key == KEY_TYPE_FACTORY::Unknown) return _UNKNOWN_FactoryType;
    auto f = _TypesFactory.find(Key);
    if (f != _TypesFactory.end())
    {
      return f->second;
    }
    return _UNKNOWN_FactoryType;
  }

  FactoryType& FactoryTypeCollection::GetFactoryType(KEY_TYPE_FACTORY Key)
  {
    if (Key == KEY_TYPE_FACTORY::Unknown) return _UNKNOWN_FactoryType;
    std::map<KEY_TYPE_FACTORY, FactoryType>::iterator F = _TypesFactory.find(Key);
    if (F == _TypesFactory.end())
    {
      throw(std::exception("F == _TypesFactory.end()"));
    }
    return F->second;
  }

  const Factory& FactoryCollection::GetFactory( KEY_FACTORY Key ) const
  {
    if ( Key == KEY_FACTORY::ID_ITEM_NoFind_Factory ) return _NoFindFactory;
    std::map<KEY_FACTORY, Factory>::const_iterator F = _Factorys.find( Key );
    return F == _Factorys.end() ? _NoFindFactory : F->second;
  }

  Factory& FactoryCollection::GetFactoryForEdit( KEY_FACTORY Key )
  {
    if (Key == KEY_FACTORY::ID_ITEM_NoFind_Factory) return _NoFindFactory;
    std::map<KEY_FACTORY, Factory>::iterator F = _Factorys.find(Key);
    if (F == _Factorys.end())
    {
      throw(std::exception("F == _Factorys.end()"));
    }
    return F->second;
  }

  int FactoryTypeCollection::ReadFromJson(const Json::Value& jsonPr)
  {
    _TypesFactory.clear();
    _TypesFactory[KEY_TYPE_FACTORY::Unknown] = _UNKNOWN_FactoryType;
    for (auto it : jsonPr) {
      KEY_TYPE_FACTORY KeyType = static_cast<KEY_TYPE_FACTORY>(it["Key"].asInt64());
      _TypesFactory[KeyType].Name = it["Name"].asString();
      _TypesFactory[KeyType].IconPath = it["IconPath"].asString();
    }
    UpdateIndex();
    return 0;
  }

  int FactoryCollection::ReadFromJson(const Json::Value& jsonPr)
  {
    const Json::Value& jsonPrFactorys = jsonPr["Factorys"];
    const Json::Value& jsonPrTypeFactorys = jsonPr["FactorysTypes"];
    _Types->ReadFromJson(jsonPrTypeFactorys);
    _Factorys.clear();
    for (auto it : jsonPrFactorys) {
      Factory factory;
      factory.ReadFromJson(it);
      _Factorys[factory.GetKey()] = factory;
    }
    UpdateIndex();
    return 0;
  }

  int FactoryCollection::WriteToJson(Json::Value& jsonPr) const
  {
    Json::Value& jsonPrFactorys = jsonPr["Factorys"];
    Json::Value& jsonPrTypeFactorys = jsonPr["FactorysTypes"];
    jsonPr["Factorys"] = Json::Value(Json::arrayValue);
    for (auto& it: _Factorys) {
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPrFactorys.append(newVal);
    }
    _Types->WriteToJson(jsonPrTypeFactorys);
    return 0;
  }

  std::map<KEY_FACTORY, Factory> FactoryCollection::GetFactoryByConditions(std::function<bool(const Factory&)> func) const
  {
    std::map<KEY_FACTORY, Factory> retval;
    for (const auto& factory: _Factorys)
    {
      if (func(factory.second))
      {
        retval[factory.first] = factory.second;
      }
    }
    return retval;
  }

  void FactoryCollection::DeleteTypes(std::set<KEY_TYPE_FACTORY> deletes)
  {
    for (auto& factory: _Factorys)
    {
      auto types = factory.second.GetTypes();
      for (KEY_TYPE_FACTORY type: deletes)
      {
        types.erase(type);
      }
      factory.second.SetTypes(types);
    }
    UpdateIndex();
  }

  FactoryTypeCollection& FactoryCollection::GetTypes()
  {
    return *_Types;
  }

  const FactoryTypeCollection& FactoryCollection::GetTypes() const
  {
    return *_Types;
  }

  int FactoryTypeCollection::WriteToJson(Json::Value& jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it: _TypesFactory) {
      Json::Value newVal;
      newVal["Key"] = static_cast<KEY_TO_Json>(it.first);
      newVal["Name"] = it.second.Name;
      newVal["IconPath"] = it.second.IconPath;
      jsonPr.append(newVal);
    }
    return 0;
  }
}
