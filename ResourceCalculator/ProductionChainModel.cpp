#include <assert.h>

#include "ProductionChainModel.h"


namespace ResourceCalculator {

  bool ProductionChainDataRow::_Init(const ParamsCollection & PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM>&Cols)
  {
    bool RetVal = RecipeCurrent == RecipeId && FactoryCurrent == FactoryId;

    if (RetVal) {
      return false;
    }

    RecipeCurrent = RecipeId;
    FactoryCurrent = FactoryId;

    const bool IsFactoryOk = PC.FC.GetFactory(FactoryId).IsAllowedProduction(PC, RecipeId);

    const size_t CountsCols = Cols.size();

    _Factorys.clear();
    _CountItems.clear();
    _ItemsPerSec.clear();
    _ColItems.clear();

    std::map<KEY_FACTORY, Factory> Factorys = PC.FC.GetFactorys();
    for (auto & it : Factorys) {
      if (it.second.IsAllowedProduction(PC, RecipeId)) {
        if (IsFactoryOk) {
          FactoryCurrent = it.first;
        }
        _Factorys.push_back(it.first);
      }
    }

    const Factory &factory = PC.FC.GetFactory(FactoryCurrent);
    const Recipe &recipe = PC.RC.GetData().find(RecipeCurrent)->second;
    const RecipeParams RP = recipe.GetRecipeParams();

    factory.FixFactoryModules(_FM);

    _SecPerOneRecipe = RP.Time;
    _SpeedFactory = factory.GetSpeed() * _FM.GetSummSpeed(PC.MC);
    _RealTimeProductionOfOneItemPerSec = _SpeedFactory / _SecPerOneRecipe;

    double ProductionSpeedPerSecond = _FM.GetSummProductivity(PC.MC);

    _PeakPower = 0.0;
    _LevelOfPollution = 0.0;
    _CountFactorys = 0.0;

    _CountItems.resize(CountsCols, 0.0);
    _ItemsPerSec.resize(CountsCols, 0.0);

    _ColItems = Cols;

    for (size_t ColId = 0; ColId < CountsCols; ColId++) {
      const KEY_ITEM ItemKey = _ColItems[ColId];
      for (auto Required : RP.Required) {
        if (Required.ItemId == ItemKey) {
          _CountItems[ColId] = -Required.Count;
        }
      }
      for (auto Result : RP.Result) {
        if (Result.ItemId == ItemKey) {
          _CountItems[ColId] += Result.Count * ProductionSpeedPerSecond;
        }
      }
    }

    return true;
  }

  bool ProductionChainDataRow::_SetCountFactorys( double Count)
  {
    bool RetVal = _CountFactorys != Count;
    if (RetVal) {
      return false;
    }
    _CountFactorys = Count;
    const size_t CountsCols = _CountItems.size();
    for (size_t ColId = 0; ColId < CountsCols; ColId++) {
      _ItemsPerSec[ColId] = _CountItems[ColId] / _RealTimeProductionOfOneItemPerSec * _CountFactorys;
    }
    return RetVal;
  }

  bool ProductionChainDataRow::_SetFactoryModules(const ParamsCollection & PC, const FactoryModules & FM)
  {
    const size_t CountsCols = _CountItems.size();

    const Recipe &recipe = PC.RC.GetData().find(RecipeCurrent)->second;
    const RecipeParams RP = recipe.GetRecipeParams();

    double ProductionSpeedPerSecond = _FM.GetSummProductivity(PC.MC);

    for (size_t ColId = 0; ColId < CountsCols; ColId++) {
      const KEY_ITEM ItemKey = _ColItems[ColId];
      for (auto Required : RP.Required) {
        if (Required.ItemId == ItemKey) {
          _CountItems[ColId] = -Required.Count;
        }
      }
      for (auto Result : RP.Result) {
        if (Result.ItemId == ItemKey) {
          _CountItems[ColId] += Result.Count * ProductionSpeedPerSecond;
        }
      }
      _ItemsPerSec[ColId] = _CountItems[ColId] / _RealTimeProductionOfOneItemPerSec * _CountFactorys;
    }
    return false;
  }

  FactoryModules ProductionChainDataRow::_GetFactoryModules() const
  {
    return _FM;
  }

  const ProductionChainDataRow & ProductionChainModel::GetRow(int Row) const
  {
    return _DataRows[Row];
  }

  bool ProductionChainModel::Optimize()
  {
    return false;
  }

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
    ItemResultTree IRT = _PC.RC.BuildTree(ItemKey, 8);

    std::list <KEY_RECIPE> ResultRecipes;
    std::list <KEY_ITEM> ResultItems;
    std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

    _PC.RC.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);

    const Item &RootItem = _PC.IC.GetData().find(ItemKey)->second;

    _Name = "Production Chain " + RootItem.GetName();

    _ItemKey = ItemKey;
    
    const size_t CountsItems = ResultItems.size();
    const size_t CountsRecipes = ResultRecipes.size();

    _ColsItems.clear();
    _ColsItems.resize(CountsItems);
    std::copy(ResultItems.begin(), ResultItems.end(), _ColsItems.begin());

    _DataRows.clear();
    _DataRows.resize(CountsRecipes);

    auto IT_Recipe = ResultRecipes.begin();

    for (size_t RecipeIdx = 0; RecipeIdx < CountsRecipes; RecipeIdx++){
      KEY_RECIPE RecipeId = *IT_Recipe;
      _DataRows[RecipeIdx]._Init(_PC, RecipeId, KEY_FACTORY::ID_ITEM_NoFind_Factory, _ColsItems);
    }

    return true;

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


  int ProductionChainModel::CountItems() const
  {
    return static_cast<int>(_ColsItems.size());
  }

  int ProductionChainModel::CountRecipes() const
  {
    return static_cast<int>(_DataRows.size());
  }

}