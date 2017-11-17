#include "CURLRequests.h"


#include "GameServerHttpClients.h"
#include "http/curl/MultiCurl.h"
#include "GameServer.h"

#include "Web.pb.h"

CURLRequests::CURLRequests()
{
    m_isNeedToSend.flip(); // all set to need send; 

    m_requestSenders[REQUEST_OP_PAY_ITEM_INFO]          = &CURLRequests::SendPayItemRequest;
    m_requestSenders[REQUEST_OP_GLOBAL_KEY_VALUE]       = &CURLRequests::SendGlobalKeyValueRequest;
}



void CURLRequests::SendCURLRequest( int op_type ) 
{
    if ( op_type < REQUEST_OP_COUNT)
    {
        (this->*m_requestSenders[ op_type])();
    }
	SetIsNeedToSend(op_type, false);
}

bool CURLRequests::IsNeedToSend( int op_type ) const
{
	if ( op_type < REQUEST_OP_COUNT)
	{
		return m_isNeedToSend.test( op_type);
	}
    return false;
}

void CURLRequests::SetIsNeedToSend( int op_type, bool need)
{
	if ( op_type < REQUEST_OP_COUNT)
	{
		 m_isNeedToSend.set( op_type, need);
	}
}

void CURLRequests::SendPayItemRequest() const
{
    // request
    std::auto_ptr< MCRequest> pRequest ( new MCRequest( sHttpClients.GetWebUrl() + "/ipn_sell_info.php"));

    pRequest->m_id = sGameSvr.GetRegionId(); 
    pRequest->m_opType = REQUEST_OP_PAY_ITEM_INFO;

    //char tmp[25] ="" ;

    //sHttps.AppendParams( pRequest->m_url, "?payItemList=", "ok" ) ;
    //sHttps.AppendParams( pRequest->m_url, "&lang=", "TC" ) ;

    //sprintf( tmp, "%d", pb::SDK_ALL ) ;
    //sHttps.AppendParams( pRequest->m_url, "&channel=", tmp ) ;

    //sprintf( tmp, "%d", pb::ALL_PLATFORM ) ;
    //sHttps.AppendParams( pRequest->m_url, "&platform=", tmp ) ;

    //sHttps.AppendParams( pRequest->m_url, "&mail_info=", "1" ) ;

    sHttps.Request( pRequest.release() ) ;

    sHttps.PostRequest() ;
}

void CURLRequests::SendGlobalKeyValueRequest() const
{
    // request
    std::auto_ptr< MCRequest> pRequest ( new MCRequest( sHttpClients.GetWebUrl() + "/get_global_key_val.php"));

    pRequest->m_opType = REQUEST_OP_GLOBAL_KEY_VALUE;
    sHttps.Request( pRequest.release() ) ;

    sHttps.PostRequest() ;
}
