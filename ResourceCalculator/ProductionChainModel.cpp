#include "ProductionChainModel.h"


namespace ResourceCalculator {


  ProductionChainModel::ProductionChainModel(const ParamsCollection &PC, KEY_ITEM ItemKey):
    ItemBase(""), _ItemKey(ItemKey), _PC(PC)
  {
    SetItemKey(ItemKey);
  }

  ProductionChainModel::ProductionChainModel(const ParamsCollection &PC) :
    ItemBase("Production Chain"), _ItemKey(KEY_ITEM::ID_ITEM_NoFind_Item), _PC(PC)
  {
  }

  ProductionChainModel::~ProductionChainModel()
  {
  }







  bool ProductionChainModel::SetItemKey(KEY_ITEM ItemKey)
  {
    std::string Name = "Production Chain";
    const std::map<KEY_ITEM, Item>& Data = _PC.IC.GetData();
    std::map<KEY_ITEM, Item>::const_iterator find = Data.find(ItemKey);
    if (find != Data.cend()) {
      Name += " " + find->second.GetName();
      _Name = Name;
      ItemResultTree _ItemResultTree = _PC.RC.BuildTree(ItemKey, 120);

      _ItemResultTree.GetResult().size();



    }

    return false;

   

  }

  bool ProductionChainModel::SetFactory(int Row, KEY_FACTORY FactoryId)
  {
    return false;
  }

  bool ProductionChainModel::SetModules(int Row, const std::vector<KEY_MODULE>& Modules)
  {
    return false;
  }

  bool ProductionChainModel::SetRecipe(int Row, KEY_RECIPE RecipeId)
  {
    return false;
  }

  bool ProductionChainModel::Rebuild()
  {
    return false;
  }

  int ProductionChainModel::CountRequired() const
  {
    return 0;
  }

  int ProductionChainModel::CountResult() const
  {
    return 0;
  }

  const ProductionChainDataRow & ProductionChainModel::GetRow(int Row) const
  {
    return _DataRows[Row];
  }

  FactoryModules::FactoryModules(KEY_FACTORY factoryID):
    FactoryID(factoryID)
  {
  }

  const std::vector<KEY_MODULE> FactoryModules::GetModules()
  {
    return _Modules;
  }

  bool FactoryModules::SetModule(int IndexSlot, KEY_MODULE key)
  {
    bool retvalue = _Modules.size() > IndexSlot;
    if (retvalue) _Modules[IndexSlot] = key;
    return retvalue;
  }

}