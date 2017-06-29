
#include "ParamsCollection.h"


namespace ResourceCalculator {


  struct ProductionChainDataRow {
    KEY_RECIPE  RecipeCurrent;
    KEY_FACTORY FactoryCurrent;

    double PeakPower;
    double SpeedFactory;
    double SecPerOneRecipe;
    double ProductionSpeedPerSecond;
    double CountFactorys;

    std::vector <KEY_RECIPE> Recipes;
    std::vector <KEY_FACTORY> Factorys;
    FactoryModules FM;

    std::vector <double> CountRecipes;
    std::vector <double> ItemsPerSec;

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
    bool SetFactory(int Row, KEY_FACTORY FactoryId);

    //���������� ������, ����� ����� �������� ��� ������
    bool SetModules(int Row, const std::vector < KEY_MODULE > & Modules );

    //���������� ������, ����� ����� �������� ��� ������
    bool SetRecipe(int Row, KEY_RECIPE RecipeId);
    
    //���������� ������, ����� ����� �������� ��� ������
    bool Rebuild();

    int CountRequired() const;
    int CountResult() const;

    const ProductionChainDataRow &GetRow(int Row) const;

  private:
    KEY_ITEM _ItemKey;
    const ParamsCollection &_PC;

    std::vector <ProductionChainDataRow>  _DataRows;

      
    //std::vector <std::list<std::pair <KEY_MODULE, int> > > _FactoryVariants;

  };



}