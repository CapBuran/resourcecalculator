#include <assert.h>
#include "ItemCollection.h"
#include "RecipeCollection.h"

namespace ResourceCalculator {

  Item NotFound;

  ItemCollection::ItemCollection( RecipeCollection &RC )
    : _Items()
    , Indexator<KEY_ITEM, Item>(_Items)
    , _RC( RC )
  {
  }

  ItemCollection::ItemCollection(const ItemCollection& copy)
    : _RC(copy._RC)
    , _Items()
    , Indexator<KEY_ITEM, Item>(_Items)
  {
    *this = copy;
  }

  ItemCollection::~ItemCollection()
  {
  }

  ItemCollection& ItemCollection::operator= (const ItemCollection& ic)
  {
    if (this != &ic)
    {
      _Items = ic._Items;
      ic.CloneTo(*this);
    }
    return *this;
  }

  const RecipeCollection& ItemCollection::GetRecipes() const
  {
    return _RC;
  }
  
  RecipeCollection& ItemCollection::GetRecipes()
  {
    return _RC;
  }

  void ItemCollection::Add( const std::map<KEY_ITEM, Item >& ItemsToAdd )
  {
    for ( auto & it : ItemsToAdd) {
      _Items[it.first] = it.second;
    }

    UpdateIndex();
  }

  bool ItemCollection::Delete( const std::set<KEY_ITEM>& ItemsKeysToDel )
  {
    bool retival = _RC.Delete( ItemsKeysToDel );
    const auto oldSize = _Items.size();
    for ( auto& it: ItemsKeysToDel ) {
      bool ToDel = false;
      for ( auto& itm : _Items ) {
        if ( itm.first == it ) {
          ToDel = true;
          break;
        }
      }
      if ( ToDel ) {
        _Items.erase( it );
      }
    }
    UpdateIndex();
    return oldSize > _Items.size() || retival;
  }
  
  int ItemCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto &it: jsonPr){
      Item ToAdd;
      ToAdd.ReadFromJson(it);
      TYPE_KEY AddKey = static_cast<TYPE_KEY>(ToAdd.GetKey());
      _Items[ToAdd.GetKey()] = ToAdd;
    }
    UpdateIndex();
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

}
