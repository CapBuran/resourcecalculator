#ifndef RecipeCollectionH
#define RecipeCollectionH

#include <set>

#include "Recipe.h"

namespace ResourceCalculator {

  class RecipeCollection;
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
    friend std::pair<const KEY_ITEM, ItemResultTree>;
  };

  class RecipeCollection: public Jsonable
  {
  public:

    ItemResultTree   BuildTree(KEY_ITEM ItemID,     int NestingResults) const;
    RecipeResultTree BuildTree(KEY_RECIPE RecipeID, int NestingResults) const;

    void Travelling(const RecipeResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer, 
      std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;

    void Travelling(const ItemResultTree &Tree, const std::map<KEY_ITEM, KEY_RECIPE> &Ansfer,
      std::list <KEY_RECIPE> &ResultRecipes, std::list <KEY_ITEM> &ResultItems) const;

    void Build(KEY_ITEM ItemID, const std::map<KEY_ITEM, KEY_RECIPE> SelectRecipe, std::list<KEY_RECIPE> &ResultRecipe, std::set<KEY_ITEM> &ResultItem);

    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;

    const std::map<KEY_RECIPE, Recipe> &GetData() const;

    Recipe *GetRecipeForEdit(KEY_RECIPE KeyRecipe);
    const Recipe *GetRecipe(KEY_RECIPE KeyRecipe) const;

    KEY_RECIPE GetUniqueRecipeKey() const;

    void Add(const Recipe &);

    void Delete(KEY_RECIPE KeyRecipe);

  private:
    std::map<KEY_RECIPE, Recipe> _Recipes;
 
  };

}

#endif // !RecipeCollectionH
