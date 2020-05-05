#include <fstream>
#include <set>

#include "../../ThreeParty/Base64/base64.h"
#include "../../ResourceCalculator/ParamsCollection.h"
#include "../../ResourceCalculator/ProductionChainTree.h"
#include "../../FactorioImport/ParseLog.h"

#define StandartTestFileJson "Factorio.json"

using namespace ResourceCalculator;
using namespace FactorioImport;

int main(int argc, char ** argv) {
  using namespace std;

  FactorioImport::ParseLog LOG;

  LOG.Parse("C:/Users/Vasiliy/AppData/Roaming/Factorio/factorio-current.log");

  ResourceCalculator::IconCollection icons;

  for (const auto& it: LOG.Icons)
  {
    if (it.Name.empty()) continue;

    for (const auto& it_icon: it.Icons)
    {
      std::string FullPath;
      if (it_icon.Path.substr(0, 8) == "__base__") {
        FullPath = LOG.ReadData + "/base" + it_icon.Path.substr(8);
      }
      else
      {
        //TODO
      }
      //TODO Check:

      if (!FullPath.empty())
      {
        ResourceCalculator::Icon icon_toAdd;
        icon_toAdd.SetName(it.Name);
        icon_toAdd.SetEntry(it.Entry);
        icon_toAdd.ReadFromFileRAW(FullPath);
        icon_toAdd.SetKeyPath(it_icon.Path);
        icons.ADD(icon_toAdd);
      }
    }
  }













  std::set<std::string> entries = {
    "recipe",
    "inserter",
    "resource",
    "item",
    "fluid",
    "technology",
    "boiler",
    "generator",
    "reactor",
    "lab",
    "equipment",
    "assembling-machine",
    "furnace",
    "mining-drill",
    "transport-belt",
    "solar-panel",
    "equipment-grid"
  };









  //for (auto& it: LOG.Icons)
  //{
  //  if (it.Entry == "recipe" && it.Icons.empty())
  //  {
  //    for(auto& )
  //  }
  //}

  //ParamsCollection PC;
  //{
  //  std::ifstream in(StandartTestFileJson);

  //  std::string contents;
  //  in.seekg(0, std::ios::end);
  //  contents.resize(in.tellg());
  //  in.seekg(0, std::ios::beg);
  //  in.read(&contents[0], contents.size());
  //  in.close();
  //  in.clear();

  //  Json::Reader JsonReader;
  //  Json::Value jsonPrRestore;
  //  bool parsingSuccessful = JsonReader.parse(contents.c_str(), jsonPrRestore);
  //  contents.clear();
  //  PC.ReadFromJson(jsonPrRestore);
  //}

  //std::list<KEY_ITEM> ListRequest;
  //std::list<KEY_ITEM> ListRequestResourceOnly;


  //KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_advanced_circuit;
  ////KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_science_pack_2;
  ////KEY_ITEM KeyFind = KEY_ITEM::ID_ITEM_petroleum_gas;

  //std::list <KEY_RECIPE> ResultRecipes;
  //std::list <KEY_ITEM> ResultItems;
  //std::map<KEY_ITEM, KEY_RECIPE> AnsferItems;
  //std::map<KEY_RECIPE, KEY_ITEM> AnsferRecipes;

  //std::list <KEY_RECIPE> ResultRecipes2;
  //std::list <KEY_ITEM>   ResultItems2;

  //ItemResultTree IRT2(PC, KeyFind);

  //IRT2.Travelling(150, AnsferItems, AnsferRecipes, ResultRecipes2, ResultItems2);

  //ProductionChainModel PCM(PC, KEY_ITEM::ID_ITEM_science_pack_1);

  return 0;

}
