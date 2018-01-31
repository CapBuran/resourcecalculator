#include "..\..\ResourceCalculator\ParamsCollection.h"
#include <fstream>
#include "..\..\ThreeParty\Base64\base64.h"

#include "..\..\ResourceCalculator\ProductionChainModel.h"

#define StandartTestFileJson "C:\\games\\Factorio.json"


using namespace ResourceCalculator;



int main(int argc, char ** argv) {
  using namespace std;

  ParamsCollection PC;
  ItemCollection &IC = PC.IC;
  RecipeCollection &RC = PC.RC;

  //{
  //  Item To;
  //  To.SetName("Железная плита");            To.SetKey(KEY_ITEM::ID_ITEM_Iron_Plate);
  //  IC.ADD(To);
  //  To.SetName("Медная плита");              To.SetKey(KEY_ITEM::ID_ITEM_Cuprum_Plate);
  //  IC.ADD(To);
  //  To.SetName("Железная шестерня");         To.SetKey(KEY_ITEM::ID_ITEM_Sherst);
  //  IC.ADD(To);
  //  To.SetName("Исследовательский пакет 1"); To.SetKey(KEY_ITEM::ID_ITEM_Paket1);
  //  IC.ADD(To);
  //  To.SetName("Железная руда");             To.SetKey(KEY_ITEM::ID_ITEM_Iron_Ruda);
  //  IC.ADD(To);
  //  To.SetName("Медная руда");               To.SetKey(KEY_ITEM::ID_ITEM_Cuprum_Ruda);
  //  IC.ADD(To);
  //}

  {
    std::list<CountsItem> Required, Result;
    Recipe recipe;
    recipe.SetKey(KEY_RECIPE::ID_RECIPE_Cuprum_Plate);
    recipe.SetTime(3.5);
    recipe.SetName("Выплавка меди");
    recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Cuprum_Ruda, 1.0 } });
    recipe.SetResult({ { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 } });
    recipe.SetTypeFactory(KEY_TYPE_FACTORY::Furnace);
    RC.Add(recipe);

    recipe.SetKey(KEY_RECIPE::ID_RECIPE_Iron_Plate);
    recipe.SetName("Выплавка железа");
    recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Iron_Ruda, 1.0 } });
    recipe.SetResult({ { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } });
    RC.Add(recipe);

    recipe.SetKey(KEY_RECIPE::ID_RECIPE_Sherst);
    recipe.SetTypeFactory(KEY_TYPE_FACTORY::Assembly);
    recipe.SetTime(0.5);
    recipe.SetName("Изготовление шестерни");
    recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Iron_Plate, 2.0 } });
    recipe.SetResult({ { KEY_ITEM::ID_ITEM_Sherst, 1.0 } });
    RC.Add(recipe);

    recipe.SetKey(KEY_RECIPE::ID_RECIPE_Paket1);
    recipe.SetTime(5.0);
    recipe.SetName("Изготовление исследовательского пакета 1");
    recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.0 } });
    recipe.SetResult({ { KEY_ITEM::ID_ITEM_Paket1, 1 } });
    RC.Add(recipe);

    recipe.SetKey(KEY_RECIPE::ID_RECIPE_Paket1_2);
    recipe.SetTime(3.0);
    recipe.SetName( "Изготовление исследовательского пакета 1 1" );
    recipe.SetRequired({ { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.5 }, { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } });
    recipe.SetResult( { { KEY_ITEM::ID_ITEM_Paket1, 1 } } );
    RC.Add(recipe);

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
    factory.SetType(KEY_TYPE_FACTORY::Furnace);
    factory.SetWear(0.0);
    PC.FC.ADD(factory);

    factory.SetName("Стальная печь");
    factory.SetSpeed(2.0);
    factory.SetKey(KEY_FACTORY::ID_FACTORY_PechStalnaya);
    PC.FC.ADD(factory);

    factory.SetName("Электрическая печь");
    factory.SetSpeed(2.0);
    factory.SetKey(KEY_FACTORY::ID_FACTORY_PechElectro1);
    factory.SetCountSlotsForModules(2);
    factory.SetCountSlotsForRecipes(3);
    PC.FC.ADD(factory);

    factory.SetName("Сборочный автомат");
    factory.SetSpeed(0.50);
    factory.SetType(KEY_TYPE_FACTORY::Assembly);
    factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly1);
    factory.SetCountSlotsForModules(0);
    factory.SetCountSlotsForRecipes(2);
    PC.FC.ADD(factory);

    factory.SetName("Сборочный автомат 2");
    factory.SetSpeed(0.75);
    factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly2);
    factory.SetCountSlotsForModules(2);
    factory.SetCountSlotsForRecipes(3);
    PC.FC.ADD(factory);

    factory.SetName("Сборочный автомат 3");
    factory.SetSpeed(1.75);
    factory.SetKey(KEY_FACTORY::ID_FACTORY_Assembly3);
    factory.SetCountSlotsForModules(4);
    factory.SetCountSlotsForRecipes(4);
    PC.FC.ADD(factory);

  }


  std::ofstream out;
  out.open(StandartTestFileJson);
  
  Json::Value jsonPr;
  PC.WriteToJson(jsonPr);
  Json::StyledWriter styledWriter;
  out << styledWriter.write(jsonPr);
  out.close();


  

  ParamsCollection PCRestore;

  {
    std::ifstream in(StandartTestFileJson);

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    in.clear();

    Json::Reader JsonReader;
    Json::Value jsonPrRestore;
    bool parsingSuccessful = JsonReader.parse(contents.c_str(), jsonPrRestore);
    contents.clear();
    PCRestore.ReadFromJson(jsonPrRestore);

  }

  std::list<KEY_ITEM> ListRequest;
  std::list<KEY_ITEM> ListRequestResourceOnly;

  ItemResultTree IRT = PCRestore.RC.BuildTree(KEY_ITEM::ID_ITEM_Paket1, 8, ListRequest, ListRequestResourceOnly );
  
  std::list <KEY_RECIPE> ResultRecipes;
  std::list <KEY_ITEM> ResultItems;
  std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

  Ansfer[KEY_ITEM::ID_ITEM_Sherst] = KEY_RECIPE::ID_RECIPE_PreviouslyProduced;
  Ansfer[KEY_ITEM::ID_ITEM_Paket1] = KEY_RECIPE::ID_RECIPE_Paket1_2;

  PCRestore.RC.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);

  std::string sss("Русские буквы");

  std::vector<char> d(sss.size() + 1);
  
  for (size_t i = 0; i < sss.size(); i++){
    d[i] = sss[i];
  }

  std::vector<char> d1, d2;

  base64_encode(d, d1);
  base64_decode(d1, d2);
  
  std::string sss2;
  sss2.resize(d2.size()-1);

  for (size_t i = 0; i < sss.size(); i++) {
    sss2[i] = d2[i];
  }

  bool b1 = sss2 == sss;
  sss[2] = '5';
  bool b2 = sss2 == sss;


  ProductionChainModel PCM(PC);
  PCM.SetItemKey(KEY_ITEM::ID_ITEM_Paket1);


  return 0;

}