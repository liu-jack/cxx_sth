#pragma once
#include "def/TypeDef.h"
#include "boost/function.hpp"
#include "../../Combat/combat_define.h"
#include "memory_buffer/MemoryPool.h"
namespace Combat
{
	//enum CombatType;
	class CombatObj;
	class CombatGroup;
}
namespace pb
{
    class Struct_City_Info;
}
class Npc;
class Player;
class CMap;
class Character;
class Reinforce;

class City
{
    DECLARE_OBJ_POOL(City)

    typedef std::vector< boost::function<void(City*)> > CallBackVec;
    struct CityPlayer {
        uint64 playerId;
        uint32 heroIdx;
    };

	struct CallUpRec
	{
		uint64 call_unique_id;	//征召唯一id
		uint64 call_time;		//征召结束
		uint32 countryid;		//可使用征召的国家
	};
public:
    enum EStatus {
        Normal,
        Fighting,   //战争
        Encircled,  //包围
        CallUp,     //可征召，允许本国玩家传送武将
        ForbidEnemy,//禁止敌方进入
        Confuse,    //敌方混乱，有几率单挑自己人
        Maze_Country_1, //迷宫，敌方经过时有几率偏离原路线
        Maze_Country_2,
        Maze_Country_3,
        Trap_Country_1, //陷阱，敌方经过时扣血
        Trap_Country_2,
        Trap_Country_3,
        Open_Monster_City, //蛮族城池可进入
		MonsterToken,	//蛮王令，可允许传送武将
        _Status_Cnt
    };
    const uint32 id;
    uint32 m_countryId;
    uint32 m_status;
	CMap& m_map;
    std::set<Combat::CombatObj*> m_defender;

    Combat::CombatGroup* m_combatGroup;
    CallBackVec m_CallBack;

	CallBackVec m_DefenceFailCallBack;

	//uint64 m_lastCallUpTime;
	//uint64 m_CallUpEndTime;

	std::vector<CallUpRec> m_country_monster_call;	//哪些国家可以通过蛮王令传到本城池
	std::vector<CallUpRec> m_country_call_up;		//哪些国家可以通过征召令传到本城池


	int m_durability;	//城市耐久度 跨服战

public:
    City(uint32 id_, uint32 countryId, CMap& map);
    void SaveTo(pb::Struct_City_Info& msg);
    void LoadFrom(const pb::Struct_City_Info& msg);
	void Init();

    uint32 GetCountryId(){ return m_countryId; }
    bool IsFighting();
    bool IsEncircled();
	bool IsMonsterCity();
    bool IsCountryEdge(uint32 checkCID = 9);
	bool IsCapitalCity();
	bool IsTonden();
	bool IsFortressCity();
    bool CanPass();
    void SetStatus(EStatus e, bool b, uint64 timeCallUp = 0, uint32 countryid = 0);
    bool HaveStatus(EStatus e);
	void ClearCallUp();
	bool HaveCountryEdge(uint32 countryid);	//除了countryid外还有其他敌对国家
	
	void AddCallUpCountry(uint32 countryid, uint64 timeCallUp, uint64 unique_call_id);
	void CallUpEndCallBack(uint64 uniqueid);
	bool IsCallUpTokenInLst(uint64 unique_call_id);
	uint64 GetCallUpUniqueid(uint32 countryid);

	void AddMonsterCallCountry(uint32 countryid, uint64 timeMonsterCall, uint64 unique_call_id);
	void MonsterCallEndCallBack(uint64 uniqueid);
	bool IsMonsterTokenInLst(uint64 unique_call_id);
	uint64 GetMonsterCallUniqueid(uint32 countryid);

    void SetMazeStatus(int countryId, bool b);
    void SetTrapStatus(int countryId, bool b);

    bool CanEnter(Combat::CombatObj& obj);
    bool CanExit(Combat::CombatObj& obj);
	void NpcEnter(Npc& npc);
    void NpcExit(Npc& npc);
    void HeroEnter(Player& player, Character& hero); //服务器重启，武将调进入城池，重建战斗内存数据
    void HeroExit(Player& player, Character& hero);

	bool ReinforceEnter(Reinforce& reinforce);
	void ReinforceExit(Reinforce& reinforce);
    void AddCallBack_BeOccupied(const boost::function<void(City*)>& func);
	void AddCallBacK_DefenceFail(const boost::function<void(City*)>& func);	//攻守城池用
	void ClearCallBack_DefenceFail();
    void OnBeOccupied(uint32 countryId);
    bool TryEncircled(); //包围

	void AddMonsterNpcsToDefence(uint32 num);
	void AddCountryTaskNpcToDefence();
    bool CanBeAttack();
    Combat::CombatGroup* GetCombatGroup(){ return m_combatGroup; }

	void AddDefenderNpc(int npcId, int npcLv, int npcCnt);
	void AddAttackNpc(int npcId, int npcLv, int npcCnt, int countryid);

	bool isRushLegal(bool isAttacker,const int soldier_times );

	void ReduceDurability(int num, uint32 attcountryid);

	void RefreshNPCNearBy(int stage);

public:
    void _AddCombatObj(Combat::CombatObj& obj, const uint32 countryId);
    void _DelCombatObj(Combat::CombatObj& obj, const uint32 countryId);
    void _AddDefenderNpc();
	void _DelDefenderNpc();
    Combat::CombatType GetCombatTyp();
	const uint32 GetCityId() const;
	
	bool _AddReinforce(Reinforce& reinforce);
	void _DelReinforce(Reinforce& reinforce);
public: //城池计策
    bool _Maze(Combat::CombatObj& obj);
    void _Trap(Combat::CombatObj& obj);
};