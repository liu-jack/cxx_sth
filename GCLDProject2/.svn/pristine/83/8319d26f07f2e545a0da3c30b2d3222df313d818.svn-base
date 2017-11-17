
#include "ReinforceTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"

SiegeArmyUp::SiegeArmyUp(const DB_SiegeArmyUp& db)
:db_(db)
{
	Utility::SplitStr2(db_.critical,critical_);
	Utility::SplitStr2(db_.developMaterial,cost_material_);
	for(IntPairVec::iterator iter = cost_material_.begin();iter != cost_material_.end();++iter)
	{
		iter->second *= -1;
	}
}

const IntPairVec& SiegeArmyUp::getCritical() const
{
	return critical_;
}

const IntPairVec& SiegeArmyUp::getCostMaterial() const
{
	return cost_material_;
}

uint32 SiegeArmyUp::Index() const
{
	return db_.index;
}

uint32 SiegeArmyUp::armyId() const
{
	return db_.armyId;
}

uint32 SiegeArmyUp::level() const
{
	return db_.level;
}

uint32 SiegeArmyUp::levelExp() const
{
	return db_.levelExp;
}

uint32 SiegeArmyUp::delvlopExp() const
{
	return db_.developExp;
}

uint32 SiegeArmyUp::damage() const
{
	return db_.damage;
}

uint32 SiegeArmyUp::FightValue() const
{
	return db_.fight_value;
}

SiegeArmySpecific::SiegeArmySpecific(const DB_SiegeArmySpecific&db)
:db_(db)
{
	Utility::SplitStr(db_.specificParameter,specific_parameter_,',');
}

const VecInt& SiegeArmySpecific::getSpecificParameter() const
{
	return specific_parameter_;
}

uint32 SiegeArmySpecific::specificId() const
{
	return db_.specificId;
}

uint32 SiegeArmySpecific::specificType() const
{
	return db_.specificType;
}

CharSiegeArmy::CharSiegeArmy(const DB_CharSiegeArmy& db)
:db_(db)
{
	Utility::SplitStr2(db_.specificId,specific_ids_);
	Utility::SplitStr(db_.fightTerrain,fight_terrain_,',');
	for(int i = 0; i < ATTR_SIZE;++i)
	{
		attr_[i] = db_.baseValue[i];
	}
}

const IntPairVec& CharSiegeArmy::specificIds() const
{
	return specific_ids_;
}

const VecInt& CharSiegeArmy::fightTerrain() const
{
	return fight_terrain_;
}

uint32 CharSiegeArmy::armyId() const
{
	return db_.armyId;
}

uint32 CharSiegeArmy::skillId() const
{
	return db_.skillId;
}

uint32 CharSiegeArmy::fightTimes() const
{
	return db_.fightTimes;
}

uint32 CharSiegeArmy::unlockType() const
{
	return db_.unlock_type;
}

const string& CharSiegeArmy::unlockCost() const
{
	return db_.unlock_cost;
}

uint32 CharSiegeArmy::discountLastTime() const
{
	return db_.discount_last_time;
}

uint32 CharSiegeArmy::linecount() const
{
	return db_.line_count;
}

uint32 CharSiegeArmy::GetAttr(uint32 index) const
{
	if(index < ATTR_SIZE)
	{
		return static_cast<uint32>(attr_[index]);
	}
	return 0;
}

void SeigeArmyForceMgr::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_CharSiegeArmy)
	{
		CharSiegeArmy*a = new CharSiegeArmy(*ptr1);
		char_siege_army_map_[a->armyId()] = a;
	}
	FOREACH_DB_ITEM(ptr2, DB_SiegeArmySpecific)
	{
		SiegeArmySpecific* a = new SiegeArmySpecific(*ptr2);
		siege_army_specific_map_[a->specificId()] = a;
	}
	FOREACH_DB_ITEM(ptr3, DB_SiegeArmyUp)
	{
		SiegeArmyUp *a = new SiegeArmyUp(*ptr3);
		siege_army_up_map_[a->Index()] = a;
	}
}

const SiegeArmyUp* SeigeArmyForceMgr::getSiegeArmyUp(const uint32 army_id,const uint32 army_level) const
{
	for(SiegeArmyUpMap::const_iterator iter = siege_army_up_map_.begin();iter != siege_army_up_map_.end();++iter)
	{
		if(iter->second->armyId() == army_id && iter->second->level() == army_level)
			return iter->second;
	}
	return NULL;
}

const uint32 SeigeArmyForceMgr::GetFightValue(const uint32 army_id,const uint32 army_level) const
{
	for(SiegeArmyUpMap::const_iterator iter = siege_army_up_map_.begin();iter != siege_army_up_map_.end();++iter)
	{
		if(iter->second->armyId() == army_id && iter->second->level() == army_level)
			return iter->second->FightValue();
	}
	return 0;
}

// const uint32 SeigeArmyForceMgr::GetDamageHurt(const uint32 army_id,const uint32 army_level) const
// {
// 	const SiegeArmyUp* army_up = getSiegeArmyUp(army_id,army_level);
// 	if(army_up)
// 	{
// 		return army_up->damage();
// 	}
// 	return 0;
// }

const CharSiegeArmy* SeigeArmyForceMgr::getCharSiegeArmy(const uint32 army_id) const
{
	return char_siege_army_map_.pfind(army_id);
}

const SiegeArmySpecific* SeigeArmyForceMgr::getSiegeArmySpecific(const uint32 specific_id) const
{
	return siege_army_specific_map_.pfind(specific_id);
}

bool SeigeArmyForceMgr::IsValidArmyId(const uint32 army_id) const
{
	return getCharSiegeArmy(army_id) != NULL;
}

const CharSiegeArmyMap& SeigeArmyForceMgr::getCharSiegeArmyMap() const
{
	return char_siege_army_map_;
}
