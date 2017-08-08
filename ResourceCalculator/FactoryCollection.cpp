#include "FactoryCollection.h"

namespace ResourceCalculator
{

  FactoryCollection::FactoryCollection()
  {
    _LastRetvalUniqueFactoryKey = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _LastRetvalUniqueFactoryTypeKey = KEY_TYPE_FACTORY::Unknown;

    _NoFindFactory.SetKey( KEY_FACTORY::ID_ITEM_NoFind_Factory );
    _NoFindFactory.SetSpeed( 0.0 );
    _NoFindFactory.SetCountSlotsForModules( 0 );
    _NoFindFactory.SetCountSlotsForRecipes( 0 );
    _NoFindFactory.SetWear( 1.0 );
    _NoFindFactory.SetPower( INFINITY );
    _NoFindFactory.SetLevelOfPollution( INFINITY );
    _NoFindFactory.SetKey( KEY_FACTORY::ID_ITEM_NoFind_Factory );
    _NoFindFactory.SetKey( KEY_FACTORY::ID_ITEM_NoFind_Factory );
    _NoFindFactory.SetKey( KEY_FACTORY::ID_ITEM_NoFind_Factory );
    _UNKNOWN_FactoryType.IconPath = "__base__/graphics/icons/coin.png";

    _TypesFactory[KEY_TYPE_FACTORY::Unknown] = _UNKNOWN_FactoryType;
  }

  FactoryCollection::~FactoryCollection()
  {
  }

  void FactoryCollection::ADD( const Factory factory )
  {
    _Factorys[factory.GetKey()] = factory;
  }

  const Factory & FactoryCollection::GetFactory( KEY_FACTORY Key ) const
  {
    if ( Key == KEY_FACTORY::ID_ITEM_NoFind_Factory ) return _NoFindFactory;
    return _Factorys.find( Key )->second;
  }

  const FactoryType &FactoryCollection::GetFactoryType( KEY_FACTORY Key ) const
  {
    const Factory & facory = GetFactory( Key );
    
    auto f = _TypesFactory.find( facory.GetType() );
    if ( f != _TypesFactory.end() ) {
      return f->second;
    }

    return _UNKNOWN_FactoryType;
  }

  Factory & FactoryCollection::GetFactoryForEdit( KEY_FACTORY Key )
  {
    if ( Key == KEY_FACTORY::ID_ITEM_NoFind_Factory ) return _NoFindFactory;
    return _Factorys.find( Key )->second;
  }

  const std::map<KEY_TYPE_FACTORY, FactoryType >& FactoryCollection::GetTypesFactorys() const
  {
    return _TypesFactory;
  }

  const std::map<KEY_FACTORY, Factory>& FactoryCollection::GetFactorys() const
  {
    return _Factorys;
  }


  void FactoryCollection::AddFactorysTypes( const std::map<KEY_TYPE_FACTORY, FactoryType > &FactoryTypes )
  {
    for ( auto & it : FactoryTypes ) {
      _TypesFactory[it.first] = it.second;
    }
  }

  void FactoryCollection::DeleteFactorysTypes( const std::list<KEY_TYPE_FACTORY> &FactoryTypesKey )
  {
    for ( auto & it : FactoryTypesKey ) {
      for ( auto &itf : _Factorys ) {
        if ( it == itf.second.GetType() ) {
          itf.second.SetType( KEY_TYPE_FACTORY::Unknown );
        }
      }
      if ( it != KEY_TYPE_FACTORY::Unknown ) {
        _TypesFactory.erase( it );
      }
    }
  }

  void FactoryCollection::DeleteFactorys( const std::list<KEY_FACTORY>& FactoryTypesKey )
  {
    for ( auto & it : FactoryTypesKey ) {
      _Factorys.erase( it );
    }
  }

  void FactoryCollection::AddFactorys( const std::map<KEY_FACTORY, Factory>& FactoryTypes )
  {
    for ( auto & it : FactoryTypes ) {
      _Factorys[it.first] = it.second;
    }
  }

  KEY_FACTORY FactoryCollection::GetUniqueFactoryKey()
  { 
    TYPE_KEY retval = static_cast<TYPE_KEY>(_LastRetvalUniqueFactoryKey) + 1;
    while ( _Factorys.find( static_cast<KEY_FACTORY>( retval ) ) != _Factorys.end() ) {
      retval++;
    }
    _LastRetvalUniqueFactoryKey = static_cast< KEY_FACTORY >( retval );
    return _LastRetvalUniqueFactoryKey;
  }

  KEY_TYPE_FACTORY FactoryCollection::GetUniqueFactoryTypeKey()
  {
    TYPE_KEY retval = static_cast<TYPE_KEY>( _LastRetvalUniqueFactoryTypeKey ) + 1;
    while ( _TypesFactory.find( static_cast<KEY_TYPE_FACTORY>( retval ) ) != _TypesFactory.end() ) {
      retval++;
    }
    _LastRetvalUniqueFactoryTypeKey = static_cast< KEY_TYPE_FACTORY >( retval );
    return  _LastRetvalUniqueFactoryTypeKey;
  }

  int FactoryCollection::ReadFromJson( const Json::Value & jsonPr )
  {
    _Factorys.clear();
    for ( auto it : jsonPr ) {
      Factory factory;
      factory.ReadFromJson( it );
      _Factorys[factory.GetKey()] = factory;
    }
    return 0;
  }

  int FactoryCollection::WriteToJson( Json::Value & jsonPr ) const
  {
    jsonPr = Json::Value( Json::arrayValue );
    for ( auto& it : _Factorys ) {
      Json::Value newVal;
      it.second.WriteToJson( newVal );
      jsonPr.append( newVal );
    }
    return 0;
  }

  int FactoryCollection::ReadFromJsonFactoryTypes( const Json::Value & jsonPr )
  {
    _TypesFactory.clear();
    _TypesFactory[KEY_TYPE_FACTORY::Unknown] = _UNKNOWN_FactoryType;
    for ( auto it : jsonPr ) {
      KEY_TYPE_FACTORY KeyType = static_cast<KEY_TYPE_FACTORY>( it["Key"].asInt64() );
      _TypesFactory[KeyType].Name     = it["Name"].asString();
      _TypesFactory[KeyType].IconPath = it["IconPath"].asString();
    }
    return 0;
  }

  int FactoryCollection::WriteToJsonFactoryTypes( Json::Value & jsonPr ) const
  {
    jsonPr = Json::Value( Json::arrayValue );
    for ( auto& it : _TypesFactory ) {
      Json::Value newVal;
      newVal["Key"]      = static_cast<KEY_TO_Json>( it.first );
      newVal["Name"]     = it.second.Name;
      newVal["IconPath"] = it.second.IconPath;
      jsonPr.append( newVal );
    }
    return 0;
  }

}
