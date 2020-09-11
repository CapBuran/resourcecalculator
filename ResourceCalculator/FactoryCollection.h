#pragma once

#include <memory>

#include "Factory.h"

namespace ResourceCalculator
{
  struct FactoryType
  {
    std::string Name;
    std::string IconPath;
  };

  class FactoryCollection;

  class FactoryTypeCollection: public Jsonable, public Indexator<KEY_TYPE_FACTORY, FactoryType>
  {
  public:
    FactoryTypeCollection(FactoryCollection& owner);
    virtual ~FactoryTypeCollection();

    void CloneTo(FactoryTypeCollection& ref) const;

    const FactoryType& GetFactoryType(KEY_TYPE_FACTORY Key) const;
    FactoryType& GetFactoryType(KEY_TYPE_FACTORY Key);

    void AddFactorysTypes(std::map<KEY_TYPE_FACTORY, FactoryType > FactoryTypes);
    void DeleteFactorysTypes(std::set<KEY_TYPE_FACTORY> FactoryTypesKey);

    int ReadFromJson(const Json::Value& jsonPr) override;
    int WriteToJson(Json::Value& jsonPr) const override;

  private:
    FactoryType _UNKNOWN_FactoryType;
    FactoryCollection& _Owner;
    std::map<KEY_TYPE_FACTORY, FactoryType> _TypesFactory;
  };

  class FactoryCollection: public Jsonable, public Indexator<KEY_FACTORY, Factory>
  {
  public:
    FactoryCollection();
    virtual ~FactoryCollection();
    FactoryCollection(const FactoryCollection& copy);
    FactoryCollection& operator=(const FactoryCollection& fc);

    const Factory& GetFactory(KEY_FACTORY Key) const;
    Factory& GetFactoryForEdit(KEY_FACTORY Key);

    void AddFactorys(std::map<KEY_FACTORY, Factory> factorys);
    void DeleteFactorys( std::set<KEY_FACTORY> factoresKey);

    int ReadFromJson( const Json::Value &jsonPr ) override;
    int WriteToJson( Json::Value &jsonPr ) const override;

    std::map<KEY_FACTORY, Factory> GetFactoryByConditions(std::function<bool(const Factory&)> func) const;

    void DeleteTypes(std::set<KEY_TYPE_FACTORY> deletes);

    FactoryTypeCollection& GetTypes();
    const FactoryTypeCollection& GetTypes() const;

  private:
    Factory _NoFindFactory;
    std::map<KEY_FACTORY, Factory> _Factorys;
    std::unique_ptr<FactoryTypeCollection> _Types;
  };

}
