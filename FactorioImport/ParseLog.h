#pragma once

#include <string>
#include <list>

namespace FactorioImport
{
  struct IconLog {
    int Size = 0;
    std::string Path;
  };

  struct IconsLog
  {
    std::string Entry;
    std::string Name;
    std::list<IconLog> Icons;
  };

  class ParseLog
  {
  public:
    std::string ReadData;
    std::string WriteData;
    std::string BinaryData;
    std::list<IconsLog> Icons;
    bool Parse(std::string FullPath);
  };
}
