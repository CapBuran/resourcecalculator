#ifndef ICON_H
#define ICON_H

#include <vector>
#include <string>

#include "Types.h"

namespace ResourceCalculator {

  class Icon {
  private:
    std::vector<char> _data;
    std::string _IconPath;
  public:

    DeclareProperty(IconPath, std::string)

    std::string GetShortName() const;

    const std::vector<char>& GetRawData() const;
    void SetRawData(size_t len, const char* data);
  };

}

#endif ICON_H