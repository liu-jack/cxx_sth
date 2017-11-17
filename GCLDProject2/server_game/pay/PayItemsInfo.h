#ifndef PayItemsInfo_h__
#define PayItemsInfo_h__

#include "def/TypeDef.h"
#include <boost/shared_ptr.hpp>
struct PayItem ;
struct PayMailInfo ;
typedef boost::shared_ptr< PayItem >            PayItemPtr ;
typedef boost::shared_ptr< PayMailInfo >        PayMailInfoPtr ;

//struct PaySellTable
//{
//public:
//    typedef int Price ;
//
//public:
//    typedef std::map< Price, PayItemPtr > ItemList ;
//    typedef ItemList::iterator             ItemListItr ;
//    typedef ItemList::const_iterator       ItemListCItr ;
//
//public:
//    PayMailInfoPtr pMailInfo ;
//    ItemList       crystalItems ;
//    ItemList       crystalCardItems ;
//} ;


class PayItemsInfo
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Pay relative
public:
  /*  typedef int                                                    ChannelId ;
    typedef int                                                    PlatformId ;
    typedef std::pair< ChannelId, PlatformId >                     ChannelAndPlatformId ;

    typedef int                                                    PayGroupId ;
	typedef std::multimap< PayGroupId, PayItemPtr >                     AllPayItemTableByGroup ;*/

	typedef std::map< ProductId, PayItemPtr >                              AllPayItemTableById ;

    typedef uint64                                                 MaildId ;
    typedef std::map< MaildId, PayMailInfoPtr >                    AllPayMailInfoTable ;

    //typedef int                                                    MoneyType ;
    //typedef std::map< ChannelAndPlatformId, PaySellTable >         PaySellByChannelPlatformId ;
    //typedef std::map< MoneyType, PaySellByChannelPlatformId >      PaySellByMonyeType ;



public:
    PayItemsInfo(){ }

    void ClearAll();

    void LoadFromCURLRespon( const std::string& respon_str) ;

   // int GetSellTable( int moneyType, int channel, int platform, const PaySellTable *&pTable ) ;

    void SetRegionId( uint32 region) { m_regionId = region; }
    PayMailInfo* GetPayMailInfo( MaildId id ) const;
    PayItem* GetPayItem( ProductId id ) const;

private:    
    uint32                 m_regionId;
    //PaySellByMonyeType     m_sellTables ;
    AllPayItemTableById    m_allPayItemsById ;
    AllPayMailInfoTable    m_allPayMailInfos ;
};

#endif // PayItemsInfo_h__
