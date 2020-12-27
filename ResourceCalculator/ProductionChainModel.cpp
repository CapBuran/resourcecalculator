#include <assert.h>

#include "ProductionChainModel.h"
#include "ParamsCollection.h"
#include "../ResourceCalculator/ProductionChainTree.h"

namespace ResourceCalculator
{
  ProductionChainDataRow::ProductionChainDataRow(
    const ParamsCollection& PC,
    KEY_RECIPE keyRecipe,
    TYPE_KEY initColumb,
    const std::vector<KEY_ITEM>& cols,
    bool isEnabled
  )
    : _SecPerOneRecipe(.0)
    , _RealTimeProductionOfOneItemPerSec(.0)
    , _CountFactorys(.0)
    , InitColumb(initColumb >= 0 ? initColumb : 0)
    , IsEnabled(isEnabled)
    , RecipeCurrent(PC.RC[keyRecipe])
  {
    assert(PC.RC[keyRecipe]);

    _ColsItems = cols;
    _FactoryCurrent = PC.FC[KEY_FACTORY::ID_ITEM_NoFind_Factory];
    _ItemsCount.resize(_ColsItems.size(), .0);
    _ItemsPerSec.resize(_ColsItems.size(), 0.0);

    _SecPerOneRecipe = RecipeCurrent.GetTime();

    if (cols.size() > 0)
    {
      const std::set<CountsItem>& Result = RecipeCurrent.GetResult();
      const std::set<CountsItem>& Required = RecipeCurrent.GetRequired();

      for (size_t i = 0; i < cols.size(); i++) {
        for (CountsItem it: Result)
          if (_ColsItems[i] == it.ItemId)
            _ItemsCount[i] += it.Count;
        for (CountsItem it: Required)
          if (_ColsItems[i] == it.ItemId)
            _ItemsCount[i] -= it.Count;
      }
    }

    if (IsEnabled)
    {
      _Factorys = PC.FC.GetByConditions(std::bind(FactoryIsAllowed, std::placeholders::_1, std::ref(RecipeCurrent)));
      if (!_Factorys.empty()) _FactoryCurrent = _Factorys.begin()->second;
    }
    else
    {
      _SecPerOneRecipe = .0;
      _RealTimeProductionOfOneItemPerSec = .0;
      _CountFactorys = .0;
      _FactoryCurrent.SetSpeed(.0);
      _FactoryCurrent.SetName("None");
      _FactoryCurrent.SetPower(.0);
      _FM.SetCountModules(0);
    }
  }
  
  bool ProductionChainDataRow::SetCountFactorys( double Count )
  {
    if ( _CountFactorys == Count ) return false;
    if (!IsEnabled) return false;
    _RealTimeProductionOfOneItemPerSec = Count / _SecPerOneRecipe;
    _CountFactorys = Count;
    for (size_t i = 0; i < _ItemsPerSec.size(); i++)
    {
      _ItemsPerSec[i] = _ItemsCount[i] * _RealTimeProductionOfOneItemPerSec;
    }
    return true;
  }

  bool ProductionChainDataRow::SetFactoryModules( FactoryModules FM )
  {
    const double OldSpeed = _FactoryCurrent.GetSpeed();
    _FM = FM;
    _FactoryCurrent.FixFactoryModules(_FM);
    const double NewSpeed = _FactoryCurrent.GetSpeed();
    _CountFactorys = _CountFactorys * NewSpeed / OldSpeed;
    return true;
  }

  bool ProductionChainDataRow::SetFactory( KEY_FACTORY KeyFactory )
  {
    if (!_Factorys.count(KeyFactory)) return false;

    double OldSpeed = _FactoryCurrent.GetSpeed();
    OldSpeed = (std::abs(OldSpeed) < 0.0000001) ? OldSpeed : 1.0;

    _FactoryCurrent = _Factorys[KeyFactory];
    _FactoryCurrent.FixFactoryModules(_FM);

    const double NewSpeed = _FactoryCurrent.GetSpeed();

    _CountFactorys = _CountFactorys * OldSpeed / NewSpeed;

    return true;
  }

  bool ProductionChainDataRow::FindCountFactorysForItemsCount(TYPE_KEY Columb, double Count )
  {
    if ( abs( _ItemsCount[Columb] ) < 1E-100) return false;
    if ( abs( _CountFactorys ) < 0.00001 ) {
      SetCountFactorys( 1.0 );
    }
    double OldCountFactorys = _CountFactorys;
    double OldCount = _ItemsPerSec[Columb];
    if ( OldCount == Count ) return false;
    double Coeff = Count / OldCount;
    SetCountFactorys(Coeff * OldCountFactorys);
    return true;
  }

  const std::vector<double> ProductionChainModel::GetSummSpeeds() const
  {
    return _SummSpeeds;
  }

  ProductionChainModel::ProductionChainModel(const FullItemTree& tree)
    : _Tree(tree)
  {
  }

  ProductionChainModel::ProductionChainModel(const FullItemTree& tree, KEY_ITEM ItemKey, int nesting)
    : _Tree(tree)
  {
    SetItemKey(ItemKey, nesting);
  }

  struct ItemIndex
  {
    TYPE_KEY index;
    KEY_ITEM key;
  };

  struct RecipeIndex
  {
    TYPE_KEY index;
    KEY_RECIPE key;
    KEY_ITEM key_find;
  };

  void RecursiveSurvey(
    TYPE_KEY treeCounter,
    TYPE_KEY treeMaxCounter,
    TYPE_KEY treeMaxEnabledCounter,
    const FullItemTree& tree,
    KEY_ITEM key_item,
    std::set<KEY_ITEM>& keys_items_set,
    std::set<KEY_ITEM>& keys_items_no_childrens_set,
    std::set<KEY_RECIPE>& keys_recipes_set,
    std::set<KEY_RECIPE>& deny_keys_recipes_set,
    std::list<ItemIndex>& keys_items_list,
    std::list<RecipeIndex>& keys_recipes_list
  )
  {
    treeCounter++;

    if (treeCounter < treeMaxCounter)
    {
      const ItemNode& treeItem = tree[key_item];
      if (!treeItem) return;
      const std::vector<KEY_RECIPE>& recipesIds = treeItem.Childrens;
      if (recipesIds.empty())
      {
        keys_items_no_childrens_set.insert(key_item);
        if (keys_items_set.count(key_item) == 0)
        {
          keys_items_set.insert(key_item);
          keys_items_list.push_back({ 0, key_item });
        }
      }
      else
      {
        if (treeCounter == 1 && keys_items_set.count(key_item) == 0)
        {
          keys_items_set.insert(key_item);
          keys_items_list.push_front({ 0, key_item });
        }
        for (KEY_RECIPE k_recipe : recipesIds)
        {
          if (keys_recipes_set.count(k_recipe) == 0)
          {
            keys_recipes_set.insert(k_recipe);
            keys_recipes_list.push_front({ treeCounter, k_recipe, key_item });
            if (treeCounter > treeMaxEnabledCounter)
            {
              deny_keys_recipes_set.insert(k_recipe);
            }
            if (deny_keys_recipes_set.count(k_recipe) == 0)
            {
              const RecipeNode& treeRecipe = tree[k_recipe];
              if (!treeRecipe) return;
              const std::vector<KEY_ITEM>& recipesIds = treeRecipe.Childrens;
              for (KEY_ITEM k_item : recipesIds)
              {
                if (keys_items_set.count(k_item) == 0)
                {
                  keys_items_set.insert(k_item);
                  keys_items_list.push_front({ treeCounter, k_item });
                  RecursiveSurvey(treeCounter, treeMaxCounter, treeMaxEnabledCounter, tree, k_item, keys_items_set, keys_items_no_childrens_set, keys_recipes_set, deny_keys_recipes_set, keys_items_list, keys_recipes_list);
                }
              }
            }
          }
        }
      }
    }
  }

  void ProductionChainModel::Rebuild(KEY_ITEM itemKey, int nesting)
  {
    if (nesting < 0) nesting = 2;

    Nesting_ = nesting;

    std::map<KEY_RECIPE, double> OldCountFactorys;
    std::map<KEY_RECIPE, KEY_FACTORY> OldFactorys;

    for (const auto& it: _DataRows)
    {
      if (!it) continue;
      KEY_RECIPE recipeKey = it->RecipeCurrent.GetKey();
      OldCountFactorys[recipeKey] = it->CountFactorys();
      OldFactorys[recipeKey] = it->GetCurrentFactory().GetKey();
    }

    _Key = itemKey;
    _DataRows.clear();
    _SummSpeeds.clear();
    _ItemsNames.clear();
    TYPE_KEY MaxRecursive = 100;  nesting < 0 ? 100 : nesting;
    TYPE_KEY treeCounter = 0;

    std::set<KEY_ITEM> keys_items_set;
    std::set<KEY_RECIPE> keys_recipes_set;

    std::list<ItemIndex> keys_items_list;
    std::list<RecipeIndex> keys_recipes_list;

    std::set<KEY_ITEM> keys_items_no_childrens_set;

    RecursiveSurvey(treeCounter, MaxRecursive, Nesting_, _Tree, _Key, keys_items_set, keys_items_no_childrens_set, keys_recipes_set, _DenyKeysRecipes, keys_items_list, keys_recipes_list);

    std::vector<KEY_ITEM> ColsItems;
    ColsItems.resize(keys_items_set.size());
    _DataRows.resize(keys_recipes_set.size());
    _SummSpeeds.resize(ColsItems.size());
    _ItemsNames.resize(ColsItems.size());

    std::map<KEY_ITEM, TYPE_KEY> InitCounts;

    {
      TYPE_KEY Counter = 0;
      
      for (KEY_ITEM ki: keys_items_no_childrens_set)
      {
        InitCounts[ki] = Counter++;
      }
      for (const ItemIndex& ki : keys_items_list)
      {
        if (InitCounts.count(ki.key) == 0)
        {
          InitCounts[ki.key] = Counter++;
        }
      }
      for (const auto& m : InitCounts)
      {
        ColsItems[m.second] = m.first;
      }
      for (TYPE_KEY i = 0; i < Counter; i++)
      {
        _ItemsNames[i] = _Tree.GetPC().IC[ColsItems[i]].GetName();
      }
    }

    TYPE_KEY LevelCounterMax = 0;
    for (const RecipeIndex& ki : keys_recipes_list)
    {
      if (ki.index > LevelCounterMax) LevelCounterMax = ki.index;
    }

    for (TYPE_KEY level = LevelCounterMax, CounterRecipes = 0; level > 0; level--)
    {
      for (const RecipeIndex& ki : keys_recipes_list)
      {
        if (ki.index != level) continue;
        if (_DenyKeysRecipes.count(ki.key) > 0)
        {
          _DataRows[CounterRecipes] = std::make_unique<ProductionChainDataRow>(_Tree.GetPC(), ki.key, InitCounts[ki.key_find], ColsItems, false);
        }
        else
        {
          _DataRows[CounterRecipes] = std::make_unique<ProductionChainDataRow>(_Tree.GetPC(), ki.key, InitCounts[ki.key_find], ColsItems, true);
          if (OldFactorys.count(ki.key))
          {
            _DataRows[CounterRecipes]->SetFactory(OldFactorys[ki.key]);
            _DataRows[CounterRecipes]->SetCountFactorys(OldCountFactorys[ki.key]);
          }
        }
        for (TYPE_KEY ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++) {
          _SummSpeeds[ItemID] += _DataRows[CounterRecipes]->ItemPerSec(ItemID);
        }
        CounterRecipes++;
      }
    }
  }

  bool ProductionChainModel::SetItemKey(KEY_ITEM ItemKey, int nesting)
  {
    Rebuild(ItemKey, nesting);
    return true;
  }

  bool ProductionChainModel::EnableRecipes(TYPE_KEY row)
  {
    if (row < _DataRows.size())
    {
      KEY_RECIPE recipeKey = _DataRows[row]->RecipeCurrent.GetKey();
      if (_DenyKeysRecipes.count(recipeKey) == 0)
        _DenyKeysRecipes.insert(recipeKey);
      else
        _DenyKeysRecipes.erase(recipeKey);
    }

    Rebuild(_Key, 100);

    return true;
  }

  bool ProductionChainModel::UpdateAll(const ParamsCollection& PC)
  {
    Rebuild(_Key, 100);
    return true;
  }

  bool ProductionChainModel::CalculateSumm()
  {
    for (auto& col : _SummSpeeds)
    {
      col = .0;
    }

    for (const auto& row: _DataRows)
    {
      for (TYPE_KEY ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++) {
        _SummSpeeds[ItemID] += row->ItemPerSec(ItemID);
      }
    }

    return true;
  }

  const ProductionChainDataRow& ProductionChainModel::GetRow(TYPE_KEY row) const
  {
    return *_DataRows[row];
  }

  ProductionChainDataRow& ProductionChainModel::GetRow(TYPE_KEY row)
  {
    return *_DataRows[row];
  }

  std::string ProductionChainModel::GetItemName(TYPE_KEY col) const
  {
    return _ItemsNames[col];
  }

  bool ProductionChainModel::FitQuantity()
  {
    if (_DataRows.size() == 0 || _SummSpeeds.size() == 0) return false;

    for (TYPE_KEY i = 0; i < _SummSpeeds.size(); i++)
    {
      _SummSpeeds[i] = .0;
    }

    for (auto& it = _DataRows.rbegin(); it != _DataRows.rend(); it++)
    {
      ProductionChainDataRow& ROW = **it;
      if (it == _DataRows.rbegin())
      {
        for (int ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++) {
          _SummSpeeds[ItemID] += ROW.ItemPerSec(ItemID);
        }
      }
      else
      {
        ROW.FindCountFactorysForItemsCount(ROW.InitColumb, -_SummSpeeds[ROW.InitColumb]);
        for (int ItemID = 0; ItemID < _SummSpeeds.size(); ItemID++) {
          _SummSpeeds[ItemID] += ROW.ItemPerSec(ItemID);
        }
      }
    }

    return true;
  }


  TYPE_KEY ProductionChainModel::CountItems() const
  {
    return static_cast<TYPE_KEY>(_SummSpeeds.size());
  }

  TYPE_KEY ProductionChainModel::CountRecipes() const
  {
    return static_cast<TYPE_KEY>(_DataRows.size());
  }

  int ProductionChainModel::ReadFromJson(const Json::Value& jsonPr)
  {
    ItemBase::ReadFromJson(jsonPr);
    //_SetItemKey(_ItemKey);
    //if (jsonPr["Rows"].size() == _DataRows.size()){
    //  uint64_t i = 0;
    //  for (auto &it : jsonPr["Rows"]) {
    //    _DataRows[i++].ReadFromJson(it);
    //  }
    //}
    //Optimize();
    return 0;
  }

  int ProductionChainModel::WriteToJson(Json::Value& jsonPr) const
  {
    ItemBase::WriteToJson(jsonPr);
    //Json::Value jsonRows = Json::Value(Json::arrayValue);
    //jsonPr["Rows"] = Json::Value(Json::arrayValue);
    //for (auto& it : _DataRows) {
    //  Json::Value newVal;
    //  it.WriteToJson(newVal);
    //  jsonRows.append(newVal);
    //}
    //jsonPr["Rows"] = jsonRows;
    return 0;
  }

  int ProductionChainDataRow::ReadFromJson(const Json::Value& jsonPr)
  {
    //KEY_FACTORY FactoryID = static_cast<KEY_FACTORY>(jsonPr["FactoryID"].asInt64());
    //double CountFactroes = jsonPr["FactoresCount"].asDouble();
    //FactoryModules FM;
    //FM.ReadFromJson(jsonPr["FactoryModules"]);
    //SetFactoryCurrent(FactoryID);
    //SetCountFactorys(CountFactroes);
    //SetFactoryModules(FM);
    return 0;
  }

  int ProductionChainDataRow::WriteToJson(Json::Value& jsonPr) const
  {
    //jsonPr["FactoryID"] = static_cast<TYPE_KEY>(_FactoryCurrent);
    //jsonPr["FactoresCount"] = _CountFactorys;
    //_FM.WriteToJson(jsonPr["FactoryModules"]);
    return 0;
  }
}
