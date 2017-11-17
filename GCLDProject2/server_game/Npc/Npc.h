#pragma once

#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "memory_buffer/MemoryPool.h"
#include "../Combat/CombatObj.h"
#include "../Combat/combat_define.h"
class City;
struct NpcProto;

//using namespace Combat;

namespace Combat
{
	enum TacticEnum;
	enum CombatType;
}

class Npc :public Combat::CombatObj {
    DECLARE_INDEX_POOL(Npc, 5000);
private:
    const NpcProto& m_npcProto;
    uint32  m_countryId;
    uint32  m_curCityId;
    uint16  m_level;
    uint32  m_unique_id;
	bool    m_canMove;

	uint32	m_tmpAiLevel;		//临时AI属性，0为普通，1为高级

    std::queue<uint32> m_path; //路线城市
    static std::set<Npc*>  m_deleteNpc;
	IntPairVec					m_enableTacs;

	Combat::TacticEnum tectpy;
	Combat::CombatType typcom;


    ~Npc(); //dtor私有，禁止外界delete pNpc;
public:
    Npc(const NpcProto& proto);

    int GetUniqueId(){ return m_unique_id; }
    static Npc* FindNpc(uint32 uniqueId);

    static void DeleteNpc(Npc* p);
    static void _DeleteNpc(); //服务器本帧结束时调用

    uint32 GetCurCity(){ return m_curCityId; }
    void SetCurCity(uint32 id){ m_curCityId = id; }
    uint32 GetCountryId(){ return m_countryId; }
    void SetCountryId(uint32 id){ m_countryId = id; }
    void SetMap(CMap* pMap){ m_pMap = pMap; }

    int32 GetLevel() const;
    void SetLevel(uint16 lv){ m_level = lv; }

	void RecoverFullSoldier();

    void _Move();
	virtual uint32 GetType() const;

    //CombatObj里的virtual func
    void    OnDead(CombatObj* killer);
	void	MoveTo(uint32 cityId);
	const string& GetPlayerName();
    uint64  GetPlayerId(){ return m_unique_id; }
    uint32  GetTableId();
	uint32	GetMaxRow();
    float   SkillRatio();
    uint8   SkillRange();
    void    InitCombatData();
	void	InitCombatDataSecond(uint32 geography = 0);
	uint32	SkillID();
	uint32	GetEnableTactics(uint32 geography = 0);
	bool NewAutoTactic(Combat::CombatType cTyp = Combat::CT_Country, Combat::TacticEnum tTyp = Combat::Attack);
	//bool IsShadow() { return false; }
	void reCalcPower();
	virtual int GetTotalFightPercent();
	virtual int GetTotalSkillPercent(const Combat::CombatType type,const uint32 obj_type = 0) ;
	virtual int GetTotalExpPercent(Combat::CombatType type );
	virtual int GetTotalReduceCostHP();
	virtual int GetTotalDoubleKill(Combat::CombatType type) ;
	virtual int GetTotalDeSkillPercent();
	virtual void AddSkillPercentBySeigeArmySpecificId(int add_percent, int enemyType = 0);
	virtual uint32 GetTerrDebuffAddtion(uint32 terrType);
	virtual uint32 GetPlayerLevel();
	virtual double GetTerrTalentAddtion(uint32 terrType);
	virtual int GetMaxHp();

	void SetMovePath(std::vector<int>& path);
	uint32 GetExtraAwardID();
	uint32 GetExtraAwardNum();
private:
    bool _ExitCurCity(City& city);
    bool _EnterCity(City& city);
    void _CreateUniqueId();
	void SetEnableTactic(std::string& tacs);
};