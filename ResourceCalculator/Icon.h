#ifndef ICON_H
#define ICON_H

#include <vector>
#include <string>

#include "Types.h"

namespace ResourceCalculator {

  class Icon {
  private:
    std::string _Entry;
    std::string _Name;
    std::string _KeyPath;
    int _Size;
    std::vector<char> _data;
  public:

    DeclareProperty(Entry, std::string)
    DeclareProperty(Name, std::string)
    DeclareProperty(KeyPath, std::string)
    DeclareProperty(Size, int)

    std::string GetShortName() const;

    const std::vector<char>& GetRawData() const;
    void SetRawData(size_t len, const char* data);
    void ReadFromFileRAW(std::string FullPath);
    void ReadFromFileZIP(std::string FullPathZip, std::string PathInZip);
  };

}

#endif// ICON_H
