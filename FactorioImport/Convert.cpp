#include <string>
#include <list>
#include "Convert.h"

namespace FactorioImport
{
  Converter::Converter(const ParseLog& PL, const AllImport& allImport, ResourceCalculator::ParamsCollection& PC)
    : pl(PL)
    , all(allImport)
    , pc(PC)
  {
  }

  int Converter::Run()
  {

    std::map<ResourceCalculator::KEY_ITEM, std::string> ItemsKS;
    std::map<std::string, ResourceCalculator::KEY_ITEM> ItemsSK;
    std::map<ResourceCalculator::KEY_RECIPE, std::string> RecipeKS;
    std::map<std::string, ResourceCalculator::KEY_RECIPE> RecipeSK;
    std::map<ResourceCalculator::KEY_MODULE, std::string> ModuleKS;
    std::map<std::string, ResourceCalculator::KEY_MODULE> ModuleSK;
    std::map<ResourceCalculator::KEY_TYPE_FACTORY, std::string> TypeFactroryKS;
    std::map<std::string, ResourceCalculator::KEY_TYPE_FACTORY> TypeFactrorySK;
    std::map<ResourceCalculator::KEY_FACTORY, std::string> FactroryKS;
    std::map<std::string, ResourceCalculator::KEY_FACTORY> FactrorySK;

    for (auto& it: all.Items)
    {
      ResourceCalculator::KEY_ITEM key = pc.IC.GetUniqueItemKey();
      ItemsKS[key] = it.first;
      ItemsSK[it.first] = key;
      if (it.second.type == "module")
      {
        ResourceCalculator::KEY_MODULE key_module = pc.MC.GetUniqueModuleKey();
        ModuleKS[key_module] = it.first;
        ModuleSK[it.first] = key_module;
      }
    }

    for (auto& it : all.Fluids)
    {
      ResourceCalculator::KEY_ITEM key = pc.IC.GetUniqueItemKey();
      ItemsKS[key] = it.first;
      ItemsSK[it.first] = key;
    }

    for (auto& it: all.Recipes)
    {
      ResourceCalculator::KEY_RECIPE key_recipe = pc.RC.GetUniqueRecipeKey();
      RecipeKS[key_recipe] = it.first;
      RecipeSK[it.first] = key_recipe;
      TypeFactrorySK[it.second.category] = ResourceCalculator::KEY_TYPE_FACTORY::Unknown;
    }

    for (auto& it: all.Furnaces)
    {
      ResourceCalculator::KEY_FACTORY key_factory = pc.FC.GetUniqueFactoryKey();
      FactroryKS[key_factory] = it.first;
      FactrorySK[it.first] = key_factory;
      for (auto& cat: it.second.crafting_categories)
      {
        TypeFactrorySK[cat.first] = ResourceCalculator::KEY_TYPE_FACTORY::Unknown;
      }      
    }

    for (auto& it: all.AssemblingMachines)
    {
      ResourceCalculator::KEY_FACTORY key_factory = pc.FC.GetUniqueFactoryKey();
      FactroryKS[key_factory] = it.first;
      FactrorySK[it.first] = key_factory;
      for (auto& cat: it.second.crafting_categories)
      {
        TypeFactrorySK[cat.first] = ResourceCalculator::KEY_TYPE_FACTORY::Unknown;
      }
    }

    for (auto& it: TypeFactrorySK)
    {
      it.second = pc.FC.GetUniqueFactoryTypeKey();
      TypeFactroryKS[it.second] = it.first;
      ResourceCalculator::FactoryType FT;
      FT.Name = it.first;
      for (auto& f: all.AssemblingMachines)
      {
        for (auto& cat : f.second.crafting_categories)
        {
          if (cat.second == "true" && it.first == cat.first)
          {
            FT.IconPath = f.second.icon_key;
            break;
          }
        }
        if (!FT.IconPath.empty()) break;
      }
      for (auto& f: all.Furnaces)
      {
        for (auto& cat: f.second.crafting_categories)
        {
          if (cat.second == "true" && it.first == cat.second)
          {
            FT.IconPath = f.second.icon_key;
            break;
          }
        }
        if (!FT.IconPath.empty()) break;
      }
      pc.FC.ADD(it.second, FT);
    }
    
    for (auto& it: all.Furnaces)
    {
      ResourceCalculator::Factory F;
      std::set<ResourceCalculator::KEY_TYPE_FACTORY> keys;
      for (auto& cat : it.second.crafting_categories)
      {
        keys.insert(TypeFactrorySK[cat.first]);
      }

      F.SetKey(FactrorySK[it.second.name]);
      F.SetName(it.second.name);
      F.SetIconKey(it.second.icon_key);
      F.SetSpeed(it.second.crafting_speed);
      F.SetCountSlotsForModules(it.second.module_inventory_size);
      F.SetCountSlotsForRecipes(it.second.ingredient_count);
      F.SetWear(0.0);//TODO
      F.SetPower(it.second.energy_usage);
      F.SetLevelOfPollution(0.0);//TODO
      F.SetTypes(keys);

      pc.FC.ADD(F);
    }

    for (auto& it : all.AssemblingMachines)
    {
      ResourceCalculator::Factory F;
      std::set<ResourceCalculator::KEY_TYPE_FACTORY> keys;
      for (auto& cat : it.second.crafting_categories)
      {
        keys.insert(TypeFactrorySK[cat.first]);
      }

      F.SetKey(FactrorySK[it.second.name]);
      F.SetName(it.second.name);
      F.SetIconKey(it.second.icon_key);
      F.SetSpeed(it.second.crafting_speed);
      F.SetCountSlotsForModules(it.second.module_inventory_size);
      F.SetCountSlotsForRecipes(it.second.ingredient_count);
      F.SetWear(0.0);//TODO
      F.SetPower(it.second.energy_usage);
      F.SetLevelOfPollution(0.0);//TODO
      F.SetTypes(keys);

      pc.FC.ADD(F);
    }

    {
      auto IC_DATA = pc.IC.GetData();
      IC_DATA.clear();

      for (const auto item : all.Items)
      {
        ResourceCalculator::Item ItemToAdd;
        ResourceCalculator::KEY_ITEM ki = ItemsSK[item.first];
        ItemToAdd.SetKey(ki);
        ItemToAdd.SetMiningHardness(1.0);
        ItemToAdd.SetName(item.second.localised_name);
        ItemToAdd.SetIsALiquidOrGas(false);
        ItemToAdd.SetIconKey(item.second.icon_key);
        IC_DATA[ki] = ItemToAdd;
      }

      for (const auto item : all.Fluids)
      {
        ResourceCalculator::Item ItemToAdd;
        ResourceCalculator::KEY_ITEM ki = ItemsSK[item.first];
        ItemToAdd.SetKey(ki);
        ItemToAdd.SetMiningHardness(1.0);
        ItemToAdd.SetName(item.second.localised_name);
        ItemToAdd.SetIsALiquidOrGas(true);
        ItemToAdd.SetIconKey(item.second.icon_key);
        IC_DATA[ki] = ItemToAdd;
      }

      pc.IC.Add(IC_DATA);
    }

    {
      for (const auto recipe : all.Recipes)
      {
        ResourceCalculator::Recipe RecipeToAdd;
        ResourceCalculator::KEY_RECIPE kr = RecipeSK[recipe.first];
        RecipeToAdd.SetKey(kr);
        RecipeToAdd.SetName(recipe.second.localised_name);
        RecipeToAdd.SetIconKey(recipe.second.icon_key);


        RecipeToAdd.SetTime(recipe.second.time);
        RecipeToAdd.SetTypeFactory(TypeFactrorySK[recipe.first]);

        std::set<ResourceCalculator::CountsItem> CountsItemToSet;

        for (const auto ingredient : recipe.second.ingredients)
        {
          ResourceCalculator::CountsItem CountsItemToAdd;
          CountsItemToAdd.Count = ingredient.amount;
          CountsItemToAdd.ItemId = ItemsSK[ingredient.name];
          CountsItemToSet.insert(CountsItemToAdd);
        }

        RecipeToAdd.SetRequired(CountsItemToSet);
        CountsItemToSet.clear();

        for (const auto ingredient : recipe.second.products)
        {
          ResourceCalculator::CountsItem CountsItemToAdd;
          CountsItemToAdd.Count = ingredient.amount;
          CountsItemToAdd.ItemId = ItemsSK[ingredient.name];
          CountsItemToSet.insert(CountsItemToAdd);
        }

        RecipeToAdd.SetResult(CountsItemToSet);

        pc.RC.Add(RecipeToAdd);
      }
      
    }




    //std::map<std::string, RecipeImport> Recipes;
    //std::map<std::string, AssemblingMachineImport> AssemblingMachines;
    //std::map<std::string, BoilerImport> Boilers;
    //std::map<std::string, FurnaceImport> Furnaces;
    //std::map<std::string, FluidImport> Fluids;
    //std::map<std::string, GeneratorImport> Generators;
    //std::map<std::string, MiningDrillImport> MiningDrills;
    //std::map<std::string, ReactorImport> Reactors;
    //std::map<std::string, TransportBeltImport> TransportBelts;


    //FactoryCollection FC;
    //ModuleCollection  MC;
    //IconCollection Icons;
    //ProductionChainCollection PCC;
    //TransportBeltCollection TC;


    return 0;
  }
}
