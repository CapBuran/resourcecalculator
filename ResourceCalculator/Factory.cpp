#include "Factory.h"

namespace ResourceCalculator {

  Factory::Factory(const std::string &Name, double Speed, double Power):
    ItemBase(Name)
  {
    _Speed = 1.0;
    _Power = 0.0;
    _CountSlotsForModules = 0;
    _CountSlotsForRecipes = 0;
    _Wear = 0.0;
    _Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _Type = TYPE_FACTORY::Unknown;
  }

  Factory::Factory():
    ItemBase("")
  {
    _Speed = 1.0;
    _Power = 0.0;
    _CountSlotsForModules = 0;
    _CountSlotsForRecipes = 0;
    _Wear = 0.0;
    _Key = KEY_FACTORY::ID_ITEM_NoFind_Factory;
    _Type = TYPE_FACTORY::Unknown;
  }

  Factory::Factory(const Factory &factory):
    ItemBase(factory.GetName()), _Wear(0.0)
  {
    _Speed = factory._Speed;
    _Power = factory._Power;
    _CountSlotsForModules = factory._CountSlotsForModules;
    _CountSlotsForRecipes = factory._CountSlotsForRecipes;
    _Wear = factory._Wear;
    _Key = factory._Key;
    _Type = factory._Type;
  }

  Factory::~Factory()
  {
  }

  KEY_FACTORY Factory::GetKey() const
  {
    return _Key;
  }

  void Factory::InitFactoryModules(FactoryModules & FM) const
  {
    FM._Modules.resize(_CountSlotsForModules);
    for (auto &it : FM._Modules) it = KEY_MODULE::ID_CleanSlot;
  }

  int Factory::ReadFromJson(const Json::Value & jsonPr)
  {
    return 0;
  }

  int Factory::WriteToJson(Json::Value & jsonPr) const
  {
    return 0;
  }

}