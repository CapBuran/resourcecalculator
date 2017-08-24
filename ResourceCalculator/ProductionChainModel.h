#ifndef ProductionChainModel_H
#define ProductionChainModel_H

#include "ParamsCollection.h"

namespace ResourceCalculator {

  class ProductionChainModel;

  class ProductionChainDataRow {

    const ParamsCollection *_PC;

    KEY_RECIPE  _RecipeCurrent;
    KEY_FACTORY _FactoryCurrent;

    double _PeakPower;
    double _LevelOfPollution;
    double _SpeedFactory;
    double _SecPerOneRecipe;
    double _RealTimeProductionOfOneItemPerSec;
    double _CountFactorys;

    std::vector <KEY_FACTORY> _Factorys;
    std::vector <KEY_ITEM>    _ColItems;
    FactoryModules _FM;

    std::vector <double> _CountItems;
    std::vector <double> _ItemsPerSec;

    bool _Init(const ParamsCollection &PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM> &Cols);

    bool _SetCountFactorys(double Count);
    bool _SetFactoryModules(const FactoryModules &FM);

    FactoryModules _GetFactoryModules() const;

   public:

    DeclarePropertyReadOnly( CurrentFactoryName, std::string )
    DeclarePropertyReadOnly( CurrentRecipeName, std::string )
    DeclarePropertyReadOnly( SummProductivity, double )
    DeclarePropertyReadOnly( SummSpeed, double )
    DeclareAndDefinitionPropertyReadOnly( SpeedFactory, double )
    DeclareAndDefinitionPropertyReadOnly( SecPerOneRecipe, double )
    DeclareAndDefinitionPropertyReadOnly( RealTimeProductionOfOneItemPerSec, double )
    DeclareAndDefinitionPropertyReadOnly( CountFactorys, double )
    DeclareAndDefinitionRefReadOnly( CountItems, std::vector <double> )
    DeclareAndDefinitionRefReadOnly( ItemsPerSec, std::vector <double> )

    friend ProductionChainModel;
    //friend std::vector <ProductionChainModel>;

  };

  class ProductionChainModel : public ItemBase
  {
  private:
    KEY_ITEM _ItemKey;
    const ParamsCollection &_PC;

    std::vector<KEY_ITEM> _ColsItems;
    std::vector <ProductionChainDataRow>  _DataRows;
    std::vector<double> _SummSpeeds;

  public:
    ProductionChainModel(const ParamsCollection &PC, KEY_ITEM ItemKey);
    ProductionChainModel(const ParamsCollection &PC);
    ~ProductionChainModel();
    
    //Возвращают истину, когда нужно обновить всю модель
    bool SetItemKey(KEY_ITEM ItemKey);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetRecipe(int Row, KEY_RECIPE RecipeId);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetFactory(int Row, KEY_FACTORY FactoryId);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetModules(int Row, const std::vector < KEY_MODULE > & Modules );

    int CountItems() const;

    int CountRecipes() const;

    const ProductionChainDataRow &GetRow(int Row) const;

    const std::vector<double> GetSummSpeeds() const;

    std::string GetItemName( int Col ) const;

    //Возвращают истину, когда нужно обновить всю модель
    bool Optimize();

  private:

    inline int ReadFromJson(const Json::Value & jsonPr) override
    {
      return 0;
    }

    inline int WriteToJson(Json::Value & jsonPr) const override
    {
      return 0;
    }

  };

}

#endif // ProductionChainModel_H
