#define NOMINMAX
#undef max
#undef min

#include "SessionHttpMgr.h"
#include "encrypt/md5.h"
#include "rapidjson/document.h"
#include "memory_buffer/NetPack.h"
#include "OS.h"
int SessionHttpManager::s_connectId =0;


SessionHttpManager::SessionHttpManager( void )
{
	m_pSSServer       =NULL ;
	m_preUpdateTimeMS =0 ;
	m_sleepTimeMS     =0 ;
}

SessionHttpManager::~SessionHttpManager( void )
{

}


void SessionHttpManager::WhenRunning( void )
{
	//ProcessInternalPackets() ;

	/*uint64 nowTime = sOS.TimeMS();
	int64 updateUseTime = nowTime - m_preUpdateTimeMS ; ASSERT( updateUseTime >= 0 ) ;
	int64 diffTime = m_sleepTimeMS - updateUseTime ;
	if( diffTime > 0 )
	{
		sOS.SleepMS( static_cast<unsigned long>( diffTime ) ) ;
	}
	m_preUpdateTimeMS =nowTime ;*/
}

SessionHttpManager& SessionHttpManager::GetInstance( void )
{
	static SessionHttpManager manager ;
	return manager ;
}


void HttpMainEventhandler( mg_connection *pConn, int eventId, void *pMsg )
{
	/*switch( eventId )
	{
	case MG_EV_POLL:
		{
		} break ;
		
	case MG_EV_HTTP_REQUEST:
		{
			sHttpMgr.ReplyError( pConn, 404, NULL ) ;
		} break ;

	case MG_EV_ACCEPT:
	case MG_EV_CONNECT:
	case MG_EV_HTTP_REPLY:
		{
		} break ;

	case MG_EV_CLOSE:
		{
			sHttpMgr.CheckWaitingConnect( 0 ) ;
		} break ;

	case MG_EV_TIMER:
		{
			bool isReomve =sHttpMgr.RemoveWaitingConnect( pConn ) ;
			assert( isReomve ) ;
			if( isReomve )
			{
				pConn->flags |= MG_F_SEND_AND_CLOSE ;
			}
		}

	case MG_EV_RECV:
	case MG_EV_SEND:
		{
		} break ;

	default:
		assert( false ) ;
	}*/
}

void HttpLoginEventhandler( mg_connection *pConn, int eventId, void *pMsg )
{
	//http_message *pHm = static_cast<http_message*>( pMsg ) ;

	//switch( eventId )
	//{
	//case MG_EV_POLL:
	//	{
	//	} break ;
	//	
	//case MG_EV_ACCEPT:
	//case MG_EV_CONNECT:
	//	{
	//	} break ;

	//case MG_EV_HTTP_CHUNK:
	//	{
	//	} break ;
	//case MG_EV_HTTP_REPLY:
	//	{
	//		bool isReomve =sHttpMgr.RemoveWaitingConnect( pConn ) ;
	//		assert( isReomve ) ;
	//		if( !isReomve )
	//		{
	//			break ;
	//		}

	//		//OutputMsg( rmNormal, "Send to %s is return", std::string( pHm->uri.p, pHm->uri.len ).c_str() );

	//		// 返回数据
	//		//{"ret":1,"msg":"SUCCESS","data":{"uid":"407767"}}
	//		//
	//		//ret为1时表成功，其他为失败。

	//		pConn->flags |= MG_F_CLOSE_IMMEDIATELY ;

	//		bool isOk =false ;
	//		assert( pConn->user_data != NULL ) ;
	//		if( pConn->user_data != NULL )
	//		{
	//			// 返回给Session 线程失败
	//			NetPack *pPack =static_cast<NetPack*>( pConn->user_data ) ;
	//			pConn->user_data =NULL ;

	//			if( pHm->resp_code == 200 )
	//			{
	//				rapidjson::Document document ;
	//				std::string buffer( pHm->body.p, pHm->body.len ) ;
	//				if( !document.ParseInsitu( &buffer[0] ).HasParseError() && document.IsObject() )
	//				{
	//					rapidjson::Document::MemberIterator retItr =document.FindMember( "ret" ) ;
	//					if( retItr != document.MemberEnd() && retItr->value.IsUint() )
	//					{
	//						unsigned int result =retItr->value.GetUint() ;
	//						if( result == 1 )
	//						{
	//							rapidjson::Document::MemberIterator dateItr =document.FindMember( "data" ) ;
	//							if( dateItr != document.MemberEnd() && dateItr->value.IsObject() )
	//							{
	//								rapidjson::Value &data =dateItr->value ;

	//								rapidjson::Document::MemberIterator uIdItr =data.FindMember( "uid" ) ;
	//								if( uIdItr != data.MemberEnd() && uIdItr->value.IsString() )
	//								{
	//									isOk =true ;
	//									*pPack << uIdItr->value.GetString() ;
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}
	//			else
	//			{
	//				std::string buffer( pHm->body.p, pHm->body.len ) ;
	//				//OutputMsg( rmError, "HttpLoginEventhandler login return %s(%d)", buffer.c_str(), pHm->resp_code );
	//			}

	//			if( !isOk )
	//			{
	//				*pPack << static_cast<unsigned int>( 0 ) ;
	//			}


	//			//OutputMsg( rmNormal, "sHttpMgr.GetSessionServer()->PostInternalPacket");
	//			sHttpMgr.GetSessionServer()->PostInternalPacket( pPack ) ;
	//		}
	//	} break ;

	//case MG_EV_CLOSE:
	//case MG_EV_TIMER:
	//	{
	//		bool isReomve =sHttpMgr.RemoveWaitingConnect( pConn ) ;
	//		if( isReomve )
	//		{
	//			pConn->flags |= MG_F_CLOSE_IMMEDIATELY ;

	//			assert( pConn->user_data != NULL ) ;
	//			if( pConn->user_data != NULL )
	//			{
	//				// 返回给Session 线程失败
	//				NetPack *pPack =static_cast<NetPack*>( pConn->user_data ) ;
	//				pConn->user_data =NULL ;

	//				*pPack << "" ;
	//				sHttpMgr.GetSessionServer()->PostInternalPacket( pPack ) ;

	//				//OutputMsg( rmError, "HttpLoginEventhandler login return time out" );
	//			}
	//		}
	//	} break ;

	//case MG_EV_RECV:
	//case MG_EV_SEND:
	//	{
	//	} break ;

	//default:
	//	assert( false ) ;
	//}
}

void SessionHttpManager::DispatchSessionInternalPacket( NetPack *pPacket )
{
	//switch( pPacket->GetOpcode() )
	//{
	//case jxInterSrvComm::SessionServerProto::sCheckPasswdResult:
	//	{
			// 先预分配返回包
			//NetPack *pRetPack =sHttpMgr.GetSessionServer()->AllocInternalPacket(666,pPacket->DynamicBodySize()) ;
			//pRetPack->SetPacketType( PACKET_TYPE_SERVER_HTTP ) ;
			//*pRetPack << pPacket->BodyRawBuffer();

			//// 取出tokenId
			//int gameServerId =0 ;
			//*pPacket >> gameServerId ;

			//std::string tokenid ;
			//*pPacket >> tokenid ;

			//HttpParamTool getPatam( HttpRequestTypeGet, GetLoginUrl(), "" ) ;
			//getPatam.AddParam( CS_STRINGIFY_MACRO( tokenid ), tokenid.c_str() ) ;

			//mg_connection *pConn =SendHttpRequest( HttpLoginEventhandler, getPatam, pRetPack ) ;

			////OutputMsg( rmNormal, "Send to %s param is: %s ", getPatam.m_url.c_str(), getPatam.m_paramBuf );

			//// 设置超时
			//if( !sHttpMgr.AddWaitingConnect( pConn, 30 ) )
			//{
			//	assert( false ) ;
			//	//OutputMsg( rmError, "SessionHttpManager add login connect pointer faild????" );
			//}
	//	} break ;

	//default:
	//	assert( false ) ;
	//}
}


