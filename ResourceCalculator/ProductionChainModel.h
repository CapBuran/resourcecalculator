
#include "ParamsCollection.h"


namespace ResourceCalculator {

  class ProductionChainModel;

  class ProductionChainDataRow {
    KEY_RECIPE  RecipeCurrent;
    KEY_FACTORY FactoryCurrent;

    double _PeakPower;
    double _LevelOfPollution;
    double _SpeedFactory;
    double _SecPerOneRecipe;
    double _ProductionSpeedPerSecond;
    double _CountFactorys;

    std::vector <KEY_RECIPE>  _Recipes;
    std::vector <KEY_FACTORY> _Factorys;
    FactoryModules _FM;

    std::vector <double> _CountItems;
    std::vector <double> _ItemsPerSec;

  public:
    
    bool Build(const ParamsCollection &PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const FactoryModules& FM, int CountFactorys);
    
    friend ProductionChainModel;

  };

  class ProductionChainModel : public ItemBase
  {
  public:
    ProductionChainModel(const ParamsCollection &PC, KEY_ITEM ItemKey);
    ProductionChainModel(const ParamsCollection &PC);
    ~ProductionChainModel();
    
    //Возвращают истину, когда нужно обновить всю модель
    bool SetItemKey(KEY_ITEM ItemKey);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetFactory(int Row, KEY_FACTORY FactoryId);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetModules(int Row, const std::vector < KEY_MODULE > & Modules );

    //Возвращают истину, когда нужно обновить всю модель
    bool SetRecipe(int Row, KEY_RECIPE RecipeId);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetAnsfer(std::map<KEY_ITEM, KEY_RECIPE> &AnsferRecipeKey, int row = -1);

    //Возвращают истину, когда нужно обновить всю модель
    bool Rebuild();

    int CountRequired() const;
    int CountResult() const;

    const ProductionChainDataRow &GetRow(int Row) const;

  private:
    KEY_ITEM _ItemKey;
    const ParamsCollection &_PC;

    std::map<KEY_ITEM, KEY_RECIPE> _AnsferRecipeKey;//TODO edit From interface

    std::vector <ProductionChainDataRow>  _DataRows;

  };



}