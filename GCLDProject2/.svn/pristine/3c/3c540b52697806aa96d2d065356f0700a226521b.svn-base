#pragma once

#include <vector>
#include "def/TypeDef.h"
#include "LootFeatureItemLogic.h"

#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP

struct LootFeature;
struct LootFeatureItems;
class LootList;

class LootFeatureLogic
{
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, LootFeatureItemLogic> ItemMap;
#else
	typedef PtrHashMap<uint32, LootFeatureItemLogic> ItemMap;
#endif

public:
	static bool IsValid(const LootFeature* pFeature);

	LootFeatureLogic(const LootFeature* pFeature);
	~LootFeatureLogic(void);

	uint32 Id() const;
	uint32 FilterId() const;
	void AddItem(const LootFeatureItems* pFeatureItem);


	bool IsMatchLevel(int giverLv, int vipLv) const;
	void Fill(LootList& lootList,bool isCountLoot,bool is_exploit = false) const;	

    void CalculateItemRate();
private:
	bool HasItem(uint32 itemId) const;
    bool IsMatchRequireMinLv( int level) const;
    bool IsMatchRequireMaxLv( int level) const;

	void PickItemBySeparateChance( LootList &lootList,bool isCountLoot ,bool is_exploit = false) const; 

private:
	const LootFeature*	m_db;
	ItemMap			m_items;
	
};
