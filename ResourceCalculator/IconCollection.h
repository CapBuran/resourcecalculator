#ifndef IconCollectionH
#define IconCollectionH

#include "Icon.h"
#include "Types.h"

namespace ResourceCalculator {

  class IconCollection : public Jsonable
  {
  public:

    void ADD(std::string KeyIcon, std::vector<char> rawdata);
    void ADD(std::string KeyIcon, std::string fullpath);
    void Delete(std::string KeyIcon);

    int ReadAll(const std::string &freffix_add, const std::string &freffix_delte, const std::list<std::string> &files, bool IsClean = false);
    int ReadRawDataAsJson(const char* RawData);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const Icon &GetIcon(std::string KeyIcon) const;

  private:
    std::map<std::string, Icon> _Icons;

  };

}

#endif // !IconCollectionH
