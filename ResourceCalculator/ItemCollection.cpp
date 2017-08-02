#include <assert.h>
#include "ItemCollection.h"

namespace ResourceCalculator {

  ItemCollection::ItemCollection()
  {
  }

  ItemCollection::~ItemCollection()
  {
  }

  void ItemCollection::ADD(const Item Item)
  {
    _Items[Item.GetKey()] = Item;
  }

  bool ItemCollection::DeleteItem(KEY_ITEM KeyItem)
  {
    _Items.erase(KeyItem);
    return true;
  }

  int ItemCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto &it: jsonPr){
      Item ToAdd;
      ToAdd.ReadFromJson(it);
      ADD(ToAdd);
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

  Item * ItemCollection::GetItemForEdit(KEY_ITEM KeyRecipe)
  {
    std::map<KEY_ITEM, Item>::iterator it = _Items.find(KeyRecipe);
    if (it == _Items.end()) {
      return nullptr;
    }
    return &it->second;
  }

  const Item * ItemCollection::GetItem(KEY_ITEM KeyRecipe) const
  {
    std::map<KEY_ITEM, Item>::const_iterator it = _Items.find(KeyRecipe);
    if (it == _Items.end()) {
      return nullptr;
    }
    return &it->second;
  }

  KEY_ITEM ItemCollection::GetUniqueRecipeKey() const
  {
    unsigned int retval = 0;
    if (_Items.size() > 0) {
      while (_Items.find(static_cast<KEY_ITEM>(retval)) != _Items.end()) {
        retval++;
      }
    }
    return static_cast<KEY_ITEM>(retval);
  }

}
