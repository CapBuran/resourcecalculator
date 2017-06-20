#include <assert.h>
#include "ItemCollection.h"

namespace ResourceCalculator {

  ItemCollection::ItemCollection()
  {
  }

  ItemCollection::~ItemCollection()
  {
  }

  void ItemCollection::ADD(const Item &Item)
  {
    _Items[Item.GetKey()] = Item;
  }

  void ItemCollection::Delete(KEY_ITEM KeyRecipe)
  {
  }

  int ItemCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto it: jsonPr){
      KEY_TO_Json Key  = it["Key"].asInt64();
      std::string Name = it["Name"]["ru"].asString();
      Item ToAdd(Name, static_cast<KEY_ITEM>(Key));
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

  const Item & ItemCollection::GetItem(KEY_ITEM key) const
  {
    return _Items.at(key);
  }

}
