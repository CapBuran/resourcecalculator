#ifndef FactoryCollectionH
#define FactoryCollectionH

#include "Factory.h"

namespace ResourceCalculator {

  class FactoryCollection: public Jsonable
  {
  public:
    FactoryCollection();
    ~FactoryCollection();

    void ADD(const Factory &);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

  private:
    std::map<KEY_FACTORY, Factory> _Factorys;

  };

}

#endif // !FactoryCollectionH