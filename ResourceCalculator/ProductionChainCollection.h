#ifndef ProductionChainCollection_H
#define ProductionChainCollection_H

#include "ProductionChainModel.h"

namespace ResourceCalculator {

class ProductionChainCollection : public Jsonable
{
private:
  std::set<ProductionChainModel*> _PCMs;
  const ParamsCollection &_PC;
public:
  ProductionChainCollection(const ParamsCollection &PC);
  ~ProductionChainCollection();
  ProductionChainModel *Add(KEY_ITEM itemID);
  void Remove(ProductionChainModel *PCM);
  std::set<ProductionChainModel*> &GetReadingPCMs();
  int ReadFromJson(const Json::Value &jsonPr) override;
  int WriteToJson(Json::Value &jsonPr) const override;
};
}

#endif // ProductionChainCollection_H
