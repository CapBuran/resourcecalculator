#include <FactoryCollection.h>

namespace ResourceCalculator
{
  FactoryTypeCollection::FactoryTypeCollection(FactoryCollection& fc)
    : _FC(fc)
  {
  }

  FactoryTypeCollection::FactoryTypeCollection(const FactoryTypeCollection& copy)
    : _FC(copy._FC)
  {
  }

  bool FactoryTypeCollection::Delete(const std::set<KEY_TYPE_FACTORY>& keys)
  {
    _FC.DeleteFactoryTypes(keys);
    MyIndexator::Delete(keys);
    return true;
  }

  bool FactoryCollection::DeleteFactoryTypes(const std::set<KEY_TYPE_FACTORY>& keys)
  {
    IteratorForAllItem([&](Factory& factory) { return factory.DeleteTypes(keys); });
    return true;
  }
}
