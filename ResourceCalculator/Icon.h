#ifndef ICON_H
#define ICON_H

#include <vector>
#include <string>

namespace ResourceCalculator {

  class Icon {
  private:
    std::vector<char> _data;
  public:
    std::string ShortName;
    const std::vector<char>& GetRawData() const;
    void SetRawData(size_t len, const char* data);
  };

}

#endif ICON_H