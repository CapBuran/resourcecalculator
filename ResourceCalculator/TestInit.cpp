#include <TestInit.h>

namespace ResourceCalculator
{
  void InitTest1(ParamsCollection& PC)
  {
    {
      std::map<KEY_ITEM, Item> Items;

      Items[KEY_ITEM::ID_ITEM_Iron_Ruda].SetName("Железная руда");
      Items[KEY_ITEM::ID_ITEM_Iron_Ruda].SetKey(KEY_ITEM::ID_ITEM_Iron_Ruda);

      Items[KEY_ITEM::ID_ITEM_Cuprum_Ruda].SetName("Медная руда");
      Items[KEY_ITEM::ID_ITEM_Cuprum_Ruda].SetKey(KEY_ITEM::ID_ITEM_Cuprum_Ruda);

      Items[KEY_ITEM::ID_ITEM_Iron_Plate].SetName("Железная плита");
      Items[KEY_ITEM::ID_ITEM_Iron_Plate].SetKey(KEY_ITEM::ID_ITEM_Iron_Plate);

      Items[KEY_ITEM::ID_ITEM_Cuprum_Plate].SetName("Медная плита");
      Items[KEY_ITEM::ID_ITEM_Cuprum_Plate].SetKey(KEY_ITEM::ID_ITEM_Cuprum_Plate);

      Items[KEY_ITEM::ID_ITEM_Sherst].SetName("Железная шестерня");
      Items[KEY_ITEM::ID_ITEM_Sherst].SetKey(KEY_ITEM::ID_ITEM_Sherst);

      Items[KEY_ITEM::ID_ITEM_science_pack_1].SetName("Исследовательский пакет 1");
      Items[KEY_ITEM::ID_ITEM_science_pack_1].SetKey(KEY_ITEM::ID_ITEM_science_pack_1);

      PC.IC.Add(Items);
    }

    {
      RecipeCollection& RC = PC.RC;

      std::list<CountsItem> Required, Result;
      Recipe recipe;
      recipe.SetKey(KEY_RECIPE::ID_RECIPE_Cuprum_Plate);
      recipe.SetTime(3.5);
      recipe.SetName("Выплавка меди");
      recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Cuprum_Ruda, 1.0 } });
      recipe.SetResult({ { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 } });
      recipe.SetTypeFactory(KEY_TYPE_FACTORY::Furnace);
      RC.Add({ { recipe.GetKey(), recipe } });

      recipe.SetKey(KEY_RECIPE::ID_RECIPE_Iron_Plate);
      recipe.SetName("Выплавка железа");
      recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Iron_Ruda, 1.0 } });
      recipe.SetResult({ { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } });
      RC.Add({ { recipe.GetKey(), recipe } });

      recipe.SetKey(KEY_RECIPE::ID_RECIPE_Sherst);
      recipe.SetTypeFactory(KEY_TYPE_FACTORY::Assembly);
      recipe.SetTime(0.5);
      recipe.SetName("Изготовление шестерни");
      recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Iron_Plate, 2.0 } });
      recipe.SetResult({ { KEY_ITEM::ID_ITEM_Sherst, 1.0 } });
      RC.Add({ { recipe.GetKey(), recipe } });

      recipe.SetKey(KEY_RECIPE::ID_RECIPE_science_pack_1);
      recipe.SetTime(5.0);
      recipe.SetName("Изготовление исследовательского пакета 1");
      recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.0 } });
      recipe.SetResult({ { KEY_ITEM::ID_ITEM_science_pack_1, 1 } });
      RC.Add({ { recipe.GetKey(), recipe } });
    }

    {
      FactoryTypeCollection& FTC = PC.FTC;
      FactoryType ft;
      ft.SetKey(KEY_TYPE_FACTORY::Assembly);
      ft.SetName("Сборочный автомат");
      FTC.Add(ft);
      ft.SetKey(KEY_TYPE_FACTORY::AssemblyFluid);
      ft.SetName("Сборочный автомат с трубой");
      FTC.Add(ft);
      ft.SetKey(KEY_TYPE_FACTORY::ChemicalPlant);
      ft.SetName("Химический завод");
      FTC.Add(ft);
      ft.SetKey(KEY_TYPE_FACTORY::Furnace);
      ft.SetName("Печь");
      FTC.Add(ft);
      ft.SetKey(KEY_TYPE_FACTORY::Refinery);
      ft.SetName("Нефтеперегонка");
      FTC.Add(ft);
    }

    {
      Factory factory;

      factory.SetName("Каменная печь");
      factory.SetSpeed(1.0);
      factory.SetCountSlotsForModules(0);
      factory.SetCountSlotsForRecipes(2);
      factory.SetKey(KEY_FACTORY::ID_FACTORY_PechKamenaya);
      factory.SetLevelOfPollution(0.01);
      factory.SetPower(0.0);
      factory.SetSpeed(1.0);
      factory.SetTypes({ KEY_TYPE_FACTORY::Furnace });
      factory.SetWear(0.0);
      PC.FC.Add({ {factory.GetKey(), factory} });

      factory.SetName("Стальная печь");
      factory.SetSpeed(2.0);
      factory.SetKey(KEY_FACTORY::ID_FACTORY_PechStalnaya);
      PC.FC.Add({ {factory.GetKey(), factory} });

      factory.SetName("Электрическая печь");
      factory.SetSpeed(2.0);
      factory.SetKey(KEY_FACTORY::ID_FACTORY_PechElectro1);
      factory.SetCountSlotsForModules(2);
      factory.SetCountSlotsForRecipes(3);
      PC.FC.Add({ {factory.GetKey(), factory} });

      factory.SetName("Сборочный автомат");
      factory.SetSpeed(0.50);
      factory.SetTypes({ KEY_TYPE_FACTORY::Assembly });
      factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly1);
      factory.SetCountSlotsForModules(0);
      factory.SetCountSlotsForRecipes(2);
      PC.FC.Add({ {factory.GetKey(), factory} });

      factory.SetName("Сборочный автомат 2");
      factory.SetSpeed(0.75);
      factory.SetTypes({ KEY_TYPE_FACTORY::Assembly, KEY_TYPE_FACTORY::AssemblyFluid });
      factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly2);
      factory.SetCountSlotsForModules(2);
      factory.SetCountSlotsForRecipes(3);
      PC.FC.Add({ {factory.GetKey(), factory} });

      factory.SetName("Сборочный автомат 3");
      factory.SetSpeed(1.75);
      factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly3);
      factory.SetCountSlotsForModules(4);
      factory.SetCountSlotsForRecipes(4);
      PC.FC.Add({ {factory.GetKey(), factory} });
    }
  }
}
