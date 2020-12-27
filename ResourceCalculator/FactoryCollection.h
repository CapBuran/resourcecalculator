#pragma once
#include <Factory.h>

namespace ResourceCalculator
{
  using FactoryType = ItemBase<KEY_TYPE_FACTORY>;
  class FactoryCollection;

  class FactoryTypeCollection: public Indexator<KEY_TYPE_FACTORY, FactoryType>
  {
  public:
    FactoryTypeCollection(FactoryCollection& fc);
    FactoryTypeCollection(const FactoryTypeCollection& copy);
    bool Delete(const std::set<KEY_TYPE_FACTORY>& keys) override;
    FactoryCollection& GetFactoryCollection() { return _FC; }
    const FactoryCollection& GetFactoryCollection() const { return _FC; }
  private:
    FactoryCollection& _FC;
  };

  class FactoryCollection: public Indexator<KEY_FACTORY, Factory>
  {
  public:
    bool DeleteFactoryTypes(const std::set<KEY_TYPE_FACTORY>& keys);
  };

}
