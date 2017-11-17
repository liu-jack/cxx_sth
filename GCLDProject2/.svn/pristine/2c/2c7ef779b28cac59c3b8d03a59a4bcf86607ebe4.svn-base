#pragma once

#include <boost/unordered_map.hpp>

#include "ptr_container/PtrHashMap.h"
#include "def/TypeDef.h"
#include "SimpleSingleton.h"
#include "datastruct/struct_loot.h"
#include "LootDefine.h"

class LootFeatureLogic;
class LootFilter;
class LootFilterGroup;
class LootList;
struct ILootPackage;

class NetPack;
class Player;
namespace pb{
	class GS2C_LootList;
}
class LootManager : public SimpleSingleton<LootManager>
{
    friend class SimpleSingleton< LootManager>;

	typedef PtrHashMap<uint32, LootFilter>				FilterLootMap;
	typedef PtrHashMap<uint32, LootFeatureLogic>		FeatureMap;
	typedef PtrHashMap<uint32, LootFilterGroup>			EntryIdLootMap;

    //typedef std::map< uint32, uint32 >               LotteryId2FixCorrectFilter;
    typedef boost::unordered_map< uint32, uint32 >               LotteryId2FixCorrectFilter;

public:
	void Init();

	bool HasLoot(int giverType, int entry) const;

    bool FillLootList(Player* player,LootParam& lootparam, LootList& lootList,bool is_exploit = false) const;

    LootFilterGroup* FindLootFilterGroup(int type, int entry) const;

    bool NeedRollRecord(int filterId) const;
	const LootFilter* FindLootFilter(int filterId) const;

    uint32 GetLotteryFixCorrectFilter( uint32 lotteryId) const;

private:
	LootManager(void);

public:
	~LootManager(void);

private:

	void LoadDb_LootFeatures();
	void LoadDb_LootFeatureItems();
	void LoadDb_LootRelations();
	LootFilter* InnerFindLootFilter(int filterId) const;
	void AddFilterFeature(LootFeatureLogic* feature);
	void AddFeature(int type, int entry, LootFilter* filter);


    

private:
	FilterLootMap		       m_filters;
	FeatureMap			       m_features;
	EntryIdLootMap		       m_loots[LOOTTYPE_COUNT];
    LotteryId2FixCorrectFilter m_lotteryId2Filter;
};

#define sLootMgr LootManager::Instance()
