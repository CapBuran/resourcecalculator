#include <assert.h>

#include "ProductionChainModel.h"

namespace ResourceCalculator
{
  bool ProductionChainDataRow::Init( const ParamsCollection & PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM>&Cols )
  {
    _PC = &PC;

    bool RetVal = _RecipeCurrent == RecipeId && _FactoryCurrent == FactoryId;

    if ( RetVal ) {
      return false;
    }

    _RecipeCurrent = RecipeId;
    _FactoryCurrent = FactoryId;

    const bool IsFactoryOk = PC.FC.GetFactory( FactoryId ).IsAllowedProduction( PC, RecipeId );

    const size_t CountsCols = Cols.size();

    _Factorys.clear();
    _CountItems.clear();
    _ItemsPerSec.clear();
    _ColItems.clear();

    const std::map<KEY_FACTORY, Factory> &Factorys = PC.FC.GetFactorys();
    for ( auto & it : Factorys ) {
      if ( it.second.IsAllowedProduction( PC, RecipeId ) ) {
        if ( !IsFactoryOk ) {
          _FactoryCurrent = it.first;
        }
        _Factorys.push_back( it.first );
      }
    }

    const Factory &factory = PC.FC.GetFactory( _FactoryCurrent );
    const Recipe &recipe = PC.RC.GetData().find( _RecipeCurrent )->second;

    factory.FixFactoryModules( _FM );

    _SecPerOneRecipe = recipe.GetTime();
    _SpeedFactory = factory.GetSpeed() * _FM.GetSummSpeed( PC.MC );
    _RealTimeProductionOfOneItemPerSec = _SecPerOneRecipe / _SpeedFactory;

    double ProductionSpeedPerSecond = _FM.GetSummProductivity( PC.MC );

    _PeakPower = 0.0;
    _LevelOfPollution = 0.0;
    _CountFactorys = 0.0;

    _CountItems.resize( CountsCols, 0.0 );
    _ItemsPerSec.resize( CountsCols, 0.0 );

    _ColItems = Cols;

    for ( size_t ColId = 0; ColId < CountsCols; ColId++ ) {
      const KEY_ITEM ItemKey = _ColItems[ColId];
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

    return true;
  }

  KEY_FACTORY ProductionChainDataRow::GetFactoryIdFromIndex( int Index ) const
  {
    return _Factorys[Index];
  }

  bool ProductionChainDataRow::SetCountFactorys( double Count )
  {
    bool RetVal = _CountFactorys == Count;
    if ( RetVal ) {
      return false;
    }
    _CountFactorys = Count;
    const size_t CountsCols = _CountItems.size();
    for ( size_t ColId = 0; ColId < CountsCols; ColId++ ) {
      _ItemsPerSec[ColId] = _CountItems[ColId] / _RealTimeProductionOfOneItemPerSec * _CountFactorys;
    }
    return RetVal;
  }

  bool ProductionChainDataRow::SetFactoryModules( const FactoryModules & FM )
  {

    _FM = FM;
    const Factory &Factory = _PC->FC.GetFactory( _FactoryCurrent );
      
    Factory.FixFactoryModules( _FM );

    const size_t CountsCols = _CountItems.size();

    const Recipe &recipe = _PC->RC.GetData().find( _RecipeCurrent )->second;

    double ProductionSpeedPerSecond = _FM.GetSummProductivity( _PC->MC );

    _SpeedFactory = Factory.GetSpeed();
    _RealTimeProductionOfOneItemPerSec = _SecPerOneRecipe / _SpeedFactory;

    for ( size_t ColId = 0; ColId < CountsCols; ColId++ ) {
      const KEY_ITEM ItemKey = _ColItems[ColId];
      _CountItems[ColId] = 0.0;
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
      _ItemsPerSec[ColId] = _CountItems[ColId] / _RealTimeProductionOfOneItemPerSec * _CountFactorys;
    }
    return false;
  }

  bool ProductionChainDataRow::SetFactoryCurrent( KEY_FACTORY KeyFactory)
  {
    if ( _FactoryCurrent == KeyFactory ) {
      return false;
    }
    _FactoryCurrent = KeyFactory;
    
    SetFactoryModules( _FM );

    return true;
  }

  bool ProductionChainDataRow::FindCountFactorysForItemsCount( int Columb, double Count )
  {
    if ( _CountItems[Columb] == 0.0 ) return false;
    if ( abs(_CountFactorys) < 0.001 ) {
      SetCountFactorys( 1.0 );
    }
    double OldCountFactorys = _CountFactorys;
    double OldCount = _ItemsPerSec[Columb];
    double Coeff = Count / OldCount;
    return SetCountFactorys( Coeff * OldCountFactorys );
  }

  FactoryModules ProductionChainDataRow::_GetFactoryModules() const
  {
    return _FM;
  }

  const ProductionChainDataRow& ProductionChainModel::GetRow( int Row ) const
  {
    return _DataRows[Row];
  }

  ProductionChainDataRow &ProductionChainModel::GetRowEdit( int Row )
  {
    return _DataRows[Row];
  }

  const std::vector<double> ProductionChainModel::GetSummSpeeds() const
  {
    return _SummSpeeds;
  }

  double ProductionChainDataRow::GetSummProductivity() const
  {
    return 0.0;
  }

  double ProductionChainDataRow::GetSummSpeed() const
  {
    return 0.0;
  }

  std::string ProductionChainDataRow::GetCurrentFactoryName() const
  {


    return _PC->FC.GetFactory( _FactoryCurrent ).GetName();
  }

  std::string ProductionChainDataRow::GetCurrentRecipeName() const
  {
    return _PC->RC.GetRecipe( _RecipeCurrent )->GetName();
  }


  bool ProductionChainModel::Optimize()
  {
    //for ( auto & it :_DataRows) {
    //  it._SetCountFactorys( 1 );
    //}
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

  bool ProductionChainModel::SetResultItemCount( double Count )
  {
    const size_t CounRecipes = _DataRows.size();




    return true;
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

  bool ProductionChainModel::SetItemKey( KEY_ITEM ItemKey )
  {
    std::list<KEY_ITEM> ListRequest;
    std::list<KEY_ITEM> ListRequestResourceOnly;

    ItemResultTree IRT = _PC.RC.BuildTree( ItemKey, 8, ListRequest, ListRequestResourceOnly );

    std::list <KEY_RECIPE> ResultRecipes;
    std::list <KEY_ITEM> ResultItems;
    std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

    _PC.RC.Travelling( IRT, Ansfer, ResultRecipes, ResultItems );

    const Item &RootItem = _PC.IC.GetData().find( ItemKey )->second;

    _Name = "Production Chain " + RootItem.GetName();

    _ItemKey = ItemKey;

    const size_t CountsItems = ResultItems.size();
    const size_t CountsRecipes = ResultRecipes.size();

    _ColsItems.clear();
    _ColsItems.resize( CountsItems );

   /* bool IsCoolStructFind = ResultRecipes.size() == ListRequestResourceOnly.size();*/

    if ( ListRequest.size() == CountsRecipes && ListRequestResourceOnly.size() + ListRequest.size() == CountsItems ) {
      std::copy( ListRequestResourceOnly.begin(), ListRequestResourceOnly.end(), _ColsItems.begin() );
      std::copy( ListRequest.begin(), ListRequest.end(), &_ColsItems[ListRequestResourceOnly.size()] );
    } else {
      std::copy( ResultItems.begin(), ResultItems.end(), _ColsItems.begin() );
    }

    _DataRows.clear();
    _DataRows.resize( CountsRecipes );

    auto IT_Recipe = ResultRecipes.begin();

    for ( size_t RecipeIdx = 0; RecipeIdx < CountsRecipes; RecipeIdx++, IT_Recipe++ ) {
      KEY_RECIPE RecipeId = *IT_Recipe;
      _DataRows[RecipeIdx].Init( _PC, RecipeId, KEY_FACTORY::ID_ITEM_NoFind_Factory, _ColsItems );
    }

    _SummSpeeds.clear();
    _SummSpeeds.resize( CountsItems, 0.0 );
    
    return Optimize();

  }

  std::string ProductionChainModel::GetItemName( int Col ) const
  {
    return _PC.IC.GetItem( _ColsItems[Col] )->GetName();
  }

  bool ProductionChainModel::SetFactory( int Row, KEY_FACTORY FactoryId )
  {
    _DataRows[Row].SetFactoryCurrent( FactoryId );
    Optimize();
    return true;
  }

  bool ProductionChainModel::SetCountFactores( int Row, double CountFactores )
  {
    _DataRows[Row].SetCountFactorys( CountFactores );
    Optimize();
    return true;
  }
  
  bool ProductionChainModel::SetModules( int Row, const std::vector<KEY_MODULE>& Modules )
  {
    return false;
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