#include "..\..\FactorioCalculator\Recipe.h"
#include "..\..\FactorioCalculator\Item.h"

int main(int argc, char ** argv) {
  using namespace std;
  using namespace FactorioCalculator;

  list<Item> Items;
  {
    Item To1("�������� �����", KEY_ITEM::ID_Iron_Plate);
    Item To2("������ �����", KEY_ITEM::ID_Cuprum_Plate);
    Item To3("�������� ��������", KEY_ITEM::ID_Sherst);
    Item To4("����������������� ����� 1", KEY_ITEM::ID_Paket1);
    Items.push_back(To1);
    Items.push_back(To2);
    Items.push_back(To3);
    Items.push_back(To4);
  }

  list<Recipe> Recipes;
  {
    std::list<CountsItem> Required, Result;

    Required = { };
    Result = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 } };
    Recipe R1("�������� ����", KEY_RECIPE::ID_Cuprum_Plate, 3.5, Required, Result);
    Recipes.push_back(R1);

    Required = { };
    Result = { { KEY_ITEM::ID_Iron_Plate, 1.0 } };
    Recipe R2("�������� ������", KEY_RECIPE::ID_Iron_Plate, 3.5, Required, Result);
    Recipes.push_back(R2);

    Required = { { KEY_ITEM::ID_Iron_Plate, 2.0 } };
    Result = { { KEY_ITEM::ID_Sherst, 1.0 } };
    Recipe R3("������������ ��������", KEY_RECIPE::ID_Sherst, 0.5, Required, Result);
    Recipes.push_back(R3);

    Required = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_Sherst, 1.0 } };
    Result = { { KEY_ITEM::ID_Paket1, 1 } };
    Recipe R4("������������ ������������������ ������ 1", KEY_RECIPE::ID_Paket1, 5.0, Required, Result);
    Recipes.push_back(R4);
  }

  return 0;

}