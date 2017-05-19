#include "Types.h"

namespace FactorioCalculator  {

  class Item: public FactorioItem, public Jsonable {
  private:
    const KEY_ITEM _Key;
  public:
    Item(std::string const &Name, KEY_ITEM Key);
    ~Item();
    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;
  };

}
