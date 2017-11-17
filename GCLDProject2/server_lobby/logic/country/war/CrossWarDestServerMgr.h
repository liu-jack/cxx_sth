//////////////�����������������////////////////
//			�ù����������������
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

//enum CrossWarState	//���ս״̬
//{
//	State_Null,		//����״̬
//	State_Collect_Info,	//�ռ���Ϸ��������Ϣ
//	State_Devide,	//���������
//	State_Prepare,	//�ﱸ�׶�
//	State_Preview,	//Ԥ���׶�
//	State_Start,	//����������
//	State_Award,	//�콱�׶�
//};

struct ServerCountry{		//�μӷ���Ļ�����λ
	ServerCountry();
	int    rank;
	SessionPairId SessionPID;		//gameserver session id
	uint32	countryid;		//���ӵĹ���id
	uint32	Pros_degree;	//���ٶ�
	uint32	uniqueIndexID;	//�����Ψһid
	uint32	serverid;		//��Ϸ������id
	SessionAddress crossAddr;	//���ӵĿ��������ip,port

	int GetRankVal(){ return Pros_degree; }
};

//#define TIME_FROM_START_COLLECT_COUNTRY 10000		//10���ռ����й�����Ϣ
//#define TIME_FROM_START_PREPARE_OVER	(((12 + 7) * 60 + 50) * 60 * 1000)		//�ﱸ�׶ν���������Ԥ���׶�
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (10 * 60 * 1000))	//Ԥ���׶ν�������ʽ��ʼ���ս
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (90 * 60 * 1000))	//���ս����
//#define TIME_FROM_START_AWARD_OVER		(24 * 60 * 60 * 1000)	//�콱�׶ν���

//#define TIME_FROM_START_PREPARE_OVER	(GET_BASE_DEF_INT( pb::BD_CROSS_PREPARE_LAST_TIME) * 60 * 1000)		//�ﱸ�׶ν���������Ԥ���׶�
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_PREVIEW_LAST_TIME) * 60 * 1000))	//Ԥ���׶ν�������ʽ��ʼ���ս
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_FIGHT_LAST_TIME) * 60 * 1000))	//���ս����
//#define TIME_FROM_START_AWARD_OVER		(TIME_FROM_START_COMBAT_OVER + (GET_BASE_DEF_INT( pb::BD_CROSS_REWARD_LAST_TIME) * 60 * 1000))	//�콱�׶ν���

//#define TIME_FROM_START_PREPARE_OVER	(1 * 60 * 1000)									//�ﱸ�׶ν���������Ԥ���׶� 10����
//#define TIME_FROM_START_PREVIEW_OVER	(TIME_FROM_START_PREPARE_OVER + (2 * 60 * 1000))	//Ԥ���׶ν�������ʽ��ʼ���ս 10����
//#define TIME_FROM_START_COMBAT_OVER		(TIME_FROM_START_PREVIEW_OVER + (30 * 60 * 1000))	//���ս���� 10����
//#define TIME_FROM_START_AWARD_OVER		(TIME_FROM_START_COMBAT_OVER + (30 * 60 * 1000))	//�콱�׶ν��� 10����

class CrossWarDestServerMgr
{
public:

	typedef std::vector<ServerCountry> SCountryVec;
	typedef std::map< SessionPairId,SCountryVec >  SessionCountrys;

public:
	CrossWarDestServerMgr();
	static CrossWarDestServerMgr& Instance(){ static CrossWarDestServerMgr T; return T; }

	void Update( uint64 nowTimeMS, int64 diffMS ) ;

	void InitCrossWarInfo();		//��ʼǰ��������������Ϣ
	void StartCrossWarDest();	//�����������
	void NoticeWarStart();		//����Ϸ������֪ͨ���ʼ
	void NoticeWarState();		//֪ͨ�������׶�
	void CollectServerInfo();	//�������Ϸ�������������������������������Ϣ

	void HandleServerInfo(pb::SG2B_Country_Info& msg, const SessionPairId sessionid);	//�յ��������������ٶ���Ϣ
	void DevideServers();		//���ռ����ķ���������
	void RetCSToGS();			//���Ϳ��������ip����Ϸ��

	void GetCrossInfoInOneSession(uint32 serverid, pb::SL2G_DevideCrossServers& sendmsg);

private:
	void BroadcastToGS(int opCode, google::protobuf::Message& msg);

public:
	Rank<ServerCountry>		m_serRankCountrys;		//���в�����ҽ�����������Ϣ
	SCountryVec				m_SerCountrys;		//���й��ҵ���Ϣ
	SessionCountrys			m_DevideCountrys;

	pb::CROSS_STATE	m_CrossWarState;	//�׶�
	uint64			m_CrossWarStartTime;//���ս��ʼʱ�䣨�̶�ʱ�䣩
	//uint64			m_StateBeginTime;	//�׶ο�ʼʱ��
	uint64			m_NextStateTime;	//��һ�׶ο�ʼʱ��
	bool			m_isCrossWarBegin;	//��������Ƿ���
	uint32			m_ServerSize;		//���ӵ���Ϸ����������
	uint32			m_ServerSizeCheck;	//���ӵ���Ϸ����������	

private:
	uint64 m_time_from_start_collect_country;
	uint64 m_time_from_start_prepare_over;
	uint64 m_time_from_start_preview_over;
	uint64 m_time_from_start_combat_over;
	uint64 m_time_from_start_award_over;
};

bool SortByRank(const ServerCountry& country1, const ServerCountry& country2);

#define sCrossWarDestSerMgr CrossWarDestServerMgr::Instance()