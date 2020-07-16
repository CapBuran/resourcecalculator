#include <assert.h>

#include "ProductionChainModel.h"
#include "ParamsCollection.h"
#include "../ResourceCalculator/ProductionChainTree.h"

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

  bool ProductionChainDataRow::_Update( )
  {
    const size_t CountsCols = _ColsItems.size();

    _Factorys.clear();

    bool IsIsFactoryOkOld = _PC->FC.GetFactory( _FactoryCurrent ).IsAllowedProduction( *_PC, _RecipeCurrent );

    KEY_ITEM InitItemKey = KEY_ITEM::ID_ITEM_NoFind_Item;

    if (CountsCols > 0 && _InitColumb >= 0) {
      InitItemKey = _ColsItems[_InitColumb];
      if (IsIsFactoryOkOld) {
        IsIsFactoryOkOld = _PC->FC.GetFactory(_FactoryCurrent).IsAllowedMining(*_PC, InitItemKey);
      }
    }

    const std::map<KEY_FACTORY, Factory> &Factorys = _PC->FC.GetFactorys();
    for ( auto & it : Factorys ) {
      bool IsFactoryOk1 = it.second.IsAllowedProduction(*_PC, _RecipeCurrent);
      bool IsFactoryOk2 = it.second.IsAllowedMining(*_PC, InitItemKey);
      if (IsFactoryOk1 && IsFactoryOk2) {
        if(!IsIsFactoryOkOld) _FactoryCurrent = it.first;
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

    const Factory &factory = _PC->FC.GetFactory( _FactoryCurrent );
    const Recipe &recipe = _PC->RC.GetData().find( _RecipeCurrent )->second;

    _CurrentFactoryName = factory.GetName();
    _CurrentRecipeName = recipe.GetName();

    factory.FixFactoryModules( _FM );
    _SecPerOneRecipe = recipe.GetTime();
    
    _SpeedFactory = factory.GetSpeed() * _FM.GetSummSpeed(_PC->MC);

    _RealTimeProductionOfOneItemPerSec = _SecPerOneRecipe / _SpeedFactory;
    double ProductionSpeedPerSecond = _FM.GetSummProductivity(_PC->MC );

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

  bool ProductionChainDataRow::Init( const ParamsCollection & PC, KEY_RECIPE RecipeId, const std::vector<KEY_ITEM>&Cols )
  {
    _PC = &PC;
    int InitColumb = -1;

    if (Cols.size() > 0) {
      const Recipe *recipe = PC.RC.GetRecipe(RecipeId);
      assert(recipe != nullptr);
      const std::set<CountsItem> &Result = recipe->GetResult();
      for (size_t i = Cols.size() - 1; i >= 0; i--) {
      //for (size_t i = 0; i < Cols.size(); i++) {
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
    _CountItems.clear();
    _ItemsPerSec.clear();
    _CountItems.resize(_ColsItems.size(), 0.0);
    _ItemsPerSec.resize(_ColsItems.size(), 0.0);
    _Update();
    return true;
  }
  
  bool ProductionChainDataRow::ReInit()
  {
    _CountItems.clear();
    _ItemsPerSec.clear();
    _CountItems.resize(_ColsItems.size(), 0.0);
    _ItemsPerSec.resize(_ColsItems.size(), 0.0);
    _Update();
    return true;
  }

  void ProductionChainDataRow::DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel )
  {
    _FM.DeleteModules( ModulesToDel );
  }

  int ProductionChainDataRow::ReadFromJson(const Json::Value & jsonPr)
  {
    KEY_FACTORY FactoryID = static_cast<KEY_FACTORY>(jsonPr["FactoryID"].asInt64());
    double CountFactroes = jsonPr["FactoresCount"].asDouble();
    FactoryModules FM;
    FM.ReadFromJson(jsonPr["FactoryModules"]);
    SetFactoryCurrent(FactoryID);
    SetCountFactorys(CountFactroes);
    SetFactoryModules(FM);
    return 0;
  }

  inline int ProductionChainDataRow::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr["FactoryID"] = static_cast<TYPE_KEY>(_FactoryCurrent);
    jsonPr["FactoresCount"] = _CountFactorys;
    _FM.WriteToJson(jsonPr["FactoryModules"]);
    return 0;
  }

  KEY_FACTORY ProductionChainDataRow::GetFactoryIdFromIndex( int Index ) const
  {
    KEY_FACTORY RetVal = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    if ( 0 <= Index && Index < _Factorys.size() ) RetVal = _Factorys[Index];
    return RetVal;
  }

  bool ProductionChainDataRow::SetCountFactorys( double Count )
  {
    bool RetVal = _CountFactorys == Count;
    if ( RetVal ) {
      return false;
    }
    _CountFactorys = Count;
    _ItemsPerSec[_InitColumb] = 0.0;
    _Update();
    return true;
  }

  bool ProductionChainDataRow::SetFactoryModules( const FactoryModules & FM )
  {
    _FM = FM;
    double OldItemsCount = _ItemsPerSec[_InitColumb];
    FindCountFactorysForItemsCount(_InitColumb, OldItemsCount);
    _Update();
    FindCountFactorysForItemsCount(_InitColumb, OldItemsCount);
    return true;
  }

  bool ProductionChainDataRow::SetFactoryCurrent( KEY_FACTORY KeyFactory )
  {
    if ( _FactoryCurrent == KeyFactory ) {
      return false;
    }
    double OldItemsCount = _ItemsPerSec[_InitColumb];
    _FactoryCurrent = KeyFactory;
    FindCountFactorysForItemsCount( _InitColumb, OldItemsCount );
    _Update();
    FindCountFactorysForItemsCount( _InitColumb, OldItemsCount );
    return true;
  }

  bool ProductionChainDataRow::FindCountFactorysForItemsCount( int Columb, double Count )
  {
    if ( _CountItems[Columb] == 0.0 ) return false;
    if ( abs( _CountFactorys ) < 0.00001 ) {
      SetCountFactorys( 1.0 );
    }
    double OldCountFactorys = _CountFactorys;
    double OldCount = _ItemsPerSec[Columb];
    if ( OldCount == Count ) return false;
    double Coeff = Count / OldCount;
    _CountFactorys = Coeff * OldCountFactorys;
    _Update();
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

  int ProductionChainModel::ReadFromJson(const Json::Value & jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    _ItemKey = static_cast<KEY_ITEM>(jsonPr["ItemKey"].asInt64());
    _SetItemKey(_ItemKey);
    if (jsonPr["Rows"].size() == _DataRows.size()){
      uint64_t i = 0;
      for (auto &it : jsonPr["Rows"]) {
        _DataRows[i++].ReadFromJson(it);
      }
    }
    Optimize();
    return 0;
  }

  inline int ProductionChainModel::WriteToJson(Json::Value & jsonPr) const
  {
    Json::Value jsonRows = Json::Value(Json::arrayValue);
    jsonPr["Rows"] = Json::Value(Json::arrayValue);
    for (auto& it : _DataRows) {
      Json::Value newVal;
      it.WriteToJson(newVal);
      jsonRows.append(newVal);
    }
    jsonPr["ItemKey"] = static_cast<KEY_TO_Json>(_ItemKey);
    jsonPr["Rows"] = jsonRows;
    return 0;
  }
  
  const std::vector<KEY_ITEM>& ProductionChainModel::GetColsItems() const
  {
    return _ColsItems;
  }

  const std::vector<double> ProductionChainModel::GetSummSpeeds() const
  {
    return _SummSpeeds;
  }

  ProductionChainModel::ProductionChainModel(FullItemTree& tree)
    : _tree(tree)
    , _ItemKey(KEY_ITEM::ID_ITEM_NoFind_Item)
    , _PC(tree.GetPC())
  {
  }

  KEY_ITEM ProductionChainModel::GetItemKey() const
  {
    return _ItemKey;
  }

  ProductionChainModel::ProductionChainModel(FullItemTree& tree, KEY_ITEM ItemKey)
    : _tree(tree)
    , _ItemKey( ItemKey )
    , _PC(tree.GetPC())
  {
    _SetItemKey( ItemKey );
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

  struct ItemIndex
  {
    KEY_ITEM key;
    int index;
  };

  struct RecipeIndex
  {
    KEY_RECIPE key;
    int index;
  };

  void RecursiveSurvey(
    int treeCounter,
    int treeMaxCounter,
    const FullItemTree& tree,
    KEY_ITEM key_item,
    std::set<KEY_ITEM>& keys_items_set,
    std::set<KEY_ITEM>& keys_items_no_childrens_set,
    std::set<KEY_RECIPE>& keys_recipes_set,
    std::list<ItemIndex>& keys_items_list,
    std::list<RecipeIndex>& keys_recipes_list
  )
  {
    treeCounter++;

    if (treeCounter < treeMaxCounter)
    {
      const ItemResultTree& tree_item = tree.GetRootItemTree(key_item);
      const std::vector<KEY_RECIPE>& recipesIds = tree_item.Childrens;
      if (recipesIds.empty())
      {
        keys_items_no_childrens_set.insert(key_item);
        if (keys_items_set.count(key_item) == 0)
        {
          keys_items_set.insert(key_item);
          keys_items_list.push_back({ key_item, 0 });
        }
      }
      else
      {
        if (treeCounter == 1 && keys_items_set.count(key_item) == 0)
        {
          keys_items_set.insert(key_item);
          keys_items_list.push_back({ key_item, 0 });
        }
        for (KEY_RECIPE k_recipe : recipesIds)
        {
          if (keys_recipes_set.count(k_recipe) == 0)
          {
            keys_recipes_set.insert(k_recipe);
            keys_recipes_list.push_front({ k_recipe, treeCounter });
            const RecipeResultTree& tree_recipe = tree.GetRootRecipeTree(k_recipe);
            const std::vector<KEY_ITEM>& recipesIds = tree_recipe.Childrens;
            for (KEY_ITEM k_item : recipesIds)
            {
              if (keys_items_set.count(k_item) == 0)
              {
                keys_items_set.insert(k_item);
                keys_items_list.push_back({ k_item, treeCounter });
                RecursiveSurvey(treeCounter, treeMaxCounter, tree, k_item, keys_items_set, keys_items_no_childrens_set, keys_recipes_set, keys_items_list, keys_recipes_list);
              }
            }
          }
        }
      }
    }
  }

  bool ProductionChainModel::_SetItemKey( KEY_ITEM ItemKey )
  {
    int MaxRecursive = 100;

    _ItemKey = ItemKey;

    const ItemResultTree& tree = _tree.GetRootItemTree(_ItemKey);

    std::set<KEY_ITEM> keys_items_set;
    std::set<KEY_RECIPE> keys_recipes_set;

    std::list<ItemIndex> keys_items_list;
    std::list<RecipeIndex> keys_recipes_list;

    std::set<KEY_ITEM> keys_items_no_childrens_set;

    int treeCounter = 0;

    RecursiveSurvey(treeCounter, MaxRecursive, _tree, _ItemKey, keys_items_set, keys_items_no_childrens_set, keys_recipes_set, keys_items_list, keys_recipes_list);

    _ColsItems.resize(keys_items_set.size());
    _DataRows.resize(keys_recipes_set.size());
    _SummSpeeds.resize(_ColsItems.size());

    {
      size_t posColItems = _ColsItems.size();
      for (int i = 0; i < MaxRecursive; i++)
      {
        bool found = false;
        for (const auto& it : keys_items_list)
        {
          if (it.index == i && keys_items_no_childrens_set.count(it.key) == 0)
          {
            _ColsItems[--posColItems] = it.key;
            found = true;
          }
        }
        if (!found) break;
      }
      for (KEY_ITEM key : keys_items_no_childrens_set)
      {
        _ColsItems[--posColItems] = key;
      }
    }

    {
      int CounterRecipes = 0;
      for (const auto& r: keys_recipes_list)
      {
        _DataRows[CounterRecipes++].Init(_PC, r.key, _ColsItems);
      }
    }
    
    return ReInit();
  }

  bool ProductionChainModel::ReInit()
  {
    for (ProductionChainDataRow &PCDR : _DataRows) {
      PCDR.ReInit();
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
    if (CountCols == 0 || CountRows == 0) return false;
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
      ROW.FindCountFactorysForItemsCount( col, -RequiresItemsSpeed );
      for ( int ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++ ) {
        _SummSpeeds[ItemID] += ROW.GetItemsPerSec()[ItemID];
      }
    }
    return true;
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

  bool ProductionChainModel::SetModules( int Row, const FactoryModules & Modules )
  {
    _DataRows[Row].SetFactoryModules(Modules);
    Optimize();
    return true;
  }

  int ProductionChainModel::CountItems() const
  {
    return static_cast<int>( _ColsItems.size() );
  }

  int ProductionChainModel::CountRecipes() const
  {
    return static_cast<int>(_DataRows.size());
  }

}