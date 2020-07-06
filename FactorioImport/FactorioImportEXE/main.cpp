#include <fstream>
#include <set>
#include <filesystem>
#include <iostream>

#include "../../ThreeParty/Base64/base64.h"
#include "../../ResourceCalculator/ParamsCollection.h"
#include "../../ResourceCalculator/ProductionChainTree.h"
#include <ParseLog.h>
#include <ParseElements.h>
#include <Convert.h>

#define StandartTestFileJson "Factorio.json"

using namespace ResourceCalculator;
using namespace FactorioImport;

int main(int argc, char ** argv) {
  using namespace std;

  FactorioImport::ParseLog LOG;

  LOG.Parse("C:/Users/Vasiliy/AppData/Roaming/Factorio/factorio-current.log");

  ParamsCollection PC;

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
        Icon icon_toAdd;
        icon_toAdd.SetName(it.Name);
        icon_toAdd.SetEntry(it.Entry);
        icon_toAdd.SetSize(it_icon.Size);
        icon_toAdd.ReadFromFileRAW(FullPath);
        icon_toAdd.SetKeyPath(it_icon.Path);
        PC.Icons.ADD(icon_toAdd);
      }
    }
  }

  AllImport all;
  all.ReadAll(LOG.WriteData + "/script-output/recipe-lister");
  all.SetIcons(LOG.Icons);

  Converter c(LOG, all, PC);
  c.Run();
  
  Json::Value jsonPr;
  PC.WriteToJson(jsonPr);
  Json::StyledWriter styledWriter;

  std::ofstream output("Factorio.json", std::ios::binary);

  std::string data = styledWriter.write(jsonPr);

  output.write(data.c_str(), data.size());

  return 0;

}
