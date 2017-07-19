#include <fstream>
#include <string>
#include <string.h>

#include "base64.h"
#include "IconCollection.h"

namespace ResourceCalculator {


  void IconCollection::ADD(std::string KeyIcon, std::vector<char> rawdata)
  {
    Icon AddIcon;
    AddIcon.SetRawData(rawdata.size(), &rawdata[0]);
    _Icons[KeyIcon] = AddIcon;
  }

  void IconCollection::ADD(std::string KeyIcon, std::string fullpath)
  {
    std::ifstream in(fullpath, std::ios::binary);
    std::vector<char> contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    in.clear();
    ADD(KeyIcon, contents);
  }

  void IconCollection::Delete(std::string KeyIcon)
  {
    _Icons.erase(KeyIcon);
  }

  int IconCollection::ReadAll(const std::string &freffix_add, const std::string &freffix_delte, const std::list<std::string> &files, bool IsClean)
  {
    if(IsClean) _Icons.clear();
    for (auto & file: files){
      std::string keyToAdd = file;
      size_t p = keyToAdd.find(freffix_delte);
      if (p != std::string::npos) {
        keyToAdd.erase(p, freffix_delte.length());
      }
      keyToAdd = freffix_add + keyToAdd;
      ADD(keyToAdd, file);
    }
    return 0;
  }

  int IconCollection::ReadRawDataAsJson(const char* RawData)
  {
    Json::Value jsonPrRestore;
    Json::Reader JsonReader;
    bool parsingSuccessful = JsonReader.parse(RawData, jsonPrRestore);
    ReadFromJson(jsonPrRestore["Icons"]);
    return 0;
  }

  int IconCollection::ReadFromJson(const Json::Value & jsonPr)
  {
    for (auto it : jsonPr) {
      std::string keyToAdd = it["KeyPath"].asString();
      std::string DataBase64_ = it["Data"].asString();
      std::vector<char> DataBase64(DataBase64_.length());
      std::vector<char> Data;
      std::copy(DataBase64_.begin(), DataBase64_.end(), DataBase64.begin());
      base64_decode(DataBase64, Data);
      Icon ToAdd;
      ToAdd.SetRawData(Data.size() - 1, &Data[0]);
      ADD(keyToAdd, Data);
    }
    return 0;
  }

  int IconCollection::WriteToJson(Json::Value & jsonPr) const
  {
    jsonPr = Json::Value(Json::arrayValue);
    for (auto& it : _Icons) {
      Json::Value newVal;
      newVal["KeyPath"]  = it.first;
      std::vector <char> toWriteString;
      base64_encode(it.second.GetRawData(), toWriteString);
      toWriteString.push_back('\0');
      newVal["Data"] = &toWriteString[0];
      jsonPr.append(newVal);
    }
    return 0;
  }

  const Icon & IconCollection::GetIcon(std::string KeyIcon) const
  {
    static Icon EmtyIcon;
    std::map<std::string, Icon>::const_iterator FinVal = _Icons.find(KeyIcon);
    return FinVal == _Icons.end() ? EmtyIcon : FinVal->second;
  }

}