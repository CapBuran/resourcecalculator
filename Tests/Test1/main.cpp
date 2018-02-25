#include "..\..\ResourceCalculator\ParamsCollection.h"
#include <fstream>
#include "..\..\ThreeParty\Base64\base64.h"

#include "..\..\ResourceCalculator\ProductionChainModel.h"

#define StandartTestFileJson "Factorio.json"

using namespace ResourceCalculator;



int main(int argc, char ** argv) {
  using namespace std;

  ParamsCollection PC;
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
    PC.ReadFromJson(jsonPrRestore);
  }

  std::list<KEY_ITEM> ListRequest;
  std::list<KEY_ITEM> ListRequestResourceOnly;

  //ItemResultTree IRT = PC.RC.BuildTree(KEY_ITEM::ID_ITEM_science_pack_2, 16, ListRequest, ListRequestResourceOnly );
  ItemResultTree IRT = PC.RC.BuildTree(KEY_ITEM::ID_ITEM_petroleum_gas, 16, ListRequest, ListRequestResourceOnly);
  
  std::list <KEY_RECIPE> ResultRecipes;
  std::list <KEY_ITEM> ResultItems;
  std::map<KEY_ITEM, KEY_RECIPE> Ansfer;


  PC.RC.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);

  ProductionChainModel PCM(PC);
  PCM.SetItemKey(KEY_ITEM::ID_ITEM_science_pack_1);


  return 0;

}