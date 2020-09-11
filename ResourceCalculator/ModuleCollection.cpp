#include <assert.h>
#include "ModuleCollection.h"

namespace ResourceCalculator {

  static Module EmptyModule;

  ModuleCollection::ModuleCollection()
    : _Modules()
    , Indexator<KEY_MODULE, Module>(_Modules)
  {
    EmptyModule.SetName("EMTY SLOT");
  }

  ModuleCollection::~ModuleCollection()
  {
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
    UpdateIndex();
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
    UpdateIndex();
  }

  void ModuleCollection::Delete( const std::set<KEY_MODULE>& ModulesToDel )
  {
    for ( auto & it: ModulesToDel ) {
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
    UpdateIndex();
  }

}
