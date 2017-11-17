#pragma once

#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "../Table/MapCity.h"
#include "utility/rank.h"
#include "../monster/MonsterCity.h"
#include "memory_buffer/MemoryPool.h"
#include "Country.pb.h"
#include "net_framework/Session.h"
class City;
class Player;
class CMap;
class LootList;
class ServerClient ;
class NetPack;

namespace google{
	namespace protobuf{
		class Message;
	}
}

//namespace pb
//{
//    class CountyInfo;
//	class Country_Levle_Up_Task_Rank;
//	class GS2C_Country_Task_Player;
//	class CrossWarInfo;
//}

struct PlayerCallRec{
	uint64 uniqueid;
	uint64 playerid;
};

struct NpcExpedition
{
	uint32 expeid;
	uint32 npcid;
};

struct CrossStarRec		//星数记录
{
	uint32 desCountryId;	//灭掉的国家id

};


struct CrossPlayer {
	CrossPlayer();
	uint64 playerId;
	int    rank; //自己在榜中的排位，1起始
	uint32 crossKill;
	bool   isTakeReward; //已领取

	int GetRankVal(){ return crossKill; }
};
typedef std::map<uint64, CrossPlayer> CrossPlayerMap;

class Country {
    DECLARE_OBJ_POOL(Country)

public:
	CrossPlayerMap		m_CrossPlayerLst;		//参与跨服玩家的信息
	Rank<CrossPlayer>				m_CrossRank;

public:
	std::set<uint16> m_setCity;
	const uint8 id;
    uint32      m_exp;
    uint8       m_level;
    uint16      m_donateTimes;  //捐赠次数
    uint16      m_countryNpcLv;
    uint32      m_countryNpcExp;
	uint64		m_lastCheckNpcExp;	//上次检验npc经验时间

    uint64      m_OpenLevelUpTaskTime;
    uint32      m_CurLevelDoTaskCnt;
    uint32      m_KillMonsterNum;
    bool        m_isMonsterTaskOpen;
    bool        m_isMonsterTaskSucceed;

	uint64      cross_war_start_time;//跨服战开时间
	uint64      cross_anabasis_start_time;//远征开始时间
	bool       isCrossAnabasisSide;//远征方,false为被远征方
	bool       isOpenedAnabasis;//是否已开启跨服远征
	bool       isOpenedCrossWar;//是否已开启跨服战
	string      anabasiServerIp;
	string      anabasiServerPort;
	string      crossServerIp;
	string      crossServerPort;
	pb::CROSS_STATE      cross_state;
	pb::CROSS_STATE      anabasis_state;
	uint64		cross_war_state_end_time;	//当前阶段结束时间
	uint64		cross_anabasis_state_end_time;	//当前阶段结束时间
	uint32		cross_uniqueID;	//服务器分配的id

	uint32		m_npclevelid;	//npc等级id，对应npc升级表
	uint32		m_tuntianLevel;	//当前屯田区等级
	uint64		m_tuntianExp;	//升级到下一级所需要的经验

	int		m_Prosperity_Degree;	//国家繁荣度
	uint32  OriGameServerId;	//原游戏服务器id
	uint32	OriGameCountryId;	//原游戏服务器国家id

	std::vector<CrossStarRec> m_crossStarRec;

    // 排行榜相关
    struct TaskPlayer {
        TaskPlayer();
        int    rank; //自己在榜中的排位，1起始
        int    statisticKill;
        bool   isTakeReward; //已领取

        int GetRankVal(){ return statisticKill; }
    };
    std::map<uint64, TaskPlayer>    m_playerLst;
    Rank<TaskPlayer>                m_RankLst;

    //蛮族城池：捐赠、发动入侵
    MonsterCity m_MonsterCity;

	std::vector<PlayerCallRec> m_player_call_rec;
	std::vector<PlayerCallRec> m_player_monster_call_rec;

	typedef std::vector<uint32>	m_expedition_npc;

	std::map<uint32 ,m_expedition_npc> m_npc_expedirion;
public:
	typedef boost::shared_ptr<ServerClient>   ServerClientPtr ;
public: //内存数据，无需入库
    uint16      m_capitalCityId;//首都
    uint16      m_monsterCityId;//对应的蛮族城池
    uint64      m_AttackNpc_1_RefreshTime;
    uint64      m_AttackNpc_2_RefreshTime;
	uint64		m_npc_refresh_time;
	ServerClientPtr m_curCrossServerPtr;

private:
	Session *m_pSession ;
	SessionAddress m_address ;

public:
    enum {
        AttackNpc_1_CD = 30*60,		//半小时
        AttackNpc_2_CD = 60*60,		//一小时
    };
   explicit Country(uint8 id_);
    ~Country();

    void Init();
	void SaveTo(pb::CountyInfo& msgData);
	void SaveCrossWarInfo(pb::CrossWarInfo &info );
	void LoadFrom(const pb::CountyInfo& msgData);
    void SaveTo(pb::Country_Levle_Up_Task_Rank& msgData);
    void LoadFrom(const pb::Country_Levle_Up_Task_Rank& msgData);
    void SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player);
	void UpdateCrossWarInfo(const pb::CrossWarInfo &info);
	uint32 RandOneCityForCelebrity();
	pb::CROSS_STATE GetCrossState();
	void  SetCrossState(pb::CROSS_STATE state);
	bool IsCanCrossWarState();
	void UpdateCurCrossServer();
	pb::CROSS_STATE GetAnabasisState();
	void SetAnabasisState(pb::CROSS_STATE state);
	void SetCrossUniqueID(uint32 uniqueid);
	uint32 GetCrossUniqueID() { return cross_uniqueID; }

	bool SendCrossServerNetPack(NetPack &pack);
	bool SendCrossServerMsg(const int opCode, const ::google::protobuf::Message &msg,char clientType,uint64 sessionId = 0);
	//升级相关
	uint8 GetLevel(){ return m_level; }
	bool LeveUp();
	void AddCountryExp(uint exp);
    void AddNpcExp(uint exp); //TODO：OnEnterNextDay()
    void Donate(Player& player);
    void ClearDonateCD(Player& player);

    //升级任务--蛮族试练
    bool OpenMonsterTask(Player& player);
    void _CloseMonsterTask();
    void AddKillMonsterNum(uint64 playerId, uint num);
    void _CheckMonsterTaskIsSucceed();
    void TakeMonsterTaskReward(Player& player);

    //己方城市被占领
    void OnCityBeOccupied(City& city, uint8 enemyCountryId);
    //占领别国城市
    void OnOccupyCity(City& city, uint8 destCountryId);

    void _RefreshAttackNpc(uint64 timeNowSec);		//刷新远征军
	void _RefreshJinWeiNpc(uint64 timeNowSec);		//刷新禁卫军
	void _RefreshCelebrityNpc(uint64 timeNow);					//
    const std::set<uint16>& GetCityLst(){ return m_setCity; }

    bool HaveSpecialCity(MapCity::SpecialCity e);
	bool IsOpenAnabasis();
	bool IsOpenCrossWar();
	uint32 GetCityCount();

	void updateNpcExpPerDay();

	void SetCrossConnectSession(const SessionAddress& sessionAdd);
	
	void SetGameSessionFromCross(Session* session) { m_pSession = session; }
	Session* GetGameSessionFromCross() { return m_pSession; }
	const SessionAddress& GetGameSessionAddress( void )  const {   return m_address ;   }
	void SetGameSessionAddress( const SessionAddress &address ) {   m_address =address ;   }

	void ModifyProsDegree(int mods);
	
	void AddCrossKill(uint64 playerId, uint32 cnt);

	bool m_isHasShield;
	uint64 m_shieldEndTime;
	void AddShieldTime();
	bool GetHasShield();
	uint64 GetShieldEndTime();

#ifdef _SERVER_CROSS_
	bool IsCrossCountryOccypied();
#endif

	uint32 GetCrossCountryDurability();

public:
	static void TakeWinReward(Player& player, int victimType);
	static void TakeRankReward(Player& player, int victimType, int rank);

    static LootList& GetWinReward(Player& player, int victimType);
    static LootList& GetRankReward(Player& player, int victimType, int rank);
};