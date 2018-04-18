#include <assert.h>
#include "TransportBeltCollection.h"
#include "RecipeCollection.h"

namespace ResourceCalculator {

  TransportBeltCollection::TransportBeltCollection( ):
    _LastGenGen(0)
  {
  }

  TransportBeltCollection::~TransportBeltCollection()
  {
  }

  void TransportBeltCollection::Add( const std::map<KEY_TRANSPORT_BELT, TransportBelt > &ModulesToAdd )
  {
    for ( auto & it : ModulesToAdd ) {
      _Items[it.first] = it.second;
    }
  }

  void TransportBeltCollection::Delete( const std::set<KEY_TRANSPORT_BELT>& ItemsKeysToDel )
  {
    for ( auto & it : ItemsKeysToDel ) {
      bool ToDel = false;
      for ( auto &itm : _Items ) {
        if ( itm.first == it ) {
          ToDel = true;
          break;
        }
      }
      if ( ToDel ) {
        _Items.erase( it );
      }
    }
  }
  
  int TransportBeltCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto &it: jsonPr){
      TransportBelt ToAdd;
      ToAdd.ReadFromJson(it);
      TYPE_KEY AddKey = static_cast<TYPE_KEY>(ToAdd.GetKey());
      if (_LastGenGen < AddKey) _LastGenGen = AddKey;
      _Items[ToAdd.GetKey()] = ToAdd;
    }
    return 0;
  }

  int TransportBeltCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Items){
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

  const std::map<KEY_TRANSPORT_BELT, TransportBelt>& TransportBeltCollection::GetData() const
  {
    return _Items;
  }

  KEY_TRANSPORT_BELT TransportBeltCollection::GetUniqueItemKey()
  {
    TYPE_KEY retval = _LastGenGen + 1;
    if (_Items.size() > 0) {
      while (_Items.find(static_cast<KEY_TRANSPORT_BELT>(retval)) != _Items.end()) {
        retval++;
      }
    }
    _LastGenGen = retval;
    return static_cast<KEY_TRANSPORT_BELT>(retval);
  }

}
