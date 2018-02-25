#ifndef RecipeCollectionH
#define RecipeCollectionH

#include <set>
#include <map>
#include <algorithm>

#include "Recipe.h"

namespace ResourceCalculator {

  class RecipeCollection;
  class ParamsCollection;
  struct RecipeResultTree;

  struct ItemResultTree
  {
  private:
    KEY_ITEM _ItemKey;
    std::map<KEY_RECIPE, RecipeResultTree> _Result;
  public:
    const std::map<KEY_RECIPE, RecipeResultTree> & GetResult() const;
    KEY_ITEM GetItemKey() const;
    friend RecipeCollection;
    friend std::pair<const KEY_RECIPE, RecipeResultTree>;
  };

  struct RecipeResultTree
  {
  private:
    KEY_RECIPE _RecipeKey;
    std::map<KEY_ITEM, ItemResultTree> _Result;
  public:
    const std::map<KEY_ITEM, ItemResultTree> & GetResult() const;
    KEY_RECIPE GetRecipeKey() const;
    friend RecipeCollection;
  };

  template <typename key_type>
  class ResultElement
  {
  private:
    std::map <key_type, int> _Levels;
  public:
    inline ResultElement()
    {
    }
    inline void AddLevel(key_type Type, int Level)
    {
      auto f = _Levels.find(Type);
      if (f == _Levels.end()) {
        _Levels[Type] = Level;
      } else {
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
      for (auto IT: _Levels) {
        if (IT.second > Max) Max = IT.second;
        if (IT.second < Min) Min = IT.second;
      }
      for (int i = Max; i >= Min; i--) {
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


  class RecipeCollection: public Jsonable
  {
  public:

    RecipeCollection();

    ItemResultTree   BuildTree(KEY_ITEM ItemID,     int NestingResults, 
      std::list<KEY_ITEM> &ListRequest, std::list<KEY_ITEM> &ListRequestResourceOnly ) const;
    RecipeResultTree BuildTree(KEY_RECIPE RecipeID, int NestingResults,
      std::list<KEY_ITEM> &ListRequest, std::list<KEY_ITEM> &ListRequestResourceOnly ) const;

    void Travelling(const RecipeResultTree &Tree, int Nesting, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                    ResultElement<KEY_RECIPE> &ResultRecipes, ResultElement<KEY_ITEM> &ResultItems) const;

    void Travelling(const ItemResultTree &Tree, int Nesting, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                    ResultElement<KEY_RECIPE> &ResultRecipes, ResultElement<KEY_ITEM> &ResultItems) const;

    void Travelling(const RecipeResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                    std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;

    void Travelling(const ItemResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
                    std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;

    void Build(KEY_ITEM ItemID, const std::map<KEY_ITEM, KEY_RECIPE> SelectRecipe, std::list<KEY_RECIPE> &ResultRecipe, std::set<KEY_ITEM> &ResultItem);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const std::map<KEY_RECIPE, Recipe> &GetData() const;
    std::map<KEY_RECIPE, Recipe> &GetDataForEdit();

    Recipe *GetRecipeForEdit(KEY_RECIPE KeyRecipe);
    const Recipe *GetRecipe(KEY_RECIPE KeyRecipe) const;

    void Delete( const std::set<KEY_ITEM>& ItemsKeysToDel );
    void Delete( const std::set<KEY_RECIPE>& RecipsKeysToDel );

    KEY_RECIPE GetUniqueRecipeKey();

    void Add(const Recipe &);

  private:
    TYPE_KEY _LastGenGey;
    std::map<KEY_RECIPE, Recipe> _Recipes;

  };

}

#endif // !RecipeCollectionH
