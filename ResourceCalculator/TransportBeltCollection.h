#ifndef TransportCollectionH
#define TransportCollectionH

#include "TransportBelt.h"

namespace ResourceCalculator {

  class TransportBeltCollection : public Jsonable
  {
  public:
    TransportBeltCollection( );
    ~TransportBeltCollection();
    void Delete( const std::set<KEY_TRANSPORT_BELT> &ItemsKeysToDel );
    void Add( const std::map<KEY_TRANSPORT_BELT, TransportBelt > &Items );
    int ReadFromJson(const Json::Value &jsonPr) override;
    int WriteToJson(Json::Value &jsonPr) const override;
    const std::map<KEY_TRANSPORT_BELT, TransportBelt> &GetData() const;
    KEY_TRANSPORT_BELT GetUniqueItemKey();
  private:
    TYPE_KEY _LastGenGen;
    std::map<KEY_TRANSPORT_BELT, TransportBelt> _Items;
    
  };

}

#endif // !TransportCollectionH
