
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
    double _RealTimeProductionOfOneItemPerSec;
    double _CountFactorys;

    std::vector <KEY_FACTORY> _Factorys;
    std::vector <KEY_ITEM>    _ColItems;
    FactoryModules _FM;

    std::vector <double> _CountItems;
    std::vector <double> _ItemsPerSec;

    bool _Init(const ParamsCollection &PC, KEY_RECIPE RecipeId, KEY_FACTORY FactoryId, const std::vector<KEY_ITEM> &Cols);

    bool _SetCountFactorys(double Count);
    bool _SetFactoryModules(const ParamsCollection & PC, const FactoryModules &FM);

    FactoryModules _GetFactoryModules() const;

  public:
    
    friend ProductionChainModel;
    //friend std::vector <ProductionChainModel>;

  };

  class ProductionChainModel : public ItemBase
  {
  public:
    ProductionChainModel(const ParamsCollection &PC, KEY_ITEM ItemKey);
    ProductionChainModel(const ParamsCollection &PC);
    ~ProductionChainModel();
    
    //���������� ������, ����� ����� �������� ��� ������
    bool SetItemKey(KEY_ITEM ItemKey);

    //���������� ������, ����� ����� �������� ��� ������
    bool SetRecipe(int Row, KEY_RECIPE RecipeId);

    //���������� ������, ����� ����� �������� ��� ������
    bool SetFactory(int Row, KEY_FACTORY FactoryId);

    //���������� ������, ����� ����� �������� ��� ������
    bool SetModules(int Row, const std::vector < KEY_MODULE > & Modules );

    int CountItems() const;

    int CountRecipes() const;

    const ProductionChainDataRow &GetRow(int Row) const;

    //���������� ������, ����� ����� �������� ��� ������
    bool Optimize();

  private:
    KEY_ITEM _ItemKey;
    const ParamsCollection &_PC;

    std::vector<KEY_ITEM> _ColsItems;
    std::vector <ProductionChainDataRow>  _DataRows;

  };



}