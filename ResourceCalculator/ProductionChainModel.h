#pragma once

#include <memory>

#include <Module.h>
#include <ProductionChainTree.h>

namespace ResourceCalculator {

class ProductionChainDataRow: public Jsonable
{
public:
  ProductionChainDataRow(
    const ParamsCollection& PC,
    KEY_RECIPE keyRecipe,
    TYPE_KEY initColumb,
    const std::vector<KEY_ITEM>& cols,
    bool isEnabled
  );

  const bool IsEnabled;
  const Recipe RecipeCurrent;
  const TYPE_KEY InitColumb;

  bool SetFactory(KEY_FACTORY type);
  const Factory& GetCurrentFactory() const { return _FactoryCurrent; }
  const std::map<KEY_FACTORY, Factory>& GetFactores() const { return _Factorys; }

  bool SetFactoryModules(FactoryModules FM);
  const FactoryModules& FM() const { return _FM; }

  double CountFactorys() const { return _CountFactorys; }
  bool SetCountFactorys(double count);

  double ItemCount(TYPE_KEY columb) const { return _ItemsCount[columb]; }
  double ItemPerSec(TYPE_KEY columb) const { return _ItemsPerSec[columb]; }
  KEY_ITEM ItemType(TYPE_KEY columb) const { return _ColsItems[columb]; }

  double GetSummProductivity(const ModuleCollection& MC) const { return _FM.GetSummProductivity(MC); }
  double GetSummSpeed(const ModuleCollection& MC) const { return _FM.GetSummSpeed(MC); }
  double SecPerOneRecipe() const { return _SecPerOneRecipe; }
  double RealTimeProductionOfOneItemPerSec() const { return _RealTimeProductionOfOneItemPerSec; }

  bool FindCountFactorysForItemsCount(TYPE_KEY columb, double count);

  int ReadFromJson(const Json::Value & jsonPr) override;
  int WriteToJson(Json::Value & jsonPr) const override;
private:

  double _SecPerOneRecipe;
  double _RealTimeProductionOfOneItemPerSec;
  double _CountFactorys;

  Factory _FactoryCurrent;
  
  std::map<KEY_FACTORY, Factory> _Factorys;
  std::vector<KEY_ITEM> _ColsItems;
  std::vector <double> _ItemsCount;
  std::vector <double> _ItemsPerSec;

  FactoryModules _FM;
};

class ProductionChainModel: public ItemBase<KEY_ITEM>
{
public:
  ProductionChainModel(const FullItemTree& tree, KEY_ITEM ItemKey, int nesting = -1);
  ProductionChainModel(const FullItemTree& tree);

  const ParamsCollection& GetPC() const { return _Tree.GetPC(); }

  bool SetItemKey(KEY_ITEM itemKey, int nesting = -1);
  bool EnableRecipes(TYPE_KEY row);
  bool FitQuantity();
  bool UpdateAll(const ParamsCollection& PC);
  bool CalculateSumm();

  TYPE_KEY CountItems() const;
  TYPE_KEY CountRecipes() const;
  const std::vector<double> GetSummSpeeds() const;
  const ProductionChainDataRow& GetRow(TYPE_KEY row) const;
  ProductionChainDataRow& GetRow(TYPE_KEY row);
  std::string GetItemName(TYPE_KEY col) const;

  int ReadFromJson(const Json::Value& jsonPr) override;
  int WriteToJson(Json::Value& jsonPr) const override;
private:
  void Rebuild(KEY_ITEM itemKey, int nesting);

  int Nesting_ = -1;
  std::vector<std::unique_ptr<ProductionChainDataRow> > _DataRows;
  std::vector<double> _SummSpeeds;
  std::vector <std::string> _ItemsNames;
  std::set<KEY_RECIPE> _DenyKeysRecipes;
  const FullItemTree& _Tree;
};

}
