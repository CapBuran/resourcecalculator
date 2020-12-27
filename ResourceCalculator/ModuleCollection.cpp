#include <assert.h>
#include "ModuleCollection.h"

namespace ResourceCalculator {

  //static Module EmptyModule;

  //ModuleCollection::ModuleCollection()
  //  : _Modules()
  //  , Indexator<KEY_MODULE, Module>(_Modules)
  //{
  //  EmptyModule.SetName("EMTY SLOT");
  //}

  //ModuleCollection::~ModuleCollection()
  //{
  //}  

  //const Module & ModuleCollection::GetModule(KEY_MODULE ModuleKey) const
  //{
  //  std::map<KEY_MODULE, Module>::const_iterator module = _Modules.find(ModuleKey);
  //  return module == _Modules.end() ? EmptyModule : module->second;
  //}

  //ModuleCollection::ModuleCollection(const ModuleCollection& copy)
  //  :  Indexator<KEY_MODULE, Module>(_Modules)
  //{
  //  *this = copy;
  //}

  //ModuleCollection& ModuleCollection::operator=(const ModuleCollection& rc)
  //{
  //  if (this != &rc)
  //  {
  //    _Modules = rc._Modules;
  //    rc.CloneTo(*this);
  //  }
  //  return *this;
  //}

  //int ModuleCollection::ReadFromJson(const Json::Value & jsonPr)
  //{
  //  _Modules.clear();
  //  _Modules[KEY_MODULE::ID_CleanSlot] = EmptyModule;
  //  for ( auto it : jsonPr ) {
  //    Module module;
  //    module.ReadFromJson( it );
  //    _Modules[module.GetKey()] = module;
  //  }
  //  UpdateIndex();
  //  return 0;
  //}

  //int ModuleCollection::WriteToJson(Json::Value & jsonPr) const
  //{
  //  jsonPr = Json::Value( Json::arrayValue );
  //  for ( auto& it : _Modules ) {
  //    Json::Value newVal;
  //    it.second.WriteToJson( newVal );
  //    jsonPr.append( newVal );
  //  }
  //  return 0;
  //}

  //void ModuleCollection::Add( const std::map<KEY_MODULE, Module > &ModulesToAdd )
  //{
  //  for ( auto & it : ModulesToAdd ) {
  //    _Modules[it.first] = it.second;
  //  }
  //  UpdateIndex();
  //}

  //bool ModuleCollection::Delete( const std::set<KEY_MODULE>& ModulesToDel )
  //{
  //  bool retval = false;
  //  for ( auto & it: ModulesToDel )
  //  {
  //    for ( auto &itm : _Modules )
  //    {
  //      if ( itm.first == it )
  //      {
  //        retval = true;
  //        break;
  //      }
  //    }
  //    if (retval) 
  //    {
  //      _Modules.erase( it );
  //    }
  //  }
  //  UpdateIndex();
  //  return retval;
  //}

}
