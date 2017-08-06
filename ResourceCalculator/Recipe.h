#ifndef RecipeH
#define RecipeH

#include "Types.h"

namespace ResourceCalculator
{

  struct CountsItem {
    KEY_ITEM ItemId;
    double   Count;
    CountsItem( KEY_ITEM item_id = KEY_ITEM::ID_ITEM_NoFind_Item, double count = 0.0 ):
      ItemId( item_id ), Count( count )
    {
    }
  };

  inline bool operator<( const CountsItem& lhs, const CountsItem& rhs )
  {
    return lhs.ItemId < rhs.ItemId;
  }

  class Recipe: public ItemBase {
  private:
    double      _Time;
    KEY_RECIPE  _Key;
    std::set<CountsItem> _Required;
    std::set<CountsItem> _Result;
    KEY_TYPE_FACTORY _TypeFactory;

  public:

    Recipe();

    DeclareAndDefinitionProperty( Key, KEY_RECIPE )
    DeclareAndDefinitionProperty( Time, double )
    DeclareAndDefinitionProperty( Required, std::set<CountsItem> )
    DeclareAndDefinitionProperty( Result, std::set<CountsItem> )
    DeclareAndDefinitionProperty( TypeFactory, KEY_TYPE_FACTORY )

    int ReadFromJson( const Json::Value & jsonPr ) override;
    int WriteToJson( Json::Value & jsonPr ) const override;

    bool DeleteItem( KEY_ITEM KeyItem );

  };

}
#endif//RecipeH
