#include <assert.h>
#include "ItemCollection.h"
#include "RecipeCollection.h"

namespace ResourceCalculator {

  ItemCollection::ItemCollection( RecipeCollection &RC ):
    _RC( RC ), _LastGenGey(0)
  {
  }

  ItemCollection::~ItemCollection()
  {
  }

  void ItemCollection::Add( const std::map<KEY_ITEM, Item > &ModulesToAdd )
  {
    for ( auto & it : ModulesToAdd ) {
      _Items[it.first] = it.second;
    }
  }

  void ItemCollection::Delete( const std::set<KEY_ITEM>& ItemsKeysToDel )
  {
    _RC.Delete( ItemsKeysToDel );
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
  
  int ItemCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto &it: jsonPr){
      Item ToAdd;
      ToAdd.ReadFromJson(it);
      TYPE_KEY AddKey = static_cast<TYPE_KEY>(ToAdd.GetKey());
      if (_LastGenGey < AddKey) _LastGenGey = AddKey;
      _Items[ToAdd.GetKey()] = ToAdd;
    }
    return 0;
  }

  int ItemCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Items){
      Json::Value newVal;
      it.second.WriteToJson(newVal);
      jsonPr.append(newVal);
    }
    return 0;
  }

  const std::map<KEY_ITEM, Item>& ItemCollection::GetData() const
  {
    return _Items;
  }

  const Item * ItemCollection::GetItem(KEY_ITEM KeyRecipe) const
  {
    std::map<KEY_ITEM, Item>::const_iterator it = _Items.find(KeyRecipe);
    if (it == _Items.end()) {
      return nullptr;
    }
    return &it->second;
  }

  KEY_ITEM ItemCollection::GetUniqueItemKey()
  {
    TYPE_KEY retval = _LastGenGey + 1;
    if (_Items.size() > 0) {
      while (_Items.find(static_cast<KEY_ITEM>(retval)) != _Items.end()) {
        retval++;
      }
    }
    _LastGenGey = retval;
    return static_cast<KEY_ITEM>(retval);
  }

}
