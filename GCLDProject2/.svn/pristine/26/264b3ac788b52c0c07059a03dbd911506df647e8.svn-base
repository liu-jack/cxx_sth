
#include <stdlib.h>
#include <algorithm>
#include "CenterDataMgr.h"
#include "mysql_common/MySqlSingleton.h"
#include "OS.h"

char g_hexTable[] = "0123456789ABCDEF";


CenterDataMgr::CenterDataMgr( void )
{
}

CenterDataMgr::~CenterDataMgr( void )
{

}



bool CenterDataMgr::InitData( uint32 regionId )
{
	m_giftBagCacheMap.InitDb(regionId);
	m_giftCodeCacheMap.InitDb(regionId);
	m_activity_plan_cache_map.InitDb(regionId);
	m_server_open_time_cache_map.InitDb(regionId);
	m_activity_proto_cache_map.InitDb(regionId);
	m_activity_reward_cache_map.InitDb(regionId);

	memset(m_keyBuffer,0,sizeof(m_keyBuffer)/sizeof(char));
	m_FeedbackInfos.reset(new FeedbackTableCacheType());
	m_FeedbackInfos->SetDB( &sDatabase ) ;
	m_FeedbackInfos->InitMaxId(regionId) ;
	m_GiftCodeUserTable.reset(new GiftCodeTableUseCacheType());
	m_GiftCodeUserTable->SetDB(&sDatabase);
	m_GiftCodeUserTable->InitMaxId(regionId);
	m_GiftCodeUserTable->SqlLoadAndAddData() ;
	m_giftBagCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_giftCodeCacheMap.LoadDatas(NULL,FlagCleanupReload);
	m_activity_plan_cache_map.LoadDatas(NULL,FlagCleanupReload);
	m_activity_proto_cache_map.LoadDatas(NULL,FlagCleanupReload);
	m_server_open_time_cache_map.LoadDatas(NULL,FlagCleanupReload);
	return true ;
}

bool CenterDataMgr::ReleaseGiftBagTable( uint32 giftId )
{
	return m_giftBagCacheMap.DeleteEntry(giftId);
}

bool CenterDataMgr::ReleaseGiftBagTable( GiftBagTable *pTable )
{
	return m_giftBagCacheMap.DeleteEntry(pTable);
}

void CenterDataMgr::SaveGiftBagTable( GiftBagTable *pTable )
{
	m_giftBagCacheMap.SaveOneMod(pTable);
}

GiftBagTable* CenterDataMgr::AddNewGiftBagTable( GiftBagTable &newTable )
{
	return m_giftBagCacheMap.AddAndAddToCache(&newTable);
}

GiftBagTable* CenterDataMgr::GetGiftBagTable( uint32 giftId )
{
	return  m_giftBagCacheMap.GetElement(giftId);
}

bool CenterDataMgr::ReleaseGiftCodeTable( GiftCodeTable *pTable )
{
	return m_giftCodeCacheMap.DeleteEntry(pTable);
}

void CenterDataMgr::SaveGiftCodeTable( GiftCodeTable *pTable )
{
	m_giftCodeCacheMap.SaveOneMod(pTable);
}

GiftCodeTable* CenterDataMgr::AddNewGiftCodeTable( GiftCodeTable &newTable )
{
	return m_giftCodeCacheMap.AddAndAddToCache(&newTable);
}

GiftCodeTable* CenterDataMgr::GetGiftCodeTable( const string& code )
{
	return  m_giftCodeCacheMap.GetElement(code);
}


bool CenterDataMgr::ReleaseGiftCodeUseTable( GiftCodeTableUse *pTable )
{
	return m_GiftCodeUserTable->RemoveAndDeleteEntry(pTable);
}

void CenterDataMgr::SaveGiftCodeUseTable( GiftCodeTableUse *pTable )
{
	m_GiftCodeUserTable->WriteEntry( pTable );
}

GiftCodeTableUse* CenterDataMgr::AddNewGiftCodeUseTable( GiftCodeTableUse &newTable )
{
	newTable.id =m_GiftCodeUserTable->IncreaseMaxId() ;
	return m_GiftCodeUserTable->SaveAndAddEntry( &newTable ) ;
}

bool CenterDataMgr::GetGiftCodeUseTableByPlayerId(uint64 playerId, std::vector<GiftCodeTableUse*>& codeVec )
{
	GiftCodeTableUseCacheVItr iterCode = m_GiftCodeUserTable->VecBegin();
	for (; iterCode != m_GiftCodeUserTable->VecEnd(); ++iterCode)
	{
		GiftCodeTableUse& codeUse = (*iterCode);
		if (codeUse.playerId == playerId)
		{
			codeVec.push_back(&codeUse);
		}
	}
	return  true;
}

bool CenterDataMgr::GetGiftCodeUseTableByGiftCode(string gfCode, std::vector<GiftCodeTableUse*>& codeVec )
{
	GiftCodeTableUseCacheVItr iterCode = m_GiftCodeUserTable->VecBegin();
	for (; iterCode != m_GiftCodeUserTable->VecEnd(); ++iterCode)
	{
		GiftCodeTableUse& codeUse = (*iterCode);
		if (gfCode.compare(codeUse.giftCode)  == 0)
		{
			codeVec.push_back(&codeUse);
		}
	}
	return  true;
}

bool CenterDataMgr::GiftCodeIsUsed(string gfCode)
{
	GiftCodeTableUseCacheVItr iterCode = m_GiftCodeUserTable->VecBegin();
	for (; iterCode != m_GiftCodeUserTable->VecEnd(); ++iterCode)
	{
		GiftCodeTableUse& codeUse = (*iterCode);
		if (gfCode.compare(codeUse.giftCode)  == 0)
		{
		  return true;
		}
	}
	return  false;
}

bool CenterDataMgr::GiftBagCodeIsUsedByPlayerId( uint64 playerId, uint32 bagId )
{
	GiftCodeTableUseCacheVItr iterCode = m_GiftCodeUserTable->VecBegin();
	for (; iterCode != m_GiftCodeUserTable->VecEnd(); ++iterCode)
	{
		GiftCodeTableUse& codeUse = (*iterCode);
		if (codeUse.playerId == playerId && codeUse.giftId == bagId)
		{
			return true;
		}
	}
	return  false;
}

FeedbackTable* CenterDataMgr::AddNewFeedBackTable( FeedbackTable &newTable )
{
	newTable.id =m_FeedbackInfos->IncreaseMaxId() ;
	return m_FeedbackInfos->SaveAndAddEntry( &newTable ) ;
}

StringVectorPtr CenterDataMgr::getGiftCode( uint32_t amount )
{
	StringVectorPtr resultPtr(new std::vector<std::string>);
	uint64_t curTime = ::time(NULL);
	uint32_t randfaction = curTime & 0x00000000FFFFFFFF;
	srand(randfaction);
	SFormatParam param;
	param.firstTime = randfaction & 0x0000FFFF;
	param.secondTime = randfaction >> 16;
	while (amount--) {
		param.randomNumber = rand() & 0x0000FFFF;
		dataToHex(reinterpret_cast<const uint8_t*>(&param), m_keyBuffer, sizeof(param));
		resultPtr->push_back(std::string(m_keyBuffer));
	}
	return resultPtr;
}

void CenterDataMgr::dataToHex( const uint8_t data[], char outStr[], size_t len )
{
	for (size_t i = 0; i < len; ++i) {
		uint8_t byte = data[i];
		size_t j = i << 1;

		outStr[j] = g_hexTable[byte >> 4];
		outStr[j + 1] = g_hexTable[byte & 0x0F];
	}
}

bool CenterDataMgr::reloadActivityTables(pb::SC2G_activity_package &msg)
{
	ActivityPlanTableMap::CurPtrCache activity_plan = m_activity_plan_cache_map.GetFreshPtrCache();
	ActivityPlanTableMap::CurCacheTypeMItr iterPlan = activity_plan->MapBegin();
	for (;iterPlan != activity_plan->MapEnd();iterPlan++)
	{
		ActivityPlanTable& ac_pl_table =iterPlan->second;
		ac_pl_table.SaveTo(*msg.add_activity_plan());
	}
	ActivityProtoTableMap::CurPtrCache activity_proto = m_activity_proto_cache_map.GetFreshPtrCache();
	ActivityProtoTableMap::CurCacheTypeMItr iterActive = activity_proto->MapBegin();
	for (;iterActive != activity_proto->MapEnd();iterActive++)
	{
		ActivityProtoTable& ac_proto_table = iterActive->second;
		ac_proto_table.SaveTo(*msg.add_activity_proto());
	}

	ServerOpenTimeTableMap::CurPtrCache server_open_time = m_server_open_time_cache_map.GetFreshPtrCache();
	ServerOpenTimeTableMap::CurCacheTypeMItr iterOpenTime = server_open_time->MapBegin();
	for (;iterOpenTime != server_open_time->MapEnd();iterOpenTime++)
	{
		ServerOpenTimeTable& ser_op_time_table =  iterOpenTime->second;
		ser_op_time_table.SaveTo(*msg.add_server_open_time());
	}

	ActivityRewardTableMap::CurPtrCache activity_reward = m_activity_reward_cache_map.GetFreshPtrCache();
	ActivityRewardTableMap::CurCacheTypeMItr iterReward = activity_reward->MapBegin();
	for (;iterReward != activity_reward->MapEnd();iterReward++)
	{
		ActivityRewardTable& ac_reward_table =  iterReward->second;
		ac_reward_table.SaveTo(*msg.add_activity_reward());
	}
	return true;
}	