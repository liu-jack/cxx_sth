#pragma once

#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "memory_buffer/MemoryPool.h"
#include "Combat/CombatObj.h"
#include "Combat/combat_define.h"
class City;
struct LogOfSeigeArmy;
class CharSiegeArmy;
class Player;

class Reinforce :public Combat::CombatObj {
	DECLARE_OBJ_POOL(Reinforce)
private:
	const CharSiegeArmy& m_reinforceProto;
	Player* m_player;		//所属玩家
	uint32  m_countryId;
	uint32  m_curCityId;
	
	uint32 m_damage;	//固定伤害
	const LogOfSeigeArmy* m_LogOfArmy;

	CurrType2Count m_DamageOfType;	//不同类型对应的伤害加成

	uint32 m_maxHP;		//最大血量

private:
	static std::vector<Reinforce*>  m_deleteReinforce;
	DISALLOW_COPY_AND_ASSIGN(Reinforce);

public:
	explicit Reinforce(const CharSiegeArmy& charProto, Player* player,LogOfSeigeArmy* LogOfArmyStruct);		//创建增援部队

public:
	static void DeleteReinforce(Reinforce* p);					//删除增援部队
	static void _DeleteReinforce();								//服务器本帧结束时调用

	uint32 GetDamage() { return m_damage; }	

	uint32 GetCurCity(){ return m_curCityId; }
	void SetCurCity(uint32 id){ m_curCityId = id; }
	uint32 GetCountryId(){ return m_countryId; }
	void SetCountryId(uint32 id){ m_countryId = id; }
	void SetMap(CMap* pMap){ m_pMap = pMap; }

	uint32 GetLevel();
	//void SetLevel(uint16 lv){ m_level = lv; }
	void RecoverFullSoldier();
	void    OnDead(CombatObj* killer);
	//void	MoveTo(uint32 cityId);
	const string& GetPlayerName();
	uint64  GetPlayerId();
	uint32  GetTableId();
	uint32	GetMaxRow();
	float   SkillRatio();
	uint8   SkillRange();
	uint32	SkillID();
	void    InitCombatData();
	void	calcateAttr();
	void	InitCombatDataSecond(uint32 geography = 0);
	uint32	GetEnableTactics(uint32 geography = 0);
	bool NewAutoTactic(Combat::CombatType cTyp = Combat::CT_Country, Combat::TacticEnum tTyp = Combat::Attack);
	bool IsShadow() { return false; }
	virtual void AddSkillPercentBySeigeArmySpecificId(int add_percent, int enemyType = 0);
	virtual int GetTotalFightPercent();
	virtual int GetTotalSkillPercent(const Combat::CombatType type,const uint32 obj_type = 0) ;
	virtual int GetTotalExpPercent(Combat::CombatType type );
	virtual int GetTotalReduceCostHP();
	virtual int GetTotalDoubleKill(Combat::CombatType type) ;
	virtual int GetTotalDeSkillPercent();
	virtual uint32 GetTerrDebuffAddtion(uint32 terrType);
	virtual uint32 GetType() { return 0; } 
	virtual uint32 GetPlayerLevel();
	virtual double GetTerrTalentAddtion(uint32 terrType);
	virtual int GetMaxHp();
};