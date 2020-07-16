#ifndef ProductionChainCollection_H
#define ProductionChainCollection_H

#include <ProductionChainModel.h>

namespace ResourceCalculator {

class ProductionChainCollection : public Jsonable
{
private:
  std::set<ProductionChainModel*> _PCMs;
  const ParamsCollection &_PC;
  FullItemTree& _tree;
public:
  ProductionChainCollection(FullItemTree& tree);
  ~ProductionChainCollection();
  ProductionChainModel *Add(KEY_ITEM itemID);
  void Remove(ProductionChainModel *PCM);
  const std::set<ProductionChainModel*> &GetReadingPCMs() const;
  const ProductionChainModel *GetPCM(int IdPCM) const;
  int ReadFromJson(const Json::Value &jsonPr) override;
  int WriteToJson(Json::Value &jsonPr) const override;
};
}

#endif // ProductionChainCollection_H
