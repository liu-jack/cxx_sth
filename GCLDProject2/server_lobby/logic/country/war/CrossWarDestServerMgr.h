//////////////跨服灭国活动服务器管理////////////////
//			该管理用来分配服务器
////////////////////////////////////////////////////
#pragma once
#include "def/TypeDef.h"
#include "utility/rank.h"
#include "BaseDefine.pb.h"
#include "../../../../net_framework/SessionTypeDef.h"
#include "../../../../net_framework/Session.h"
#include "InterServer.pb.h"

namespace pb{
	class SG2B_Country_Info;
}

//enum CrossWarState	//跨服战状态
//{
//	State_Null,		//空闲状态
//	State_Collect_Info,	//收集游戏服务器信息
//	State_Devide,	//分配服务器
//	State_Prepare,	//筹备阶段
//	State_Preview,	//预览阶段
//	State_Start,	//开启跨服灭国
//	State_Award,	//领奖阶段
//};

struct ServerCountry{		//参加分配的基本单位
	ServerCountry();
	int    rank;
	SessionPairId SessionPID;		//gameserver session id
	uint32	countryid;		//连接的国家id
	uint32	Pros_degree;	//繁荣度
	uint32	uniqueIndexID;	//分配的唯一id
	uint32	serverid;		//游戏服务器id
	SessionAddress crossAddr;	//连接的跨服服务器ip,port

	int GetRankVal(){ return Pros_degree; }
};

//#define TIME_FROM_START_COLLECT_COUNTRY 10000		//10秒收集所有国家信息
//#define TIME_FROM_START_PREPARE_OVER	(((12 + 7) * 60 + 50) * 60 * 1000)		//筹备阶段结束，进入预览阶段
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (10 * 60 * 1000))	//预览阶段结束，正式开始跨服战
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (90 * 60 * 1000))	//跨服战结束
//#define TIME_FROM_START_AWARD_OVER		(24 * 60 * 60 * 1000)	//领奖阶段结束

//#define TIME_FROM_START_PREPARE_OVER	(GET_BASE_DEF_INT( pb::BD_CROSS_PREPARE_LAST_TIME) * 60 * 1000)		//筹备阶段结束，进入预览阶段
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_PREVIEW_LAST_TIME) * 60 * 1000))	//预览阶段结束，正式开始跨服战
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_FIGHT_LAST_TIME) * 60 * 1000))	//跨服战结束
//#define TIME_FROM_START_AWARD_OVER		(TIME_FROM_START_COMBAT_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_REWARD_LAST_TIME) * 60 * 1000))	//领奖阶段结束

//#define TIME_FROM_START_PREPARE_OVER	(1 * 60 * 1000)									//筹备阶段结束，进入预览阶段 10分钟
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (2 * 60 * 1000))	//预览阶段结束，正式开始跨服战 10分钟
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (30 * 60 * 1000))	//跨服战结束 10分钟
//#define TIME_FROM_START_AWARD_OVER		(TIME_FROM_START_COMBAT_OVER + (30 * 60 * 1000))	//领奖阶段结束 10分钟

class CrossWarDestServerMgr
{
public:

	typedef std::vector<ServerCountry> SCountryVec;
	typedef std::map< SessionPairId,SCountryVec >  SessionCountrys;

public:
	CrossWarDestServerMgr();
	static CrossWarDestServerMgr& Instance(){ static CrossWarDestServerMgr T; return T; }

	void Update( uint64 nowTimeMS, int64 diffMS ) ;

	void InitCrossWarInfo();		//开始前、结束后清理信息
	void StartCrossWarDest();	//开启跨服灭国活动
	void NoticeWarStart();		//向游戏服与跨服通知活动开始
	void NoticeWarState();		//通知服务器阶段
	void CollectServerInfo();	//向各个游戏服务器发送数据以请求服务器国家信息

	void HandleServerInfo(pb::SG2B_Country_Info& msg, const SessionPairId sessionid);	//收到各个服务器繁荣度信息
	void DevideServers();		//将收集到的服务器分组
	void RetCSToGS();			//发送跨服服务器ip给游戏服

	void GetCrossInfoInOneSession(uint32 serverid, pb::SL2G_DevideCrossServers& sendmsg);

private:
	void BroadcastToGS(int opCode, google::protobuf::Message& msg);

public:
	Rank<ServerCountry>		m_serRankCountrys;		//所有参与国家进行排名的信息
	SCountryVec				m_SerCountrys;		//所有国家的信息
	SessionCountrys			m_DevideCountrys;

	pb::CROSS_STATE	m_CrossWarState;	//阶段
	uint64			m_CrossWarStartTime;//跨服战开始时间（固定时间）
	//uint64			m_StateBeginTime;	//阶段开始时间
	uint64			m_NextStateTime;	//下一阶段开始时间
	bool			m_isCrossWarBegin;	//跨服灭国活动是否开启
	uint32			m_ServerSize;		//连接的游戏服务器数量
	uint32			m_ServerSizeCheck;	//连接的游戏服务器计数	

private:
	uint64 m_time_from_start_collect_country;
	uint64 m_time_from_start_prepare_over;
	uint64 m_time_from_start_preview_over;
	uint64 m_time_from_start_combat_over;
	uint64 m_time_from_start_award_over;
};

bool SortByRank(const ServerCountry& country1, const ServerCountry& country2);

#define sCrossWarDestSerMgr CrossWarDestServerMgr::Instance()