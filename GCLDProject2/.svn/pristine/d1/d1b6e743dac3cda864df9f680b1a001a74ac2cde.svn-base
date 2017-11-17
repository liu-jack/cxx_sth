#pragma once
#include "def/TypeDef.h"
#include "datastruct/struct_seige_force.h"
#include "ptr_container/PtrMap.h"
#include "boost/shared_ptr.hpp"
#include "SimpleSingleton.h"
#define ATTR_SIZE 12

#define UNLOCK_USE_DIAMOND		1
#define UNLOCK_USE_ITEM			2
#define UNLOCK_USE_LOGIN_DAY	3

class SiegeArmyUp
{
private:
	const DB_SiegeArmyUp& db_;
	IntPairVec critical_;
	IntPairVec cost_material_;
public:
	SiegeArmyUp(const DB_SiegeArmyUp& db);
	const IntPairVec& getCritical() const;
	const IntPairVec& getCostMaterial() const;
	uint32 Index() const;
	uint32 armyId() const;
	uint32 level() const;
	uint32 levelExp() const;
	uint32 delvlopExp() const;//once level up operate
	uint32 damage() const;
	uint32 FightValue() const;
};

class SiegeArmySpecific
{
private:
	const DB_SiegeArmySpecific& db_;
	VecInt specific_parameter_;
public:
	SiegeArmySpecific(const DB_SiegeArmySpecific&db);
	const VecInt& getSpecificParameter() const;
	uint32 specificId() const;
	uint32 specificType() const;
};


//ÔöÔ®²¿¶Ó
class CharSiegeArmy
{
private:
	const DB_CharSiegeArmy& db_;
	IntPairVec specific_ids_;
	VecInt fight_terrain_;
	float attr_[ATTR_SIZE];
public:
	CharSiegeArmy(const DB_CharSiegeArmy& db);
	const IntPairVec& specificIds() const;
	const VecInt& fightTerrain() const;
	uint32 armyId() const;
	uint32 skillId() const;
	uint32 fightTimes() const;
	uint32 unlockType() const;
	const string& unlockCost() const;
	uint32 discountLastTime() const;
	uint32 linecount() const;
	uint32 GetAttr(uint32 index) const;
};

typedef PtrMap<uint32,SiegeArmyUp> SiegeArmyUpMap;
typedef PtrMap<uint32,SiegeArmySpecific> SiegeArmySpecificMap;
typedef PtrMap<uint32,CharSiegeArmy> CharSiegeArmyMap;

class SeigeArmyForceMgr:public SimpleSingleton<SeigeArmyForceMgr>
{
private:
	SiegeArmyUpMap siege_army_up_map_;
	SiegeArmySpecificMap siege_army_specific_map_;
	CharSiegeArmyMap char_siege_army_map_;
public:
	void Init();
	const SiegeArmyUp* getSiegeArmyUp(const uint32 army_id,const uint32 army_level) const;
	const uint32 GetFightValue(const uint32 army_id,const uint32 army_level) const;
	//const uint32 GetDamageHurt(const uint32 army_id,const uint32 army_level) const;
	const CharSiegeArmy* getCharSiegeArmy(const uint32 army_id) const;
	const SiegeArmySpecific* getSiegeArmySpecific(const uint32 specific_id) const;
	bool IsValidArmyId(const uint32 army_id) const;
	const CharSiegeArmyMap& getCharSiegeArmyMap() const;
};


#define sSeigeArmyForceMgr (SeigeArmyForceMgr::Instance())