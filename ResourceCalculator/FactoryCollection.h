#ifndef FactoryCollectionH
#define FactoryCollectionH

#include "Factory.h"

namespace ResourceCalculator {
  struct FactoryType {
    std::string Name;
    std::string IconPath;
  };

  class FactoryCollection: public Jsonable
  {
  public:
    FactoryCollection();
    ~FactoryCollection();

    void ADD(const Factory);

    const Factory& GetFactory( KEY_FACTORY Key ) const;
    Factory& GetFactoryForEdit( KEY_FACTORY Key );

    const std::map<KEY_TYPE_FACTORY, FactoryType >& GetTypesFactorys() const;

    const std::map<KEY_FACTORY, Factory>& GetFactorys() const;

    void DeleteFactorysTypes( const std::list<KEY_TYPE_FACTORY> &FactoryTypesKey );
    void AddFactorysTypes( const std::map<KEY_TYPE_FACTORY, FactoryType > &FactoryTypes );

    KEY_FACTORY GetUniqueFactoryKey();
    KEY_TYPE_FACTORY GetUniqueFactoryTypeKey();

    int ReadFromJson( const Json::Value &jsonPr ) override;
    int WriteToJson( Json::Value &jsonPr ) const override;

    int ReadFromJsonFactoryTypes( const Json::Value &jsonPr );
    int WriteToJsonFactoryTypes( Json::Value &jsonPr ) const;

  private:
    KEY_FACTORY _LastRetvalUniqueFactoryKey;
    KEY_TYPE_FACTORY _LastRetvalUniqueFactoryTypeKey;

    std::map<KEY_FACTORY, Factory> _Factorys;
    std::map<KEY_TYPE_FACTORY, FactoryType > _TypesFactory;
    Factory _NoFindFactory;
  };

}

#endif // !FactoryCollectionH
