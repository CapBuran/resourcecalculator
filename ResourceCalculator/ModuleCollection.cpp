#include <assert.h>
#include "ModuleCollection.h"

namespace ResourceCalculator {

  static Module EmptyModule;

  ModuleCollection::ModuleCollection()
  {
    EmptyModule.SetName("EMTY SLOT");
    _LastRetvalUniqueModuleKey = KEY_MODULE::ID_CleanSlot;
  }

  ModuleCollection::~ModuleCollection()
  {
  }
  
  const std::map<KEY_MODULE, Module> & ModuleCollection::GetModules() const
  {
    return _Modules;
  }

  const Module & ModuleCollection::GetModule(KEY_MODULE ModuleKey) const
  {
    std::map<KEY_MODULE, Module>::const_iterator module = _Modules.find(ModuleKey);
    return module == _Modules.end() ? EmptyModule : module->second;
  }

  int ModuleCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    _Modules.clear();
    _Modules[KEY_MODULE::ID_CleanSlot] = EmptyModule;
    for ( auto it : jsonPr ) {
      Module module;
      module.ReadFromJson( it );
      _Modules[module.GetKey()] = module;
    }
    return 0;
  }

  int ModuleCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value( Json::arrayValue );
    for ( auto& it : _Modules ) {
      Json::Value newVal;
      it.second.WriteToJson( newVal );
      jsonPr.append( newVal );
    }
    return 0;
  }

  void ModuleCollection::Add( const std::map<KEY_MODULE, Module > &ModulesToAdd )
  {
    for ( auto & it : ModulesToAdd ) {
      _Modules[it.first] = it.second;
    }
  }

  void ModuleCollection::Delete( const std::set<KEY_MODULE>& ModulesToDel )
  {
    for ( auto & it : ModulesToDel ) {
      bool ToDel = false;
      for ( auto &itm : _Modules ) {
        if ( itm.first == it ) {
          ToDel = true;
          break;
        }
      }
      if ( ToDel ) {
        _Modules.erase( it );
      }
    }
  }

  KEY_MODULE ModuleCollection::GetUniqueModuleKey()
  {
    TYPE_KEY retval = static_cast<TYPE_KEY>( _LastRetvalUniqueModuleKey) + 1;
    while ( _Modules.find( static_cast<KEY_MODULE>( retval ) ) != _Modules.end() ) {
      retval++;
    }
    _LastRetvalUniqueModuleKey = static_cast< KEY_MODULE >( retval );
    return  _LastRetvalUniqueModuleKey;
  }

}
