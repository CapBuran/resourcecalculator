#include <assert.h>

#include "ProductionChainModel.h"

namespace ResourceCalculator
{
  const ProductionChainDataRow& ProductionChainModel::GetRow( int Row ) const
  {
    return _DataRows[Row];
  }

  ProductionChainDataRow &ProductionChainModel::GetRowEdit( int Row )
  {
    return _DataRows[Row];
  }

  double ProductionChainDataRow::GetSummProductivity(const ParamsCollection & PC) const
  {
    return _FM.GetSummProductivity(PC.MC);
  }

  double ProductionChainDataRow::GetSummSpeed(const ParamsCollection & PC) const
  {
    return _FM.GetSummSpeed(PC.MC);
  }

  bool ProductionChainDataRow::_Update( const ParamsCollection & PC )
  {
    const size_t CountsCols = _ColsItems.size();

    _Factorys.clear();

    bool IsIsFactoryOkOld = PC.FC.GetFactory( _FactoryCurrent ).IsAllowedProduction( PC, _RecipeCurrent );

    const std::map<KEY_FACTORY, Factory> &Factorys = PC.FC.GetFactorys();
    for ( auto & it : Factorys ) {
      bool IsFactoryOk = it.second.IsAllowedProduction( PC, _RecipeCurrent );
      if ( it.second.IsAllowedProduction( PC, _RecipeCurrent ) ) {
        if ( !IsFactoryOk ) {
          _FactoryCurrent = it.first;
        }
        _Factorys.push_back( it.first );
      }
    }

    if ( _Factorys.size() == 0 ) {
      _Factorys.push_back( KEY_FACTORY::ID_ITEM_NoFind_Factory );
      _FactoryCurrent = KEY_FACTORY::ID_ITEM_NoFind_Factory;
      return true;
    }

    if ( IsIsFactoryOkOld == false ) {
      _FactoryCurrent = *_Factorys.begin();
    }

    const Factory &factory = PC.FC.GetFactory( _FactoryCurrent );
    const Recipe &recipe = PC.RC.GetData().find( _RecipeCurrent )->second;

    _CurrentFactoryName = factory.GetName();
    _CurrentRecipeName = recipe.GetName();

    factory.FixFactoryModules( _FM );
    _SecPerOneRecipe = recipe.GetTime();
    _SpeedFactory = factory.GetSpeed() * _FM.GetSummSpeed( PC.MC );
    _RealTimeProductionOfOneItemPerSec = _SecPerOneRecipe / _SpeedFactory;
    double ProductionSpeedPerSecond = _FM.GetSummProductivity( PC.MC );

    _PeakPower = 0.0;
    _LevelOfPollution = 0.0;

    double OldCountItems = _ItemsPerSec[_InitColumb];

    for ( auto &it : _CountItems ) {
      it = 0.0;
    }
    
    for ( size_t ColId = 0; ColId < CountsCols; ColId++ ) {
      const KEY_ITEM ItemKey = _ColsItems[ColId];
      for ( auto Required : recipe.GetRequired() ) {
        if ( Required.ItemId == ItemKey ) {
          _CountItems[ColId] = -Required.Count;
        }
      }
      for ( auto Result : recipe.GetResult() ) {
        if ( Result.ItemId == ItemKey ) {
          _CountItems[ColId] += Result.Count * ProductionSpeedPerSecond;
        }
      }
    }

    for ( size_t ColId = 0; ColId < CountsCols; ColId++ ) {
      _ItemsPerSec[ColId] = _CountItems[ColId] / _RealTimeProductionOfOneItemPerSec * _CountFactorys;
    }

    return true;
  }

  bool ProductionChainDataRow::Init( const ParamsCollection & PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM>&Cols, int InitColumb )
  {
    if (InitColumb == -1){
      const Recipe *recipe = PC.RC.GetRecipe(RecipeId);
      assert(recipe != nullptr);
      const std::set<CountsItem> &Result = recipe->GetResult();
      for (size_t i = Cols.size() - 1; i >= 0; i--){
        for (CountsItem it : Result) {
          if (Cols[i] == it.ItemId) {
            InitColumb = static_cast<int>(i);
            break;
          }
        }
        if (InitColumb >= 0) {
          break;
        }
      }
    }
    _InitColumb = InitColumb;
    _ColsItems = Cols;
    _RecipeCurrent = RecipeId;
    _FactoryCurrent = FactoryId;
    _CountItems.clear();
    _ItemsPerSec.clear();
    _CountItems.resize( _ColsItems.size(), 0.0 );
    _ItemsPerSec.resize( _ColsItems.size(), 0.0 );
    _Update( PC );
    return true;
  }
  
  bool ProductionChainDataRow::ReInit(const ParamsCollection &PC)
  {
    _CountItems.clear();
    _ItemsPerSec.clear();
    _CountItems.resize(_ColsItems.size(), 0.0);
    _ItemsPerSec.resize(_ColsItems.size(), 0.0);
    _Update(PC);
    return true;
  }

  void ProductionChainDataRow::DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel )
  {
    _FM.DeleteModules( ModulesToDel );
  }

  KEY_FACTORY ProductionChainDataRow::GetFactoryIdFromIndex( int Index ) const
  {
    KEY_FACTORY RetVal = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    if ( 0 <= Index && Index < _Factorys.size() ) RetVal = _Factorys[Index];
    return RetVal;
  }

  bool ProductionChainDataRow::SetCountFactorys( const ParamsCollection &PC, double Count )
  {
    bool RetVal = _CountFactorys == Count;
    if ( RetVal ) {
      return false;
    }
    _CountFactorys = Count;
    _ItemsPerSec[_InitColumb] = 0.0;
    _Update( PC );
    return true;
  }

  bool ProductionChainDataRow::SetFactoryModules( const ParamsCollection &PC, const FactoryModules & FM )
  {
    _FM = FM;
    _Update( PC );
    return true;
  }

  bool ProductionChainDataRow::SetFactoryCurrent( const ParamsCollection &PC, KEY_FACTORY KeyFactory )
  {
    if ( _FactoryCurrent == KeyFactory ) {
      return false;
    }
    double OldItemsCount = _ItemsPerSec[_InitColumb];
    _FactoryCurrent = KeyFactory;
    FindCountFactorysForItemsCount( PC, _InitColumb, OldItemsCount );
    _Update( PC );
    FindCountFactorysForItemsCount( PC, _InitColumb, OldItemsCount );
    return true;
  }

  bool ProductionChainDataRow::FindCountFactorysForItemsCount( const ParamsCollection &PC, int Columb, double Count )
  {
    if ( _CountItems[Columb] == 0.0 ) return false;
    if ( abs( _CountFactorys ) < 0.00001 ) {
      SetCountFactorys( PC, 1.0 );
    }
    double OldCountFactorys = _CountFactorys;
    double OldCount = _ItemsPerSec[Columb];
    if ( OldCount == Count ) return false;
    double Coeff = Count / OldCount;
    _CountFactorys = Coeff * OldCountFactorys;
    _Update( PC );
    return true;
  }

  bool ProductionChainModel::Optimize()
  {
    for ( size_t ItemId = 0; ItemId < _SummSpeeds.size(); ItemId++ ) {
      _SummSpeeds[ItemId] = 0.0;
    }
    for ( auto &row : _DataRows ) {
      for ( size_t ItemId = 0; ItemId < _SummSpeeds.size(); ItemId++ ) {
        _SummSpeeds[ItemId] += row.GetItemsPerSec()[ItemId];
      }
    }
    return true;
  }

  const std::vector<double> ProductionChainModel::GetSummSpeeds() const
  {
    return _SummSpeeds;
  }

  ProductionChainModel::ProductionChainModel( const ParamsCollection &PC, KEY_ITEM ItemKey ):
    _ItemKey( ItemKey ), _PC( PC )
  {
    SetItemKey( ItemKey );
  }

  ProductionChainModel::ProductionChainModel( const ParamsCollection &PC ):
    _ItemKey( KEY_ITEM::ID_ITEM_NoFind_Item ), _PC( PC )
  {
  }

  ProductionChainModel::~ProductionChainModel()
  {
  }

  void ProductionChainModel::DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel )
  {
    for ( auto &it : _DataRows ) {
      it.DeleteModules( ModulesToDel );
    }
  }

  bool ProductionChainModel::SetItemKey( KEY_ITEM ItemKey )
  {
    std::list<KEY_ITEM> ListRequest;
    std::list<KEY_ITEM> ListRequestResourceOnly;

    ItemResultTree IRT = _PC.RC.BuildTree( ItemKey, 100, ListRequest, ListRequestResourceOnly );

    std::list <KEY_RECIPE> ResultRecipes;
    std::list <KEY_ITEM> ResultItems;
    std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

    _PC.RC.Travelling( IRT, Ansfer, ResultRecipes, ResultItems );

    const Item &RootItem = _PC.IC.GetData().find( ItemKey )->second;

    _Name = "Production Chain " + RootItem.GetName();

    _ItemKey = ItemKey;

    const size_t CountsItems = ResultItems.size();
    const size_t CountsRecipes = ResultRecipes.size();

    assert( CountsRecipes < CountsItems );

    _ColsItems.clear();
    _ColsItems.resize( CountsItems );

    if ( ListRequest.size() == CountsRecipes && ListRequestResourceOnly.size() + ListRequest.size() == CountsItems ) {
      std::copy( ListRequestResourceOnly.begin(), ListRequestResourceOnly.end(), _ColsItems.begin() );
      if ( ListRequest.size() > 0 ) {
        std::copy( ListRequest.begin(), ListRequest.end(), &_ColsItems[ListRequestResourceOnly.size()] );
      }
    } else {
      std::copy( ResultItems.begin(), ResultItems.end(), _ColsItems.begin() );
    }

    _DataRows.clear();
    _DataRows.resize( CountsRecipes );

    auto IT_Recipe = ResultRecipes.begin();

    for ( size_t RecipeIdx = 0; RecipeIdx < CountsRecipes; RecipeIdx++, IT_Recipe++ ) {
      KEY_RECIPE RecipeId = *IT_Recipe;
      _DataRows[RecipeIdx].Init(_PC, RecipeId, KEY_FACTORY::ID_ITEM_NoFind_Factory, _ColsItems);
    }

    _SummSpeeds.clear();
    _SummSpeeds.resize( CountsItems, 0.0 );

    return Optimize();

  }

  bool ProductionChainModel::ReInit()
  {
    for (ProductionChainDataRow &PCDR : _DataRows) {
      PCDR.ReInit(_PC);
    }
    return Optimize();
  }

  std::string ProductionChainModel::GetItemName( int Col ) const
  {
    const ResourceCalculator::Item *Item = _PC.IC.GetItem( _ColsItems[Col] );
    if ( Item != nullptr ) {
      return Item->GetName();
    } 
    return "Item deleted!!!";
  }

  bool ProductionChainModel::FitQuantity()
  {
    const int CountRows = ( int ) _DataRows.size();
    const int CountCols = ( int ) _SummSpeeds.size();
    if ( CountRows == 0 || CountRows == 0 ) return false;
    const int Sub = ( CountCols - CountRows ) > 2 ? CountCols - CountRows : 0;
    for ( int ItemId = 0; ItemId < CountCols; ItemId++ ) {
      _SummSpeeds[ItemId] = _DataRows[CountRows-1].GetItemsPerSec()[ItemId];
    }
    for ( int RecipeID = CountRows - 2; RecipeID >= 0; RecipeID-- ) {
      ProductionChainDataRow &ROW = _DataRows[RecipeID];
      int col = ROW.GetInitColumb();
      double RequiresItemsSpeed = _SummSpeeds[col];
      const std::vector <double> &CountItems = ROW.GetCountItems();
      double prod = CountItems[col];
      ROW.FindCountFactorysForItemsCount( _PC, col, -RequiresItemsSpeed );
      for ( int ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++ ) {
        _SummSpeeds[ItemID] += ROW.GetItemsPerSec()[ItemID];
      }
    }
    return true;
  }

  bool ProductionChainModel::SetFactory( int Row, KEY_FACTORY FactoryId )
  {
    _DataRows[Row].SetFactoryCurrent( _PC, FactoryId );
    Optimize();
    return true;
  }

  bool ProductionChainModel::SetCountFactores( int Row, double CountFactores )
  {
    _DataRows[Row].SetCountFactorys( _PC, CountFactores );
    Optimize();
    return true;
  }

  bool ProductionChainModel::SetModules( int Row, const FactoryModules & Modules )
  {
    _DataRows[Row].SetFactoryModules(_PC, Modules);
    Optimize();
    return true;
  }

  bool ProductionChainModel::SetRecipe( int Row, KEY_RECIPE RecipeId )
  {
    return false;
  }

  int ProductionChainModel::CountItems() const
  {
    return static_cast<int>( _ColsItems.size() );
  }

  int ProductionChainModel::CountRecipes() const
  {
    return static_cast<int>( _DataRows.size() );
  }

}