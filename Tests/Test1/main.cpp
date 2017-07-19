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
    IC.ADD(To5);
    IC.ADD(To6);
  }

  {
    std::list<CountsItem> Required, Result;
    RecipeParams RP;
    RP.Key = KEY_RECIPE::ID_RECIPE_Cuprum_Plate;
    RP.Time = 3.5;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Ruda, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 } };
    RP.TypeFactory = KEY_TYPE_FACTORY::Furnace;
    Recipe R1("Выплавка меди", RP);

    RP.Key = KEY_RECIPE::ID_RECIPE_Iron_Plate;
    RP.Required = { { KEY_ITEM::ID_ITEM_Iron_Ruda, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } };
    Recipe R2("Выплавка железа", RP);

    RP.Key = KEY_RECIPE::ID_RECIPE_Sherst;
    RP.TypeFactory = KEY_TYPE_FACTORY::Assembly;
    RP.Time = 0.5;
    RP.Required = { { KEY_ITEM::ID_ITEM_Iron_Plate, 2.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Sherst, 1.0 } };
    Recipe R3("Изготовление шестерни", RP);

    RP.Key = KEY_RECIPE::ID_RECIPE_Paket1;
    RP.Time = 5.0;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Paket1, 1 } };
    Recipe R4("Изготовление исследовательского пакета 1", RP);
    
    RP.Key = KEY_RECIPE::ID_RECIPE_Paket1_2;
    RP.Time = 3.0;
    RP.Required = { { KEY_ITEM::ID_ITEM_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_ITEM_Sherst, 1.5 }, { KEY_ITEM::ID_ITEM_Iron_Plate, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_ITEM_Paket1, 1 } };
    Recipe R5("Изготовление исследовательского пакета 1 1", RP);

    RC.Add(R1);
    RC.Add(R2);
    RC.Add(R3);
    RC.Add(R4);
    RC.Add(R5);
  }

  {
    FactoryParams FP;

    Factory P1("Каменная печь", 1.0);
    FP.CountSlotsForModules = 0;
    FP.CountSlotsForRecipes = 2;
    FP.Key = KEY_FACTORY::ID_FACTORY_PechKamenaya;
    FP.LevelOfPollution = 0.01;
    FP.PeakPower = 0.01;
    FP.Speed = 1.0;
    FP.Type = KEY_TYPE_FACTORY::Furnace;
    FP.Wear = 0.0;
    P1.SetParams(FP);

    Factory P2("Стальная печь", 2.0);
    FP.Speed = 2.0;
    FP.Key = KEY_FACTORY::ID_FACTORY_PechStalnaya;
    P2.SetParams(FP);

    Factory P3("Электрическая печь", 2.0);
    FP.Speed = 2.0;
    FP.Key = KEY_FACTORY::ID_FACTORY_PechElectro1;
    FP.CountSlotsForModules = 2;
    FP.CountSlotsForRecipes = 3;
    P3.SetParams(FP);

    Factory A1("Сборочный автомат", 0.50);
    FP.Speed = 0.50;
    FP.Type = KEY_TYPE_FACTORY::Assembly;
    FP.Key = KEY_FACTORY::ID_FACTORY_Assembly1;
    FP.CountSlotsForModules = 0;
    FP.CountSlotsForRecipes = 2;
    A1.SetParams(FP);

    Factory A2("Сборочный автомат 2", 0.75);
    FP.Speed = 0.75;
    FP.Key = KEY_FACTORY::ID_FACTORY_Assembly2;
    FP.CountSlotsForModules = 2;
    FP.CountSlotsForRecipes = 3;
    A2.SetParams(FP);

    Factory A3("Сборочный автомат 3", 1.75);
    FP.Speed = 1.75;
    FP.Key = KEY_FACTORY::ID_FACTORY_Assembly3;
    FP.CountSlotsForModules = 4;
    FP.CountSlotsForRecipes = 4;
    A3.SetParams(FP);

    PC.FC.ADD(A1);
    PC.FC.ADD(A2);
    PC.FC.ADD(A3);
    PC.FC.ADD(P1);
    PC.FC.ADD(P2);
    PC.FC.ADD(P3);
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

  ItemResultTree IRT = PCRestore.RC.BuildTree(KEY_ITEM::ID_ITEM_Paket1, 8);

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