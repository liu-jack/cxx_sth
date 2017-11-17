#include "PayItemsInfo.h"

#include "Logger.h"

#include "GameServerHttpClients.h"
#include "http/curl/MultiCurl.h"


#include "data/PayItemTable.h"
#include "json/JsonTool.h"
#include "Web.pb.h"



//int PayItemsInfo::GetSellTable( int moneyType, int channel, int platform, const PaySellTable *&pTable )
//{
//    // 获取 对应 金币类型的列表
//    PaySellByMonyeType::iterator itrSellList =m_sellTables.find( moneyType ) ;
//    if( itrSellList == m_sellTables.end() )
//    {
//        return pb::CAN_NOT_FIND_MONEY_TYPE ;
//    }
//
//    // 进一步, 获取对应 平台与渠道的列表
//    PaySellByChannelPlatformId &byIdList =itrSellList->second ;
//
//    PaySellByChannelPlatformId::iterator byIdItr =byIdList.find( ChannelAndPlatformId( channel, platform ) ) ;
//    if( byIdItr == byIdList.end() )
//    {
//        return pb::CHANNEL_PLATFORM_ID_ERROR ;
//    }
//    pTable =&byIdItr->second ;
//
//    return pb::WEB_PAY_RERSULT_OK ;
//}

PayMailInfo* PayItemsInfo::GetPayMailInfo( MaildId id ) const
{
    AllPayMailInfoTable::const_iterator itr =m_allPayMailInfos.find( id ) ;
    return ( itr != m_allPayMailInfos.end() ? itr->second.get() : NULL ) ;
}

PayItem* PayItemsInfo::GetPayItem( ProductId id ) const
{
    AllPayItemTableById::const_iterator itr =m_allPayItemsById.find( id ) ;
    return ( itr != m_allPayItemsById.end() ? itr->second.get() : NULL ) ;
}

void PayItemsInfo::LoadFromCURLRespon( const std::string& respon_str )
{
    ClearAll();
    JsonTool jt ;
    if( jt.LoadFromStr( respon_str ) )
    {
        // Pay item
        JsonTool childItem =jt.GetChild( "pay_item" ) ;
        if( childItem.GetIsOk() && childItem.GetIsHadElement() )
        {
            JsonTool element =childItem.GetElement() ;
            while( element.GetIsOk() )
            {
                bool isAdd =true ;
                PayItemPtr ptr( new PayItem() ) ;

                ptr->iVal.id                =element.GetInt( "id" ) ;
				ptr->iVal.product_id.clear();
				element.GetString("product_id",ptr->iVal.product_id);
				ptr->iVal.platfrom			=element.GetInt( "platform" ) ;
                ptr->iVal.need_rmb          =element.GetFloat("need_rmb" ) ;
                ptr->iVal.reward_diamond    =element.GetInt( "reward_diamond" ) ;
				ptr->tex.clear();
                element.GetString( "image_name", ptr->tex ) ;
                if( isAdd )
                {
                    m_allPayItemsById.insert( std::make_pair( ptr->iVal.product_id, ptr ) ) ;
                }
                element =childItem.GetElement() ;
            }
        }

    //    // pay mail
    //    JsonTool childMail =jt.GetChild( "platform_mail" ) ;
    //    if( childMail.GetIsOk() && childMail.GetIsHadElement() )
    //    {
    //        JsonTool element =childMail.GetElement() ;
    //        while( element.GetIsOk() )
    //        {
    //            bool isAdd =true ;
    //            PayMailInfoPtr ptr( new PayMailInfo() ) ;

    //            ptr->id                =element.GetInt64( "id" ) ;
    //            element.GetString( "title", ptr->title ) ;
    //            element.GetString( "content", ptr->content ) ;
				//element.GetString( "items", ptr->items ) ;

    //            if( isAdd )
    //            {
    //                m_allPayMailInfos.insert( std::make_pair( ptr->id, ptr ) ) ;
    //            }

    //            element =childMail.GetElement() ;
    //        }
    //    }
	}
}

void PayItemsInfo::ClearAll()
{
   // m_sellTables.clear();
    m_allPayItemsById.clear() ;
    m_allPayMailInfos.clear() ;    
}


