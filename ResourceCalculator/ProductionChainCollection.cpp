#include <assert.h>
#include "ProductionChainCollection.h"

namespace ResourceCalculator {
ProductionChainCollection::ProductionChainCollection(const ParamsCollection & PC):
  _PC(PC)
{
}

ProductionChainCollection::~ProductionChainCollection()
{
  for (ProductionChainModel* pcm: _PCMs) {
    delete pcm;
  }
}

ProductionChainModel * ProductionChainCollection::Add(KEY_ITEM itemID)
{
  ProductionChainModel *RetVal = new ProductionChainModel(_PC, itemID);
  _PCMs.insert(RetVal);
  return RetVal;
}

void ProductionChainCollection::Remove(ProductionChainModel * PCM)
{
  assert(_PCMs.find(PCM) != _PCMs.end());
  _PCMs.erase(PCM);
  delete PCM;
}

std::set<ProductionChainModel*>& ProductionChainCollection::GetReadingPCMs()
{
  return _PCMs;
}

int ProductionChainCollection::ReadFromJson(const Json::Value & jsonPr)
{
  for (ProductionChainModel* pcm : _PCMs) {
    delete pcm;
  }
  _PCMs.clear();
  for (auto &it : jsonPr) {
    ProductionChainModel *ToAdd = new ProductionChainModel(_PC);
    ToAdd->ReadFromJson(it);
    _PCMs.insert(ToAdd);
  }
  return 0;
}

int ProductionChainCollection::WriteToJson(Json::Value & jsonPr) const
{
  jsonPr = Json::Value(Json::arrayValue);
  for (auto& it : _PCMs) {
    Json::Value newVal;
    it->WriteToJson(newVal);
    jsonPr.append(newVal);
  }
  return 0;
}

}
