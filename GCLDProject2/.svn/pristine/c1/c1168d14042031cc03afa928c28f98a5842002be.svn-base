#pragma once
#include "def/TypeDef.h"
#include "Base.h"
#ifdef USE_PTR_HASH_MAP
#include <boost/unordered_map.hpp>
#endif // USE_PTR_HASH_MAP

class Player;
class Character;
class MapEvent;
class Country;

namespace Combat
{
	class CombatGroup;
	class CombatObj;
}

class Npc;

namespace pb
{
    class GS2C_MapLogic_Init;
    class GS2C_MapLogic_ArmyResource;
    class GS2C_All_CITY_EVENT;
    class CityEvent;
	class C2GS_REQUEST_FOG_INFO;
	class GS2C_FOG_INFO_Lst;
	class Struct_FOG_INFO;
	class GS2C_Take_Army_Resource_Result;
	class GS2C_MapLogic_ArmyResource;
	class GS2C_TunTian_Donate_Ret;
	class GS2C_Clear_Tuntian_CD_Ret;
	class GS2C_Train_Info;
	class GS2C_Take_Award_Ret;
	class GS2C_UseCurrencyToTakeRewardRsp;
	class StReward;
	class CelebrityEvent;
	class GS2C_Monster_City_Donate;
	class MonsterCityLst;
	class SG2D_PersonalEC;
}

struct CityEventInfo
{
    uint64 happenTime;		//活动开始时间
    uint16 dailyTimes;		//每日领取的奖励次数
    uint32 cityId;			//奖励发生城市
    uint8  leftCnt;
    bool   isBeginning;
	bool   isCelebrity;
	uint64 endTime;			//活动结束时间
	std::set<uint64>	takePlayers;

    CityEventInfo()
		: happenTime(0)
		, dailyTimes(0)
		, cityId(0)
		, leftCnt(0)
		, isBeginning(false)
		, isCelebrity(false)
		, endTime(0)
	{
		takePlayers.clear();
	}
    CityEventInfo(uint64 time)
        : happenTime(time)
        , dailyTimes(0)
        , cityId(0)
        , leftCnt(0)
        , isBeginning(false)
		, isCelebrity(false)
		, endTime(0)
    {
		takePlayers.clear();
	}
	void ClearInfo(bool is_celebrity,uint64 next_fresh_time)
	{
		happenTime = 0;
	    dailyTimes = 0;
		cityId = 0;
		leftCnt = 0;
		isBeginning = false;
		isCelebrity = is_celebrity;
		endTime = next_fresh_time;
		takePlayers.clear();
	}
};

enum EventType
{
	Event_Null,
	Event_Dialog,
};

struct EventCeil
{
	uint32		eventid;		//当前任务的id
	EventType	etype;			//根据id读表得到type
	uint32		cityid;
	uint32		target;			//任务目标
	uint32		curCompletion;	//任务完成度
	IntPairVec	eawards;
	bool		isTakeAward;	//奖励是否已领取
	void Reset();
};

struct MysteryItem
{
	uint32 itemid;		//道具id
	uint32 itemnum;		//道具个数
	bool   isSold;		//是否已经售卖掉
};

struct MysteryStore
{
	int leftFreshTimes;		//剩余刷新次数
	std::vector<MysteryItem> myItems;	//商店道具列表
	void Reset();
};

class PersonalEventChain	//个人事件链
{
public:
	PersonalEventChain();

public:
	uint64	lastFreshTime;	//上次点击刷新时间		要求一日内不允许重复点
	uint32	currentStep;	//当前环数(已完成个数)
	IntPairVec awardsEx;	//全部完成后的奖励
	bool	isTakeExAward;

	EventCeil currentEvent;

	MysteryStore mysteryStore;

public:
	void SaveTo(pb::SG2D_PersonalEC& msg, uint64 playerid);
	void LoadFrom(pb::SG2D_PersonalEC& msg);
};

class CityEventChain	//攻城事件链
{
public:
	CityEventChain();

public:
	uint64	lastFreshTime;	//上次点击刷新时间		要求一日内不允许重复点
	uint32	currentStep;	//当前环数(已完成个数)
	IntPairVec awardsEx;	//全部完成后的奖励
	bool	isTakeExAward;

	EventCeil currentEvent;

};

class PersonalTriggerEvent	//个人攻城触发时间
{
public:
	PersonalTriggerEvent();

public:
	uint32	currentStep;	//当前环数(触发个数，不同于上面两个，可以未完成)
	
	EventCeil currentEvent;

private:
	std::vector<uint32>	timeOfTriggerWork;
};


#ifndef USE_PTR_HASH_MAP
typedef std::map<int32,CityEventInfo> MapCityEvent;
#else
typedef typename boost::unordered_map<int32,CityEventInfo> MapCityEvent;
#endif // USE_PTR_HASH_MAP

class FogInfo
{
public:
	FogInfo(uint32 fogid, uint64 uniqueid);

	uint32 m_fogID;
	uint64 m_uniqueID;
	IntPairVec m_fogGuardsInt;
	std::set<Combat::CombatObj*> m_fogGuardsObj;
	//std::set<Combat::CombatObj*> m_fogObj;
	IntPairVec m_fogGuardsMaxhp;

	uint32 guard_all_hp;
public:
	void refreshInt();
};

class PlayerMapLogic { //TODO:zhoumf:存数据库
    enum {
        ArmyResource_Open_Level = 31,
        Tonden_CD               = 10 * 60,
        Tonden_Last_Time_Max    = 12 * 3600,
        Train_Last_Time         = 3 * 60,
        ArmyResource_Max_Size   = 24,
        ArmyResource_Silver_Rate = 45,
        ArmyResource_Iron_Rate  = 2,
    };
    uint32 m_countryId;
    uint32 m_StoryCountryId; //剧本中的国家id，无需入库

	uint32 m_CrossCountryID;	//跨服战中被分配的国家id

    //军资相关
    uint32  m_ArmyResourceUpdateDay;
    uint8  m_ArmyResourceUpdateHour;
    static uint32 m_CountryHaveCitys[24][3];
    IntPairVec m_ArmyResource;

    //已拿到的路途宝箱
    uint64 m_takeBoxBit;

    //个人城池事件
    MapCityEvent m_privateCityEventLst;
    //全服共享城池事件
    static MapCityEvent m_publicCityEventLst;
    //已经解锁的迷雾
    uint64 m_ulockFogCityBit[3];

    //屯粮
    std::map<uint8, uint64> m_tondenHeroBeginTime; // <heroIdx,beginTime>
	uint64 m_tondenCDEndTime;		//屯田捐赠cd结束时间

	//迷雾信息
	std::vector<FogInfo*> m_fogGuardians;		//所有的迷雾守卫
	
public:
	std::pair<uint32,CityEventInfo> m_CelebrityCityEventLst;
    uint64 m_countryDonateTime; //国家升级捐赠
    uint64 m_monsterDonateTime; //蛮族捐赠

	uint64 m_trainingStartTime;
	uint32 m_trainType;
	std::vector<uint32> m_trainHeros;
	uint64 m_trainingEndTime;	//训练结束时间
	uint64 m_lastUpdateMapTime;
private:
	//Player&   m_player;

public:
    PlayerMapLogic();
    void SaveTo(pb::GS2C_MapLogic_Init& msg);
    void LoadFrom(Player* player, const pb::GS2C_MapLogic_Init& msg);
    void SaveTo(Player& player,pb::GS2C_MapLogic_ArmyResource& msg);
    void LoadFrom(const pb::GS2C_MapLogic_ArmyResource& msg);
    void SaveTo(pb::GS2C_All_CITY_EVENT& msg);
    static void SaveTo(pb::CityEvent& msg, uint32 eventId, const CityEventInfo& info);
	void SaveTo(pb::GS2C_FOG_INFO_Lst& msg);
	void SaveTo(pb::CelebrityEvent& msg,const MapEvent& eventTable);
		

	bool Init(Player& player);

    uint32 GetCountryId();
    Country* GetWorldMapCountry();
    bool _SetCountryId(Player& player, uint32 id);
    void SetStoryCountryId(uint32 id){ m_StoryCountryId = id; }

	//军资
    static void _UpdateHaveCityCntPerHour(uint32 hourTime);

	void updateArmyResourcePerHour(Player& player);
private:
    void _UpdateArmyResource(Player& player);
    void _AddArmyResource(uint32 hourTime, uint32 cityCnt);
public:
	bool TakeArmyResource(Player& player, pb::GS2C_Take_Army_Resource_Result& msg);

    void TakeBox(Player& player, uint8 boxId); //已拿到的路途宝箱
    uint32 GetBoxCnt() const;

    //个人城池事件
    void _UpdateCityEvent(Player& player, uint64 timeNowMS);
    void _CheckCityEvent(Player* player, const MapEvent& eventTable, uint64 timeNow, bool freshTimes);
    void _OnCityEventBegin(Player& player, const CityEventInfo& info, const MapEvent& eventTable);
    void _OnCityEventEnd(Player& player, const CityEventInfo& info, const MapEvent& eventTable);
    uint16 _RandPrivateEventCity();
	void ClientHandleCityEvent(Player& player, uint32 cityId, uint32 eventId, uint8 selectIdx, pb::GS2C_Take_Award_Ret& msg,IntPairVec& rewrad);
    uint64 GetCityEventHappenTime(uint32 eventId) const;

	void _InitMapEvents(Player* player, const MapEvent& eventTable, uint64 timeNow);

    //全服共享城池事件
    static void UpdateCityEvent();
    static void _CheckCityEvent(const MapEvent& eventTable, uint64 timeNow);
    static void _OnCityEventBegin(const CityEventInfo& info, const MapEvent& eventTable);
    static void _OnCityEventEnd(const CityEventInfo& info, const MapEvent& eventTable);
    static uint16 _RandPublicEventCity(const MapEvent& eventTable);
/////Celebrity event
	void UpdateCelebrityCityEvent(Player* player);
	void CheckCelebrityCityEvent(Player* player,const uint32 event_id, uint64 activity_start);
	void OnCelebrityCityEventChange(Player* player,const CityEventInfo& info,const uint32 event_id);
	void PackEventInfo(const MapEvent& eventTable,pb::CelebrityEvent& msg);
	uint16 RandCelebrityPrivateEventCity();
	void ClientHandleCelebrityCityEvent(Player* player, uint32 cityId, uint32 eventId, uint8 selectIdx,pb::GS2C_UseCurrencyToTakeRewardRsp& msg,IntPairVec& reward);

	
    //迷雾
    void UnlockFog(Player& player, uint32 fogId);
	void dealRequireFogInfo(Player& player, pb::C2GS_REQUEST_FOG_INFO& requires);
	void EnterFog(Player& player, uint32 fog_id, std::vector<uint32>& heroIds);
	void OnFogEnd(Player* player, uint32 fogId, Combat::CombatGroup* pGroup, bool isAttackWin);
	void OnNpcDead(Player* player, uint32 fogId, Combat::CombatGroup* pGroup, Combat::CombatObj* obj);
	bool TakeFogReward(const uint32 id,IntPair* pair_reward);

    //屯粮
    bool TondenBegin(Player& player, Character& hero);
    void TondenEnd(Player& player, Character& hero);

    //练兵
    void TondenTrainBegin(Player& player, Character& hero, uint32 tokenItemId);
	void FillTrainInfo(Player& player, pb::GS2C_Train_Info& msg, uint32 trainIdx = 0);
	void TrainFinish(Player* player, pb::GS2C_Train_Info& msg);
	void AdditionUpEnd(Player* player, uint64 time);	//重置玩家武将的加成
	void TrainInstant(Player* player, pb::GS2C_Train_Info& msg);

    //屯田区的捐款
	void TondenDonateMoney(Player& player, pb::GS2C_TunTian_Donate_Ret& msg);
	void ClearCDorFresh(Player& player, bool isClear, pb::GS2C_Clear_Tuntian_CD_Ret& msg);

    //蛮族的捐赠
	void SendMonsterAllInfo(Player& player,pb::MonsterCityLst& msg);
	bool MonsterDonate(Player& player, uint32 countryId,pb::GS2C_Monster_City_Donate& msg);
    bool ClearMonsterDonateCD(Player& player);
	void ClearMonsterCityInvadeCount(const uint32 country_id);

//////////// 新世界事件 //////////////
public:
	PersonalEventChain		m_personalEC;	//个人世界事件链
	CityEventChain			m_cityEC;		//个人攻城事件链
	PersonalTriggerEvent	m_personalTE;	//个人攻城触发事件

//////////跨服专用///////////
public:
	void SetCrossUniqueID(uint32 countryid) { m_CrossCountryID = countryid; }
	uint32 GetCrossUniqueID() { return m_CrossCountryID; }

};
