#include "Icon.h"
#include <fstream>

namespace ResourceCalculator 
{

  DefinitionProperty(Entry, std::string, Icon)
  DefinitionProperty(Name, std::string, Icon)
  DefinitionProperty(KeyPath, std::string, Icon)
  DefinitionPropertyReadOnly(Size, int, Icon)

  const std::vector<char>& Icon::GetRawData() const
  {
    return _data;
  }

  void Icon::SetRawData(size_t len, const char * data)
  {
    _data.clear();
    _data.resize(len);
    std::copy(data, data + len, _data.begin());
  }

  void Icon::ReadFromFileRAW(std::string FullPath)
  {
    std::ifstream in(FullPath, std::ios::binary);
    if (in.is_open())
    {
      std::vector<char> contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      in.clear();
    }
  }

  void Icon::ReadFromFileZIP(std::string FullPathZip, std::string PathInZip)
  {
  }

  std::string Icon::GetShortName() const
  {
    return _Name;
  }

}