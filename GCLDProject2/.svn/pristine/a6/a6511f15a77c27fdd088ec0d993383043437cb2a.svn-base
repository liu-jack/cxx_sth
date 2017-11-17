#ifndef CENTERDATAMGR_H__
#define CENTERDATAMGR_H__
#include "Singleton.h"
#include <boost/cstdint.hpp>
#include "mysql_common/structDef.h"
#include "table/GiftBagTable.h"
#include "table/GiftCodeTable.h"
#include "table/FeedbackTable.h"
#include "ptr_container/TableCachePtrMap.h"
#include "table/activity_plan.h"
#include "table/activity_proto.h"
#include "table/server_open_time.h"
#include "table/activity_reward.h"
#include "InterServer.pb.h"

typedef TableCachePtrMap<GiftBagTable> GiftBagMap;
typedef TableCachePtrMap<GiftCodeTable> GiftCodeMap;
typedef TableCachePtrMap<ServerOpenTimeTable> ServerOpenTimeTableMap;
typedef TableCachePtrMap<ActivityPlanTable> ActivityPlanTableMap;
typedef TableCachePtrMap<ActivityProtoTable> ActivityProtoTableMap;
typedef TableCachePtrMap<ActivityRewardTable> ActivityRewardTableMap;

TABLE_TYPEDEF( FeedbackTable )
TABLE_TYPEDEF( GiftCodeTableUse )
using namespace boost;
typedef boost::shared_ptr<std::vector<std::string> > StringVectorPtr;
struct SFormatParam {
	uint16_t firstTime;
	uint16_t secondTime;
	uint16_t randomNumber;
};

class CenterDataMgr: public Singleton<CenterDataMgr>
{
private:
	friend class Singleton<CenterDataMgr> ;
private:
	CenterDataMgr( void ) ;

public:
	~CenterDataMgr( void ) ;
public:
	bool InitData( uint32 regionId ) ;

	bool ReleaseGiftBagTable( uint32 giftId) ;
	bool ReleaseGiftBagTable( GiftBagTable *pTable ) ;
	void SaveGiftBagTable( GiftBagTable *pTable ) ;
	GiftBagTable* AddNewGiftBagTable( GiftBagTable &newTable ) ;
	GiftBagTable* GetGiftBagTable( uint32 giftId );
	GiftBagMap& GetGiftBagCacheMap( void ) {   return m_giftBagCacheMap ;}

	bool ReleaseGiftCodeTable( GiftCodeTable *pTable ) ;
	void SaveGiftCodeTable( GiftCodeTable *pTable ) ;
	GiftCodeTable* AddNewGiftCodeTable( GiftCodeTable &newTable ) ;
	GiftCodeTable* GetGiftCodeTable( const string& code );
	GiftCodeMap& GetGiftCodeCacheMap( void ) {   return m_giftCodeCacheMap ;}

	bool ReleaseGiftCodeUseTable( GiftCodeTableUse *pTable ) ;
	void SaveGiftCodeUseTable( GiftCodeTableUse *pTable ) ;
	GiftCodeTableUse* AddNewGiftCodeUseTable( GiftCodeTableUse &newTable ) ;
	bool GetGiftCodeUseTableByPlayerId(uint64 playerId, std::vector<GiftCodeTableUse*>& codeVec );
	bool GetGiftCodeUseTableByGiftCode(string gfCode, std::vector<GiftCodeTableUse*>& codeVec );
	bool GiftCodeIsUsed(string gfCode);
	bool GiftBagCodeIsUsedByPlayerId(uint64 playerId, uint32 bagId);
	FeedbackTable* AddNewFeedBackTable( FeedbackTable &newTable ) ;

	StringVectorPtr getGiftCode(uint32_t amount);
	bool reloadActivityTables(pb::SC2G_activity_package &msg);
private:
	void dataToHex(const uint8_t data[], char outStr[], size_t len);

private:
	GiftBagMap m_giftBagCacheMap;
	GiftCodeMap m_giftCodeCacheMap;
	ActivityPlanTableMap m_activity_plan_cache_map;
	ServerOpenTimeTableMap m_server_open_time_cache_map;
	ActivityProtoTableMap m_activity_proto_cache_map;
	ActivityRewardTableMap m_activity_reward_cache_map;
	FeedbackTableCache   m_FeedbackInfos ;
	GiftCodeTableUseCache m_GiftCodeUserTable;
	char m_keyBuffer[16];
};


#define sCenterDataMgr (CenterDataMgr::Instance())


#endif
