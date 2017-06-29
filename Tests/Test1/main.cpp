#include "..\..\ResourceCalculator\ParamsCollection.h"
#include <fstream>
#include "..\..\ThreeParty\Base64\base64.h"

#define StandartTestFileJson "C:\\games\\Factorio.json"


using namespace ResourceCalculator;



int main(int argc, char ** argv) {
  using namespace std;

  ParamsCollection PC;
  ItemCollection &IC = PC.IC;
  RecipeCollection &RC = PC.RC;

  {
    Item To1("Железная плита", KEY_ITEM::ID_ITEM_Iron_Plate);
    Item To2("Медная плита", KEY_ITEM::ID_ITEM_Cuprum_Plate);
    Item To3("Железная шестерня", KEY_ITEM::ID_ITEM_Sherst);
    Item To4("Исследовательский пакет 1", KEY_ITEM::ID_ITEM_Paket1);
    Item To5("Железная руда", KEY_ITEM::ID_ITEM_Iron_Ruda);
    Item To6("Медная руда", KEY_ITEM::ID_ITEM_Cuprum_Ruda);
    IC.ADD(To1);
    IC.ADD(To2);
    IC.ADD(To3);
    IC.ADD(To4);
  }

  {
    std::list<CountsItem> Required, Result;
    RecipeParams RP;
    RP.Key = KEY_RECIPE::ID_RECIPE_Cuprum_Plate;
    RP.Time = 3.5;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Ruda, 1.0 } };
    RP.FactoryAllowed = { KEY_FACTORY::ID_FACTORY_PechKamenaya, KEY_FACTORY::ID_FACTORY_PechStalnaya };
    RP.Result = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 } };
    RP.CurrentFactory = KEY_FACTORY::ID_FACTORY_PechKamenaya;
    Recipe R1("Выплавка меди", RP);

    RP.Key = KEY_RECIPE::ID_RECIPE_Iron_Plate;
    RP.Required = { { KEY_ITEM::ID_ITEM_Iron_Ruda, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } };
    Recipe R2("Выплавка железа", RP);

    RP.CurrentFactory = KEY_FACTORY::ID_FACTORY_Assembly1;
    RP.FactoryAllowed = { KEY_FACTORY::ID_FACTORY_Assembly1, KEY_FACTORY::ID_FACTORY_Assembly2, KEY_FACTORY::ID_FACTORY_Assembly3 };
    RP.Key = KEY_RECIPE::ID_RECIPE_Sherst;
    RP.Time = 0.5;
    RP.Required = { { KEY_ITEM::ID_ITEM_Iron_Plate, 2.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Sherst, 1.0 } };
    Recipe R3("Изготовление шестерни", RP);

    RP.CurrentFactory = KEY_FACTORY::ID_FACTORY_Assembly2;
    RP.Key = KEY_RECIPE::ID_RECIPE_Paket1;
    RP.Time = 5.0;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Paket1, 1 } };
    Recipe R4("Изготовление исследовательского пакета 1", RP);
    
    RP.CurrentFactory = KEY_FACTORY::ID_FACTORY_Assembly3;
    RP.Key = KEY_RECIPE::ID_RECIPE_Paket1_2;
    RP.Time = 3.0;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.5 }, { KEY_ITEM::ID_ITEM_Sherst, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Paket1, 1 } };
    Recipe R5("Изготовление исследовательского пакета 1 1", RP);

    RC.Add(R1);
    RC.Add(R2);
    RC.Add(R3);
    RC.Add(R4);
    RC.Add(R5);
  }

  std::ofstream out;
  out.open(StandartTestFileJson);
  
  Json::Value jsonPr;
  IC.WriteToJson(jsonPr["Items"]);
  RC.WriteToJson(jsonPr["Recipes"]);
  Json::StyledWriter styledWriter;
  out << styledWriter.write(jsonPr);
  out.close();

  ItemCollection ICRestore;
  RecipeCollection RCRestore;
  Json::Value jsonPrRestore;

  std::ifstream in(StandartTestFileJson);

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();
  in.clear();

  Json::Reader JsonReader;
  bool parsingSuccessful = JsonReader.parse(contents.c_str(), jsonPrRestore);
  contents.clear();

  ICRestore.ReadFromJson(jsonPrRestore["Items"]);
  RCRestore.ReadFromJson(jsonPrRestore["Recipes"]);

  ItemResultTree IRT = RCRestore.BuildTree(KEY_ITEM::ID_ITEM_Paket1, 8);

  std::list <KEY_RECIPE> ResultRecipes;
  std::list <KEY_ITEM> ResultItems;
  std::map<KEY_ITEM, KEY_RECIPE> Ansfer;

  RCRestore.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);

  std::string sss("Русские буквы");

  std::vector<char> d(sss.size());
  
  for (size_t i = 0; i < sss.size(); i++){
    d[i] = sss[i];
  }

  std::vector<char> d1, d2;

  base64_encode(d, d1);
  base64_decode(d1, d2);
  
  std::string sss2;
  sss2.resize(d2.size());

  for (size_t i = 0; i < sss.size(); i++) {
    sss2[i] = d2[i];
  }

  bool b1 = sss2 == sss;
  sss[2] = '5';
  bool b2 = sss2 == sss;

  return 0;

}