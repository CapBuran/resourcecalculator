#include "..\..\FactorioCalculator\ItemCollection.h"
#include "..\..\FactorioCalculator\RecipeCollection.h"
#include <fstream>

#define StandartTestFileJson "C:\\games\\Factorio.json"

int main(int argc, char ** argv) {
  using namespace std;
  using namespace FactorioCalculator;

  ItemCollection IC;
  RecipeCollection RC;

  {
    Item To1("Железная плита", KEY_ITEM::ID_Iron_Plate);
    Item To2("Медная плита", KEY_ITEM::ID_Cuprum_Plate);
    Item To3("Железная шестерня", KEY_ITEM::ID_Sherst);
    Item To4("Исследовательский пакет 1", KEY_ITEM::ID_Paket1);
    IC.ADD(To1);
    IC.ADD(To2);
    IC.ADD(To3);
    IC.ADD(To4);
  }

  {
    std::list<CountsItem> Required, Result;
    RecipeParams RP;
    RP.Key = KEY_RECIPE::ID_Cuprum_Plate;
    RP.Time = 3.5;
    RP.Required = { };
    RP.Result = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 } };
    Recipe R1("Выплавка меди", RP);

    RP.Required = { };
    RP.Key = KEY_RECIPE::ID_Iron_Plate;
    RP.Time = 3.5;
    RP.Result = { { KEY_ITEM::ID_Iron_Plate, 1.0 } };
    Recipe R2("Выплавка железа", RP);

    RP.Key = KEY_RECIPE::ID_Sherst;
    RP.Time = 0.5;
    RP.Required = { { KEY_ITEM::ID_Iron_Plate, 2.0 } };
    RP.Result = { { KEY_ITEM::ID_Sherst, 1.0 } };
    Recipe R3("Изготовление шестерни", RP);

    RP.Key = KEY_RECIPE::ID_Paket1;
    RP.Time = 5.0;
    RP.Required = { { KEY_ITEM::ID_Cuprum_Plate, 1.0 }, { KEY_ITEM::ID_Sherst, 1.0 } };
    RP.Result = { { KEY_ITEM::ID_Paket1, 1 } };
    Recipe R4("Изготовление исследовательского пакета 1", RP);

    RC.ADD(R1);
    RC.ADD(R2);
    RC.ADD(R3);
    RC.ADD(R4);
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

  ICRestore.ReadFromJson(jsonPr["Items"]);
  RCRestore.ReadFromJson(jsonPr["Recipes"]);

  return 0;

}