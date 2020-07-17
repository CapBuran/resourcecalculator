#pragma once

#include <string>
#include <list>
#include <json/json.h>
#include <ParseLog.h>

namespace FactorioImport
{
  class BaseImport: public Jsonable
  {
  public:
    std::string name;
    std::string localised_name;
    std::string localised_description;
    std::string icon_key;
  public:
    int ParseLocalised(const std::string& Line);
    int ReadFromJson(const Json::Value& jsonPr) override;
    int WriteToJson(Json::Value& jsonPr) const override;
  };

  struct ModuleEffect: public Jsonable
  {
    std::string effect_category;
    double bonus = 0.0;
    virtual int ReadFromJson(const Json::Value& jsonPr) override;
    virtual int WriteToJson(Json::Value& jsonPr) const override;
  };

  class ItemImport: public BaseImport
  {
  public:
    std::string type;
    int stack_size = 0;
    double fuel_value = 0.0;
    std::string fuel_category;
    std::string category;
    double fuel_acceleration_multiplier = 0.0;
    double fuel_top_speed_multiplier = 0.0;
    std::string burnt_result;
    int tier = 0;
    std::list<ModuleEffect> module_effects;
    std::list<std::string> limitations;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  struct RecipeComponentImport: public Jsonable
  {
    std::string type;
    std::string name;
    double amount = 0.0;
    virtual int ReadFromJson(const Json::Value& jsonPr) override;
    virtual int WriteToJson(Json::Value& jsonPr) const override;
  };

  class RecipeImport: public BaseImport
  {
  public:
    std::string category;
    double time = 0.0;
    std::list<RecipeComponentImport> ingredients;
    std::list<RecipeComponentImport> products;
    RecipeComponentImport main_product;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class AssemblingMachineImport: public BaseImport
  {
  public:
    double energy_usage = 0.0;
    double ingredient_count = 0.0;
    double crafting_speed = 0.0;
    double drain = 0.0;
    int module_inventory_size = 0;
    std::string type;
    std::map<std::string, std::string> crafting_categories;
    std::map<std::string, bool> allowed_effects;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class BoilerImport : public BaseImport
  {
  public:
    double max_energy_usage = 0.0;
    double target_temperature = 0.0;
    double burner_effectivity = 0.0;
    std::string input_fluid;
    std::string output_fluid;
    std::map<std::string, std::string> fuel_categories;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class FurnaceImport: public BaseImport
  {
  public:
    std::string type;
    double energy_usage = 0.0;
    int ingredient_count = 0;
    double crafting_speed = 0.0;
    std::map<std::string, std::string> crafting_categories;
    int module_inventory_size = 0;
    double burner_effectivity = 1.0;
    std::map<std::string, std::string> fuel_categories;
    std::map<std::string, bool> allowed_effects;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class FluidImport : public BaseImport
  {
  public:
    double default_temperature = 0.0;
    double max_temperature = 0.0;
    double fuel_value = 0.0;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class GeneratorImport: public BaseImport
  {
  public:
    double effectivity = 0.0;
    double maximum_temperature = 0.0;
    double fluid_usage_per_tick = 0.0;
    double max_energy_output = 0.0;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class MiningDrillImport: public BaseImport
  {
  public:
    double energy_usage = 0.0;
    double mining_speed = 0.0;
    std::map<std::string, std::string> resource_categories;
    std::map<std::string, bool> allowed_effects;
    double burner_effectivity = 0.0;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class ReactorImport: public BaseImport
  {
  public:
    double max_energy_usage = 0.0;
    double neighbour_bonus = 0.0;
    double burner_effectivity = 0.0;
    std::map<std::string, bool> fuel_categories;
    double max_energy_output = 0.0;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class TransportBeltImport: public BaseImport
  {
  public:
    double belt_speed = 0.0;
  public:
    int ReadFromJson(const Json::Value& jsonPr) override;
  };

  class AllImport
  {
  public:
    enum class Type
    {
      ItemImport,
      RecipeImport,
      AssemblingMachineImport,
      BoilerImport,
      FurnaceImport,
      FluidImport,
      GeneratorImport,
      MiningDrillImport,
      ReactorImport,
      TransportBeltImport
    };

    std::map<std::string, ItemImport> Items;
    std::map<std::string, RecipeImport> Recipes;
    std::map<std::string, AssemblingMachineImport> AssemblingMachines;
    std::map<std::string, BoilerImport> Boilers;
    std::map<std::string, FurnaceImport> Furnaces;
    std::map<std::string, FluidImport> Fluids;
    std::map<std::string, GeneratorImport> Generators;
    std::map<std::string, MiningDrillImport> MiningDrills;
    std::map<std::string, ReactorImport> Reactors;
    std::map<std::string, TransportBeltImport> TransportBelts;

    int ReadFromFile(std::string FullPath, Type type);
    int ReadLocalised(std::string FullPath, Type type);
    int SetIcons(const std::list<IconsLog>& icons);
    int ReadAll(std::string FullPath);
  };

}
