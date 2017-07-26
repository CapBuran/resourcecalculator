#include "Icon.h"

namespace ResourceCalculator {

  DefinitionProperty(IconPath, std::string, Icon)

  std::string Icon::GetShortName() const
  {
    std::string ShortName;
    auto n = _IconPath.rfind('/');
    if (n != std::string::npos) {
      ShortName.resize(_IconPath.size() - n - 1);
      std::copy(&_IconPath[n + 1], &_IconPath[_IconPath.size()], ShortName.begin());
    } else {
      ShortName = _IconPath;
    }
    return ShortName;
  }

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