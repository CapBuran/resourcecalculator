#include "..\..\FactorioCalculator\Recipe.h"
#include "..\..\FactorioCalculator\Item.h"

int main(int argc, char ** argv) {
  using namespace std;
  using namespace FactorioCalculator;

  list<Item> Items;
  {
    Item To1("Железная плита", KEY_ITEM::ID_Iron_Plate);
    Item To2("Медная плита", KEY_ITEM::ID_Cuprum_Plate);
    Item To3("Железная шестерня", KEY_ITEM::ID_Sherst);
    Item To4("Исследовательский пакет 1", KEY_ITEM::ID_Paket1);
    Items.push_back(To1);
    Items.push_back(To2);
    Items.push_back(To3);
    Items.push_back(To4);
  }

  list<Recipe> Recipes;
  {
    std::list<CountsItem> Required, Result;
    RecipeParams RP;
    RP.Required = { };
    RP.Result = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 } };
    Recipe R1("Выплавка меди", KEY_RECIPE::ID_Cuprum_Plate, 3.5, RP);
    Recipes.push_back(R1);

    RP.Required = { };
    RP.Result = { { KEY_ITEM::ID_Iron_Plate, 1.0 } };
    Recipe R2("Выплавка железа", KEY_RECIPE::ID_Iron_Plate, 3.5, RP);
    Recipes.push_back(R2);

    RP.Required = { { KEY_ITEM::ID_Iron_Plate, 2.0 } };
    RP.Result = { { KEY_ITEM::ID_Sherst, 1.0 } };
    Recipe R3("Изготовление шестерни", KEY_RECIPE::ID_Sherst, 0.5, RP);
    Recipes.push_back(R3);

    RP.Required = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_Sherst, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_Paket1, 1 } };
    Recipe R4("Изготовление исследовательского пакета 1", KEY_RECIPE::ID_Paket1, 5.0, RP);
    Recipes.push_back(R4);
  }

  return 0;

}