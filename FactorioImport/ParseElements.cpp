#include <assert.h>

#include <iostream>
#include <fstream>

#include "ParseElements.h"

namespace FactorioImport
{

  template<class T>
  int ReadJsonableFromJsonToList(const Json::Value& jsonPr, std::list<T>& DST)
  {
    DST.clear();
    for (auto& it: jsonPr)
    {
      T ToAdd;
      ToAdd.ReadFromJson(it);
      DST.push_back(ToAdd);
    }
    return 0;
  }

  template<class T>
  int ReadJsonableFromJsonToMapKeyName(const Json::Value& jsonPr, std::map<std::string, T>& DST)
  {
    DST.clear();
    for (auto& it : jsonPr)
    {
      T ToAdd;
      ToAdd.ReadFromJson(it);
      DST[ToAdd.name] = ToAdd;
    }
    return 0;
  }

  int ReadFromJsonToList(const Json::Value& jsonPr, std::list<std::string>& DST)
  {
    DST.clear();
    for (auto& it: jsonPr)
    {
      DST.push_back(it.asString());
    }
    return 0;
  }

  int ReadFromJsonToList(const Json::Value& jsonPr, std::map<std::string, bool>& DST)
  {
    DST.clear();
    auto keys = jsonPr.getMemberNames();

    for (auto& it : keys)
    {
      DST[it] = jsonPr[it].asBool();
    }
    return 0;
  }

  int ReadFromJsonToList(const Json::Value& jsonPr, std::map<std::string, std::string>& DST)
  {
    DST.clear();
    auto keys = jsonPr.getMemberNames();

    for (auto& it : keys)
    {
      DST[it] = jsonPr[it].asString();
    }
    return 0;
  }

  int BaseImport::ParseLocalised(const std::string& Line)
  {
    enum class State
    {
      Name,
      LocalisedName,
      LocalisedDescription,
      End
    } state = State::Name;

    for (size_t i = 0; i < Line.length(); i++)
    {
      if (state == State::End) break;
      char ch = Line[i];
      if (ch == '|') {
        if (state == State::Name)
        {
          state = State::LocalisedName;
          continue;
        }
        if (state == State::LocalisedName)
        {
          state = State::LocalisedDescription;
          continue;
        }
        if (state == State::LocalisedDescription)
        {
          state = State::End;
          continue;
        }
        state = State::End;
      }

      switch (state)
      {
      case State::Name:
        name += ch;
        break;
      case State::LocalisedName:
        localised_name += ch;
        break;
      case State::LocalisedDescription:
        localised_description += ch;
        break;
      default:
        break;
      }
    }

    return 0;
  }

  int BaseImport::ReadFromJson(const Json::Value& jsonPr)
  {
    name = jsonPr["name"].asString();
    //localised_name = jsonPr["localised_name"].asString();
    return 0;
  }

  int BaseImport::WriteToJson(Json::Value& jsonPr) const
  {
    return 0;
  }

  int RecipeImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    category = jsonPr["category"].asString();
    time = jsonPr["energy"].asDouble();
    ReadJsonableFromJsonToList(jsonPr["ingredients"], ingredients);
    ReadJsonableFromJsonToList(jsonPr["products"], products);
    main_product.ReadFromJson(jsonPr["main_product"]);
    return 0;
  }

  int ItemImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    type = jsonPr["type"].asString();
    stack_size = jsonPr["stack_size"].asInt();
    fuel_value = jsonPr["fuel_value"].asDouble();
    fuel_category = jsonPr["fuel_category"].asString();
    category = jsonPr["category"].asString();
    fuel_acceleration_multiplier = jsonPr["fuel_acceleration_multiplier"].asDouble();
    fuel_top_speed_multiplier = jsonPr["fuel_top_speed_multiplier"].asDouble();
    burnt_result = jsonPr["burnt_result"].asString();
    tier = jsonPr["tier"].asInt();
    ReadJsonableFromJsonToList(jsonPr["module_effects"], module_effects);
    ReadFromJsonToList(jsonPr["limitations"], limitations);
    return 0;
  }

  int AssemblingMachineImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    type = jsonPr["type"].asString();
    energy_usage = jsonPr["energy_usage"].asDouble();
    ingredient_count = jsonPr["ingredient_count"].asDouble();
    crafting_speed = jsonPr["crafting_speed"].asDouble();
    drain = jsonPr["drain"].asDouble();
    module_inventory_size = jsonPr["module_inventory_size"].asInt();
    ReadFromJsonToList(jsonPr["crafting_categories"], crafting_categories);
    ReadFromJsonToList(jsonPr["allowed_effects"], allowed_effects);
    return 0;
  }

  int BoilerImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    max_energy_usage = jsonPr["max_energy_usage"].asDouble();
    target_temperature = jsonPr["target_temperature"].asDouble();
    burner_effectivity = jsonPr["burner_effectivity"].asDouble();
    input_fluid = jsonPr["input_fluid"].asString();
    output_fluid = jsonPr["output_fluid"].asString();
    ReadFromJsonToList(jsonPr["fuel_categories"], fuel_categories);
    return 0;
  }

  int FurnaceImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    type = jsonPr["type"].asString();
    energy_usage = jsonPr["energy_usage"].asDouble();
    crafting_speed = jsonPr["crafting_speed"].asDouble();
    burner_effectivity = jsonPr["burner_effectivity"].asDouble();
    ingredient_count = jsonPr["ingredient_count"].asInt();
    module_inventory_size = jsonPr["module_inventory_size"].asInt();
    ReadFromJsonToList(jsonPr["crafting_categories"], crafting_categories);
    ReadFromJsonToList(jsonPr["fuel_categories"], fuel_categories);
    ReadFromJsonToList(jsonPr["allowed_effects"], allowed_effects);
    return 0;
  }

  int TransportBeltImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    belt_speed = jsonPr["belt_speed"].asDouble();
    return 0;
  }

  int ReactorImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    max_energy_usage = jsonPr["max_energy_usage"].asDouble();
    neighbour_bonus = jsonPr["neighbour_bonus"].asDouble();
    burner_effectivity = jsonPr["burner_effectivity"].asDouble();
    max_energy_output = jsonPr["max_energy_output"].asDouble();
    ReadFromJsonToList(jsonPr["fuel_categories"], fuel_categories);
    return 0;
  }

  int MiningDrillImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    energy_usage = jsonPr["energy_usage"].asDouble();
    mining_speed = jsonPr["mining_speed"].asDouble();
    burner_effectivity = jsonPr["burner_effectivity"].asDouble();
    ReadFromJsonToList(jsonPr["resource_categories"], resource_categories);
    ReadFromJsonToList(jsonPr["allowed_effects"], allowed_effects);
    return 0;
  }

  int GeneratorImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    effectivity = jsonPr["effectivity"].asDouble();
    maximum_temperature = jsonPr["maximum_temperature"].asDouble();
    fluid_usage_per_tick = jsonPr["fluid_usage_per_tick"].asDouble();
    max_energy_output = jsonPr["max_energy_output"].asDouble();
    return 0;
  }

  int FluidImport::ReadFromJson(const Json::Value& jsonPr)
  {
    BaseImport::ReadFromJson(jsonPr);
    default_temperature = jsonPr["default_temperature"].asDouble();
    max_temperature = jsonPr["max_temperature"].asDouble();
    fuel_value = jsonPr["fuel_value"].asDouble();
    return 0;
  }

  int RecipeComponentImport::ReadFromJson(const Json::Value& jsonPr)
  {
    type = jsonPr["type"].asString();
    name = jsonPr["name"].asString();
    amount = jsonPr["amount"].asDouble();
    if (jsonPr.isMember("probability"))
    {
      amount *= jsonPr["probability"].asDouble();
    }

    return 0;
  }

  int RecipeComponentImport::WriteToJson(Json::Value& jsonPr) const
  {
    return 0;
  }

  int ModuleEffect::ReadFromJson(const Json::Value& jsonPr)
  {
    effect_category = jsonPr["effect_category"].asString();
    bonus = jsonPr["bonus"].asDouble();
    return 0;
  }

  int ModuleEffect::WriteToJson(Json::Value& jsonPr) const
  {
    return 0;
  }


  int AllImport::ReadFromFile(std::string FullPath, Type type)
  {
    std::ifstream in(FullPath);
    if (in.is_open())
    {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      in.clear();

      Json::Reader JsonReader;
      Json::Value jsonPr;
      bool parsingSuccessful = JsonReader.parse(contents.c_str(), jsonPr);
      contents.clear();

      switch (type)
      {
      case Type::ItemImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Items);
        break;
      case Type::RecipeImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Recipes);
        break;
      case Type::AssemblingMachineImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, AssemblingMachines);
        break;
      case Type::BoilerImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Boilers);
        break;
      case Type::FurnaceImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Furnaces);
        break;
      case Type::FluidImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Fluids);
        break;
      case Type::GeneratorImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Generators);
        break;
      case Type::MiningDrillImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, MiningDrills);
        break;
      case Type::ReactorImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, Reactors);
        break;
      case Type::TransportBeltImport:
        ReadJsonableFromJsonToMapKeyName(jsonPr, TransportBelts);
        break;
      default:
        break;
      }
    }

    return 0;
  }

  template<class T>
  void SetParsingLocalisedData(const std::list<BaseImport>& base, std::map<std::string, T>& custom)
  {
    for (const auto& b: base)
    {
      custom[b.name].localised_name = b.localised_name;
      custom[b.name].localised_description = b.localised_description;
    }
  }

  int AllImport::ReadLocalised(std::string FullPath, Type type)
  {
    std::ifstream file(FullPath);

    if (!file.is_open()) return -1;

    std::list<BaseImport> ParsingLocalised;
    std::string line;

    while (std::getline(file, line)) {
      BaseImport bi;
      bi.ParseLocalised(line);
      ParsingLocalised.push_back(bi);
    }

    switch (type)
    {
    case FactorioImport::AllImport::Type::ItemImport:
      SetParsingLocalisedData(ParsingLocalised, Items);
      break;
    case FactorioImport::AllImport::Type::RecipeImport:
      SetParsingLocalisedData(ParsingLocalised, Recipes);
      break;
    case FactorioImport::AllImport::Type::AssemblingMachineImport:
      SetParsingLocalisedData(ParsingLocalised, AssemblingMachines);
      break;
    case FactorioImport::AllImport::Type::BoilerImport:
      SetParsingLocalisedData(ParsingLocalised, Boilers);
      break;
    case FactorioImport::AllImport::Type::FurnaceImport:
      SetParsingLocalisedData(ParsingLocalised, Furnaces);
      break;
    case FactorioImport::AllImport::Type::FluidImport:
      SetParsingLocalisedData(ParsingLocalised, Fluids);
      break;
    case FactorioImport::AllImport::Type::GeneratorImport:
      SetParsingLocalisedData(ParsingLocalised, Generators);
      break;
    case FactorioImport::AllImport::Type::MiningDrillImport:
      SetParsingLocalisedData(ParsingLocalised, MiningDrills);
      break;
    case FactorioImport::AllImport::Type::ReactorImport:
      SetParsingLocalisedData(ParsingLocalised, Reactors);
      break;
    case FactorioImport::AllImport::Type::TransportBeltImport:
      SetParsingLocalisedData(ParsingLocalised, TransportBelts);
      break;
    default:
      break;
    }

    return 0;
  }

  template<class T>
  void SetIcon(const std::map<std::string, std::string>& allIcons, std::map<std::string, T>& elements)
  {
    for (auto& it: elements)
    {
      auto f = allIcons.find(it.first);
      if (f != allIcons.end())
      {
        it.second.icon_key = f->second;
      }
    }
  }

  int AllImport::SetIcons(const std::list<IconsLog>& icons)
  {
    std::map<std::string, std::string> allIcons;
    for (const auto& icon : icons)
    {
      if (icon.Icons.size() > 0)
      {
        allIcons[icon.Name] = icon.Icons.begin()->Path;
      }
      else
      {
        if (allIcons.count(icon.Name) == 0)
        {
          allIcons[icon.Name] = "";
        }
      }
    }

    SetIcon(allIcons, Items);
    SetIcon(allIcons, Recipes);
    SetIcon(allIcons, AssemblingMachines);
    SetIcon(allIcons, Boilers);
    SetIcon(allIcons, Furnaces);
    SetIcon(allIcons, Fluids);
    SetIcon(allIcons, Generators);
    SetIcon(allIcons, MiningDrills);
    SetIcon(allIcons, Reactors);
    SetIcon(allIcons, TransportBelts);

    return 0;
  }

  int AllImport::ReadAll(std::string FullPath)
  {
    ReadFromFile(FullPath + "/item.json", Type::ItemImport);
    ReadFromFile(FullPath + "/recipe.json", Type::RecipeImport);
    ReadFromFile(FullPath + "/assembling-machine.json", Type::AssemblingMachineImport);
    ReadFromFile(FullPath + "/boiler.json", Type::BoilerImport);
    ReadFromFile(FullPath + "/furnace.json", Type::FurnaceImport);
    ReadFromFile(FullPath + "/fluid.json", Type::FluidImport);
    ReadFromFile(FullPath + "/generator.json", Type::GeneratorImport);
    ReadFromFile(FullPath + "/mining-drill.json", Type::MiningDrillImport);
    ReadFromFile(FullPath + "/reactor.json", Type::ReactorImport);
    ReadFromFile(FullPath + "/transport-belt.json", Type::TransportBeltImport);

    ReadLocalised(FullPath + "/item.txt", Type::ItemImport);
    ReadLocalised(FullPath + "/recipe.txt", Type::RecipeImport);
    ReadLocalised(FullPath + "/assembling-machine.txt", Type::AssemblingMachineImport);
    ReadLocalised(FullPath + "/boiler.txt", Type::BoilerImport);
    ReadLocalised(FullPath + "/furnace.txt", Type::FurnaceImport);
    ReadLocalised(FullPath + "/fluid.txt", Type::FluidImport);
    ReadLocalised(FullPath + "/generator.txt", Type::GeneratorImport);
    ReadLocalised(FullPath + "/mining-drill.txt", Type::MiningDrillImport);
    ReadLocalised(FullPath + "/reactor.txt", Type::ReactorImport);
    ReadLocalised(FullPath + "/transport-belt.txt", Type::TransportBeltImport);

    return 0;
  }

}