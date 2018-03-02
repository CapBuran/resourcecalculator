#ifndef ProductionChainTree_H
#define ProductionChainTree_H

#include <vector>
#include <list>
#include <map>

#include "ParamsCollection.h"

namespace ResourceCalculator {

template <typename key_type>
class ResultElement2
{
private:
  std::map <key_type, int> _Levels;
public:
  inline ResultElement2()
  {
  }
  inline void AddLevel(key_type Type, int Level)
  {
    auto f = _Levels.find(Type);
    if (f == _Levels.end()) {
      _Levels[Type] = Level;
    }
    else {
      if (f->second > Level) f->second = Level;
    }
  }
  inline std::list <key_type> GetList() const
  {
    std::list <key_type> Result;
    int Min = 0, Max = 0;
    if (_Levels.size() > 0) {
      Min = _Levels.begin()->second;
      Max = Min;
    }
    for (auto IT : _Levels) {
      if (IT.second > Max) Max = IT.second;
      if (IT.second < Min) Min = IT.second;
    }
    for (int i = Min; i <= Max; i++) {
      for (auto IT : _Levels) {
        if (i == IT.second) {
          auto F = std::find(Result.begin(), Result.end(), IT.first);
          if (F == Result.end()) {
            Result.push_back(IT.first);
          }
        }
      }
    }
    return Result;
  }
};

class RecipeResultTree2;

class ItemResultTree2
{
private:
  std::vector<KEY_RECIPE> _Result;
  void _Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, ResultElement2<KEY_RECIPE>& RetRecipes, ResultElement2<KEY_ITEM>& RetItems) const;

public:
  const KEY_RECIPE Parent;
  const KEY_ITEM   ItemID;
  const ParamsCollection &PC;
  ItemResultTree2(const ParamsCollection &PC, KEY_ITEM ItemID, KEY_RECIPE Parent = KEY_RECIPE::ID_RECIPE_NoFindRecipe);
  KEY_RECIPE GetChildren(size_t ID) const;
  size_t GetCountChildrens() const;
  void Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;
  friend RecipeResultTree2;
};

class RecipeResultTree2
{
private:
  void _Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, ResultElement2<KEY_RECIPE>& RetRecipes, ResultElement2<KEY_ITEM>& RetItems) const;
public:
  const KEY_ITEM   Parent;
  const KEY_RECIPE RecipeID;
  const ParamsCollection &PC;
  RecipeResultTree2(const ParamsCollection &PC, KEY_RECIPE RecipeID, KEY_ITEM Parent = KEY_ITEM::ID_ITEM_NoFind_Item);
  KEY_ITEM GetChildren(size_t ID) const;
  size_t GetCountChildrens() const;
  void Travelling(int Nesting, const std::map<KEY_ITEM, KEY_RECIPE>& Ansfer, std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;
  friend ItemResultTree2;
};

}

#endif //ProductionChainTree_H