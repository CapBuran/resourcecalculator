#include <string>

#include "Types.h"

namespace ResourceCalculator {

  class Factory;

  class FactoryModules {
  private:
    std::vector < KEY_MODULE > _Modules;
    FactoryModules() = delete;
    FactoryModules(KEY_FACTORY FactoryID);
    FactoryModules(const FactoryModules &) = delete;
  public:
    const KEY_FACTORY FactoryID;
    const std::vector < KEY_MODULE > GetModules();
    bool SetModule(int IndexSlot, KEY_MODULE key);
    friend Factory;
  };

  class Factory : public ItemBase, public Jsonable {
  private:
    double _Speed;
    double _Power;

    int _CountSlotsForModules;
    int _CountSlotsForRecipes;

    //!Износ от 0.0 до 1.0
    double _Wear;

    KEY_FACTORY _Key;

    TYPE_FACTORY _Type;

    Factory();
    Factory(const Factory &recipe);

  public:

    Factory(std::string const &Name, double Speed, double Power = 1.0);
    ~Factory();

    KEY_FACTORY GetKey() const;

    void InitFactoryModules(FactoryModules &) const;

    int ReadFromJson(const Json::Value & jsonPr) override;
    int WriteToJson(Json::Value & jsonPr) const override;

    friend std::pair<const KEY_FACTORY, Factory>;

  };

}
