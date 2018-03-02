#include <fstream>

#include "../../ThreeParty/Base64/base64.h"
#include "../../ResourceCalculator/ParamsCollection.h"
#include "../../ResourceCalculator/ProductionChainTree.h"

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


  KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_advanced_circuit;
  //KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_science_pack_2;
  //KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_petroleum_gas;

  ItemResultTree IRT = PC.RC.BuildTree(KeyFind, 16, ListRequest, ListRequestResourceOnly);
  
  std::list <KEY_RECIPE> ResultRecipes;
  std::list <KEY_ITEM> ResultItems;
  std::map<KEY_ITEM, KEY_RECIPE> Ansfer;


  PC.RC.Travelling(IRT, Ansfer, ResultRecipes, ResultItems);


  std::list <KEY_RECIPE> ResultRecipes2;
  std::list <KEY_ITEM>   ResultItems2;

  ItemResultTree2 IRT2(PC, KeyFind);

  IRT2.Travelling(150, Ansfer, ResultRecipes2, ResultItems2);

  ProductionChainModel PCM(PC, KEY_ITEM::ID_ITEM_science_pack_1);

  return 0;

}
