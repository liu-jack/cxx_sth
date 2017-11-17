
/*
*Add BY:zhoulunhao 
*CREATE TIME:2017.10.9
*MAIL:zhoulunhao@hotmail.com
*/


#ifndef INCLUDE_ACTIVITY_RANK_H
#define INCLUDE_ACTIVITY_RANK_H


#include "game_data//datastruct/struct_act_country.h"
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "Exploit/TrickReward.h"
//#include "lock_free/Mutex.h"

class CountryMatch
{
private:
	const DB_CountryMatch& db_;
	IntPairVec reward_;
public:
	CountryMatch(const DB_CountryMatch& db);
	const IntPairVec& getReward() const;
};

class OccupationMatch
{
public:
	OccupationMatch(const DB_OccupationMatch& db);
	const IntPairVec& getReward() const;
	const IntPair& getRank() const;
private:
	const DB_OccupationMatch& db_;
	IntPairVec reward_;
	IntPair rank_;
};


class AddSoliderMatch
{
	const DB_AddSoliderMatch& db_;
	IntPairVec reward_;
	IntPair rank_;
public:
	AddSoliderMatch(const DB_AddSoliderMatch& db);
	const IntPairVec& getReward() const;
	const IntPair& getRank() const;
};

#define TYPE_OF_ADD_SOLIDER 1
#define TYPE_OF_OCCUPATION 2

typedef PtrMap<uint32,CountryMatch> ActivityRankCountryMatchMap;
typedef PtrMap<uint32,OccupationMatch> ActivityRankOccupationMatchMap;
typedef PtrMap<uint32,AddSoliderMatch> ActivityRankAddSoliderMatchMap;
class ActivityRankTableMgr:public SimpleSingleton<ActivityRankTableMgr>
{
	ActivityRankCountryMatchMap activity_rank_country_match_map_;
	ActivityRankOccupationMatchMap activity_rank_occupation_match_map_;
	ActivityRankAddSoliderMatchMap activity_rank_add_solider_match_map_;
	//mutable Mutex mutex_;
	
public:
	void Init();
	const IntPairVec* getRewardByCountryRank(const uint32 countryRank) const;
	const IntPairVec* getRewardByRankId(const uint32 type,const int rankId) const;
	void InitializationRankMap();
	void GiveOccupationRewardWhenActivityEnd();
	void GiveAddSoldierRewardWhenActivityEnd();
	ranksortMap occupation_rank_map_;
	ranksortMap add_solider_rank_map_;
};

#define sActivityRankTableMgr (ActivityRankTableMgr::Instance())

#endif ///INCLUDE_ACTIVITY_RANK_H