#pragma once
#include "def/TypeDef.h"
#include "combat_define.h"
#include "memory_buffer/MemoryPool.h"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "Base.h"
#include "Combat.pb.h"
//TODO：同步战斗数据给客户端，广播仅前排几个同步具体数据，后面的让客户端想看自己取

class Player;
class Character;
namespace google{
    namespace protobuf{
        class Message;
    }
}
namespace pb
{
    class GS2C_Combat_Begin;
    class GS2C_Combat_End;
    class GS2C_City_Fight_Info;		//攻守方城池血量等信息
	class C2GS_Combat_Begin;
	class GS2C_Combat_Select_Tactic;
	class GS2C_Rush_Result;
	class GS2C_Combat_Runaway;
	class GS2C_Combat_Attract_Enemy;
	class GS2C_Combat_Call_Up;
	class GS2C_Combat_Call_Request;
	class GS2C_Combat_Pre_Begin;
	class GS2C_Combat_Select_Tactic_Single;
	class GS2C_Combat_Next_Lst;
	class C2GS_Combat_Call_Request;
	class Combat_All_Buff;
	//class Msg_return_common;
}

namespace Combat{

class CombatObj;

struct StatisticData {
    int      statisticExp;   //统计：战斗获得的总经验 //玩家经验
	int		 statisticExpPlayer;//统计：战斗获得的玩家总经验
    int      statisticMoney; //统计：战斗获得的总银币
    int      statisticDead;
    int      statisticKill;

	StatisticData(){ ZeroMemoryThis; }
	void add(const CombatObj& obj);
};
struct CombatGainsRec {
	uint32		combatPExp;		//玩家获得经验
	uint32		combatDeads;	//损失数
	uint32		combatKills;	//杀敌数
	uint32		combatExploit;	//功勋
	uint32		combatCoin;		//银币
	IntPairVec	combatExGains;	//额外获得奖励
	CombatGainsRec(){ ZeroMemoryThis; }
};
class CombatGroup {
    DECLARE_INDEX_POOL(CombatGroup, 128);
public:
	//战斗组固定参数  创建战斗组时确定
    // 外界保存战斗组的m_index，该战斗组结束后，索引再被另一场战斗复用
    // 再根据m_index找到的数据就错了，加上这个唯一id防范此情况
    uint32 m_unique_id;
	CombatType m_eCombat;	//战斗类型
	bool m_isAttractCombat;	//是否是诱敌战
	uint32 m_terrainType;	//地形

public:
	//战斗中变化参数
	//uint32 m_seige_army_count_;
    GroupLst m_group_attack;
    GroupLst m_group_defence;
    std::vector<uint64>   m_watchPlayer; //要同步的玩家		所有观战玩家，包括本玩家与旁观玩家
	std::set<uint64>		AttractCombat_attack_id;//诱敌时攻击玩家
	std::set<uint64>		AttractCombat_defence_id;//诱敌时防守玩家
    uint32 m_leftOperateSec; //手动操作剩余秒数，死一排即重置
	char m_leftFlashSec;   //client动画播放时间
	uint64 m_nextOperTime;	//倒计时结束时间
	bool m_isReady;			//是否准备好
	char m_battleDeleteTime;
	int m_battleDeletePrepareTime;

    std::map<uint64, StatisticData> m_statisticMap;	//统计数据表

	std::map<uint64, CombatGainsRec> m_combatGainsMap;	//新统计表

	bool m_isCombatEnd;		//战斗是否结束
	char m_combatEndLast;	//战斗结束后等待时间
	
	uint64 m_CurrentRound;	//当前回合数

	bool m_isQuickCombat;	//是否开启快速战斗
	uint64	m_FubenPlayer;	//副本专属

	uint64 m_timeClockMS;	//计时器，微秒
	bool   m_fight_calc;	//回合战斗是否在计算中
	bool   m_isInnerCir;	//快速战斗中不再进入循环
	uint32 m_idxInLst;
	std::map<uint64, IntPairVec> m_extraAwards;

	bool m_isStartNextRound;	//是否开始下一个回合（计算完一回合数据后置false,收到动画结束消息或倒计时结束时置true）





private:
    typedef std::vector< boost::function<void(CombatGroup*, bool)> > CallBackVec;
    CallBackVec m_CallBack;

	//typedef std::vector< boost::function<void(CombatGroup*, uint32)>> CallBackCrusade;
	//CallBackCrusade m_AddHerosCallBack;

	typedef std::vector< boost::function<void(CombatGroup*, CombatObj*)>> CallBackDead;
	CallBackDead m_NpcDeadCallBack;

	typedef std::vector< boost::function<void (CombatGroup*, CombatObj*)> > CallBackCharacterDead;
	CallBackCharacterDead m_CharDeadCallBack;

	typedef  std::vector<boost::function<void(CombatGroup*)> >CallBackRelease;
	CallBackRelease m_ReleaseCallBack;

	typedef std::vector<boost::function<void()> >CallBackBegin;
	CallBackBegin m_CombatBegin;

    CombatGroup(CombatType typ);
	~CombatGroup();

public:
    // 属性计算由另外模块完成，Combat只管战斗过程
    static CombatGroup* FindGroup(uint32 uniqueId);
    static CombatGroup* CreateGroup(CombatObj& attack, CombatObj& defence, CombatType typ, bool isInAttract = false, uint32 terrainType = 0);
	static CombatGroup* CreateGroup(CombatObj& attack, std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType = 0);
	static CombatGroup* CreateGroup(std::vector<CombatObj*>& attack, std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType = 0);
	static CombatGroup*	CreateGroup(std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType = 0);
    void AddCallBack_OnEnd(const boost::function<void(CombatGroup*, bool)>& func);

	//void AddCallBack_OnAddSoldier(const boost::function<void(CombatGroup*, uint32)>& func);
	void AddCallBack_OnNpcDead(const boost::function<void(CombatGroup*, CombatObj*)>& func);
	void AddCallBack_OnGroupRelease(const boost::function<void(CombatGroup*)>& func);
	void AddCallBack_OnBegin(const boost::function<void()>& func);
	void AddCallBack_OnCharacterDead(const boost::function<void (CombatGroup*, CombatObj*)>& func);


    void SaveTo(pb::GS2C_Combat_Begin& msg);
    void SaveTo(pb::GS2C_City_Fight_Info& msg);
	void SaveTo(pb::GS2C_Combat_Next_Lst& msg, uint32 beginindex, bool isAttack);
    void SaveStatisticData(pb::GS2C_Combat_End& msg, uint64 playerId);
	void SaveAllStaticData();
    void Broadcast(int opCode, google::protobuf::Message& msg, uint64 except = 0);
    void BroadcastStatisticData(bool isAttackWin);
	void SaveTo(pb::GS2C_Combat_Pre_Begin& msg);
	bool canCallSeigeArmy(const uint32 type,const uint32 country_id,const uint32 city_s_country_id = 0);
	void StartCombatAndChange(pb::C2GS_Combat_Begin& msg);

	bool SaveTo(pb::GS2C_Combat_Select_Tactic_Single& attmsg, pb::GS2C_Combat_Select_Tactic_Single& defmsg, int& count); //count为0则都不发送，1为发送att，2为发送def，3为都发送

	void SaveTo(pb::Combat_All_Buff& buffMsg);

	template< class stFunc >
	void ForEachWatchPlayer(stFunc& func){
		for (std::vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end(); ++it){
			func(*it);
		}
	}

	template< class stFunt >
	void ForMainPlayer(stFunt& func){ func(m_FubenPlayer); }

//	Combat::CombatObj* Get
	void _UpdateOperateTime(bool innerCir = false);
	void _NewUpdateOperateTime(bool innerCir = false);

    bool AddObjToAttack(CombatObj& obj,bool isSkillIgnore = false);
    bool AddObjToDefencek(CombatObj& obj,bool isSkillIgnore = false);
    void DelObjToAttack(CombatObj& obj);
    void DelObjToDefencek(CombatObj& obj);
	bool AddReinforceToAttack(CombatObj& obj);
	bool AddReinforceToDefence(CombatObj& obj);
	void UpdateHerosInCombat();
	void ReleaseHeros();

    void Watch(uint64 playerId);
    void UnWatch(uint64 playerId);
    CombatType GetType(){ return m_eCombat; }
    CombatObj* FindObj(uint64 playerId, uint32 heroId, bool isAttacker);

	//客户端战斗动画结束消息
	void AniFinished();
	void SendNewRoundMsg(uint64 oprEndTime, CombatObj* objAtt, CombatObj* objDef);

    //诱敌
	bool AttractEnemy(::Player& playe, bool isAttacker, pb::GS2C_Combat_Attract_Enemy& msg);
    //突进
	bool Rush(::Character& hero, uint16 destCityId, bool isAttacker, pb::GS2C_Rush_Result& msg);
    //增兵
    bool AddSoldier(::Player& player, bool isAttacker, uint32 hero_id, uint32& addExp, uint32 desCity);
    //撤退
	bool RunAway(::Player& player, ::Character& hero, uint16 destCityId, bool isAttacker, pb::GS2C_Combat_Runaway& msg);
    //征召
	static void CallUp(::Player& player, uint16 destCityId, pb::GS2C_Combat_Call_Up& msg);
	static void CallRequest(::Player& player, pb::C2GS_Combat_Call_Request& callmsg, uint16 destCityId,pb::GS2C_Combat_Call_Request& msg);
    //帅旗，加速战斗
    bool SpeedUp(::Player& player, uint16 destCityId);
	
	bool AutoAttractEnemy(Character* charone, bool isAttacker, pb::GS2C_Combat_Attract_Enemy& msg);

	//远征填充武将
	void AddHeros(::Player& player, uint32 heroID);

	void ReleaseGroup();
	static void _OtherKillStatisticData(::Character& hero, CombatType typ);
	bool IsIdInAttractCombatList(uint64 playerid);
	bool IsPlayerIdInSet(std::set<uint64>& Set,uint64 id);

	int GetMonsterInAttackLst();

	void ShutDown();

	void RecordCombatGainsInfo(uint64 playerid, pb::Msg_Records_Combat_Gains type, uint32 num, IntPairVec gainsPair);
	void BroadcastCombatGainsInfo(uint32 cityid,uint32 wincountry);

	void SaveCombatInfosOfGains(pb::GS2C_Combat_End& msg, uint64 playerId);		//获取在这场战斗中玩家获得的奖励及其他数据
	static void _SaveStaticKillOnce(::Character& hero, CombatType typ, uint32 killonce);		//记录一次伤害统计

	uint32 GetShadowNum(bool isAtt, uint64 playerid = 0);
private:
    void _CheckDead();
    void OnGroupCombatEnd(bool isAttackWin);
	bool DoFight1(CombatObj* attack, CombatObj* defence);
	void DoFight2(CombatObj* attack, CombatObj* defence, pb::GS2C_Combat_Select_Tactic& msg);

	bool CalcNewFight(CombatObj* attack, CombatObj* defence);	//新战斗计算
	void FightInOrder(pb::GS2C_Combat_Select_Tactic& msg, CombatObj* attack, CombatObj* defence);	//序列化战斗
	void CleanFightList();
	void MakeObjDead(CombatObj* comobj);											//obj后事
	void AfterFightAutoCountryWar();							//战斗结束后自动国战

    bool _AddObj(CombatObj& obj, GroupLst& lst, bool isSkillIgnore = false);
    void _DelObj(CombatObj& obj, GroupLst& lst);

    void _AttractEnemyEnd(bool isAttackWin);
    void _RefreshFightingObj(GroupLst& lst);
    int  _GetFirstNotFightingIdx(GroupLst& lst);
    CombatObj* RandObj(GroupLst& lst, CombatObj* except);
	CombatObj* GetFirstNotFightingObj(GroupLst& lst);
    static void CalcRewardOneLoop(CombatObj* obj, CombatType typ);

	static void CalcRewardEnd(CombatObj* obj, CombatType typ);

    void _CreateUniqueId();

    void _NotifyClientHeroStatus(CombatObj* obj);

	void _CheckBuffDead();

	uint32 GetInitOperateSeconds();
};

}