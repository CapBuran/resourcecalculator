#include "Icon.h"

namespace ResourceCalculator {

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

}