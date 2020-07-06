#ifndef IconCollectionH
#define IconCollectionH

#include "Icon.h"
#include "Types.h"

namespace ResourceCalculator {

  class IconCollection : public Jsonable
  {
  public:

    void ADD(std::string KeyIcon, const std::vector<char>& rawdata);

    void ADD(const Icon& icon);
    void Delete(std::string keyIcon);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const Icon &GetIcon(std::string KeyIcon) const;

    const std::map<std::string, Icon> & GetAllIcon() const;

  private:
    std::map<std::string, Icon> _Icons;

  };

}

#endif // !IconCollectionH
