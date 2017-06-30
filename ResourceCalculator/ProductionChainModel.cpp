#include <assert.h>

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

    ItemResultTree IRT = _PC.RC.BuildTree(KEY_ITEM::ID_ITEM_Paket1, 8);

    std::list <KEY_RECIPE> ResultRecipes;
    std::list <KEY_ITEM> ResultItems;
    std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

    _PC.RC.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);


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

  bool ProductionChainModel::SetAnsfer(std::map<KEY_ITEM, KEY_RECIPE>& AnsferRecipeKey, int row)
  {
    _AnsferRecipeKey = AnsferRecipeKey;
    return Rebuild();
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

  bool ProductionChainDataRow::Build(const ParamsCollection &PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const FactoryModules & FM, int CountFactorys)
  {
    assert(_FM.FactoryID == FactoryCurrent);

    bool RetVal = RecipeCurrent == RecipeId;

    RetVal = RetVal && FactoryCurrent == FactoryId;
    RetVal = RetVal && _FM.GetModules() == FM.GetModules();

    if (RetVal) {
      return true;
    }

    RecipeCurrent  = RecipeId;
    FactoryCurrent = FactoryId;

    const Factory &factory = PC.FC.GetFactory(FactoryId);
    
    //TODO modules

    //_SpeedFactory = factory.


    //KEY_RECIPE  RecipeCurrent;
    //KEY_FACTORY FactoryCurrent;

    ////double _PeakPower;
    //double _SpeedFactory;
    //double _SecPerOneRecipe;
    //double _ProductionSpeedPerSecond;
    //double _CountFactorys;

    //std::vector <KEY_RECIPE>  _Recipes;
    //std::vector <KEY_FACTORY> _Factorys;
    //FactoryModules _FM;

    //std::vector <double> _CountItems;
    //std::vector <double> _ItemsPerSec;

    

    return false;
  }

}