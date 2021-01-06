#include <assert.h>
#include <ItemCollection.h>
#include <RecipeCollection.h>

namespace ResourceCalculator {

  ItemCollection::ItemCollection( RecipeCollection &RC )
    : _RC( RC )
  {
  }

  ItemCollection::ItemCollection(const ItemCollection& copy)
    : _RC(copy._RC)
  {
    CloneFrom(copy);
  }

  bool ItemCollection::Delete(const std::set<KEY_ITEM>& keys)
  {
    _RC.DeleteItems(keys);
    MyIndexator::Delete(keys);
    return true;
  }

  void ItemCollection::CloneFrom(const MyIndexator& other)
  {
    MyIndexator::CloneFrom(other);
    _RC.CloneFrom(dynamic_cast<const ItemCollection&>(other).GetRecipes());
  }

}
