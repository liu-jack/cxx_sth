#pragma once

#include <vector>
#include "boost/throw_exception.hpp"
#include "boost/dynamic_bitset.hpp"
#include "def/TypeDef.h"
#include "LootFeatureLogic.h"

struct Loot_Filter;

class LootFilter
{
    friend class LootManager;
	typedef std::vector<LootFeatureLogic*> FeatureArray;
	static const int RAND_FACTOR = 100;
public:
	LootFilter(const Loot_Filter* filter);
	~LootFilter(void);
    
    int RoundCorrectMod() const;
    int RoundCorrectCount(int outerCorrectCount = 0) const;

    bool NeedRollRecord() const;

    uint32 LootBoxType() const;
    uint32 FilterId() const;
    uint32 Priority() const;
	void AddFeature(LootFeatureLogic* feature);
	void Fill(LootList& lootList, int lv, int vipLv,bool isCountLoot = false,bool is_exploit = false) const;

	bool IsMatchRate( int rollCount = 0, int rollSeccessCount = 0,  int outerCorrectCount = 0) const;
    bool IsMatchTime() const;
private:
	LootFeatureLogic* FindFeature(uint32 featureId);

private:
	const Loot_Filter*            m_db;
	FeatureArray                  m_features;
};
