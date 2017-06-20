#include <string>

#include "Types.h"

namespace ResourceCalculator {
  class Factory : public FactorioItem, public Jsonable {
  private:
    double _Speed;
    double _Power;
    
    KEY_FACTORY _Key;

    Factory();
    Factory(const Factory &recipe);

  public:

    Factory(std::string const &Name, double Speed, double Power = 1.0);
    ~Factory();

    KEY_FACTORY GetKey() const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    friend std::pair<const KEY_FACTORY, Factory>;

  };

}
