#ifndef ProductionChainModel_H
#define ProductionChainModel_H

#include "ParamsCollection.h"

namespace ResourceCalculator {

  class ProductionChainModel;

  class ProductionChainDataRow {

    KEY_RECIPE  _RecipeCurrent;
    KEY_FACTORY _FactoryCurrent;

    std::string _CurrentFactoryName;
    std::string _CurrentRecipeName;

    double _PeakPower;
    double _LevelOfPollution;
    double _SpeedFactory;
    double _SecPerOneRecipe;
    double _RealTimeProductionOfOneItemPerSec;
    double _CountFactorys;
    int    _InitColumb;

    std::vector <KEY_FACTORY> _Factorys;
    std::vector <KEY_ITEM>    _ColsItems;
    FactoryModules _FM;

    std::vector <double> _CountItems;
    std::vector <double> _ItemsPerSec;

    bool _Update( const ParamsCollection &PC );

  public:

    bool SetCountFactorys( const ParamsCollection &PC, double Count );
    bool SetFactoryModules( const ParamsCollection &PC, const FactoryModules &FM );
    bool SetFactoryCurrent( const ParamsCollection &PC, KEY_FACTORY );
    bool FindCountFactorysForItemsCount( const ParamsCollection &PC, int Columb, double Count );
    
    bool Init( const ParamsCollection &PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM> &Cols, int InitColumb );

    DeclareAndDefinitionPropertyReadOnly( Factorys, std::vector <KEY_FACTORY> )
    DeclareAndDefinitionPropertyReadOnly( FactoryCurrent, KEY_FACTORY )
    DeclareAndDefinitionPropertyReadOnly( CurrentFactoryName, std::string )
    DeclareAndDefinitionPropertyReadOnly( CurrentRecipeName, std::string )
    DeclarePropertyReadOnly( SummProductivity, double )
    DeclarePropertyReadOnly( SummSpeed, double )
    DeclareAndDefinitionPropertyReadOnly( InitColumb, int )
    DeclareAndDefinitionPropertyReadOnly( SpeedFactory, double )
    DeclareAndDefinitionPropertyReadOnly( SecPerOneRecipe, double )
    DeclareAndDefinitionPropertyReadOnly( RealTimeProductionOfOneItemPerSec, double )
    DeclareAndDefinitionPropertyReadOnly( CountFactorys, double )
    DeclareAndDefinitionRefReadOnly( CountItems, std::vector <double> )
    DeclareAndDefinitionRefReadOnly( ItemsPerSec, std::vector <double> )
    KEY_FACTORY GetFactoryIdFromIndex(int Index) const;
    void DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel );

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

    inline const ParamsCollection &GetPC() const
    {
      return _PC;
    }

    ProductionChainModel(const ParamsCollection &PC, KEY_ITEM ItemKey);
    ProductionChainModel(const ParamsCollection &PC);
    ~ProductionChainModel();
    
    void DeleteModules( const std::set<ResourceCalculator::KEY_MODULE>& ModulesToDel );

    //Возвращают истину, когда нужно обновить всю модель
    bool SetItemKey(KEY_ITEM ItemKey);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetRecipe(int Row, KEY_RECIPE RecipeId);

    //Возвращают истину, когда нужно обновить всю модель
    bool SetFactory(int Row, KEY_FACTORY FactoryId);

    bool SetCountFactores( int Row, double CountFactores );

    //Возвращают истину, когда нужно обновить всю модель
    bool SetModules(int Row, const std::vector < KEY_MODULE > & Modules );

    int CountItems() const;

    int CountRecipes() const;

    const ProductionChainDataRow &GetRow( int Row ) const;

    ProductionChainDataRow &GetRowEdit(int Row);

    const std::vector<double> GetSummSpeeds() const;

    std::string GetItemName( int Col ) const;

    bool FitQuantity();

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
