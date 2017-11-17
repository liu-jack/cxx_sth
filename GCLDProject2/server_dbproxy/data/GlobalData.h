#ifndef DB_GLOABALDATA_PROXY_H
#define DB_GLOABALDATA_PROXY_H
//#include "../PlayerBaseData.h"
//#include <boost/unordered_map.hpp>
//#include "DbproxyDataDefine.h"
//#include "Singleton.h"
#include "../player_data/CachePtrMap.h"
//#include "EventTable.h"
//#include "def/ConstDef.h"

namespace pb
{
	class SC2G_activity_package;
}

struct ActivityProtoTable;
struct ActivityPlanTable;
struct ServerOpenTimeTable;
struct ActivityRewardTable;
struct ActivityCountryOccupyCityTable;

enum ACTIVITY_MODIFY_TYPE
{
	MODIFY_TYPE_PROTO,
	MODIFY_TYPE_PLAN,
	MODIFY_TYPE_OPEN_TIME,
	MODIFY_TYPE_REWARD,
	MODIFY_TYPE_COUNT,
};
class GlobalData
{
public:
	GlobalData();
	~GlobalData();

	void Init() ;

	inline time_t GetLastSaveTime( void ){    return m_lastSaveTime;    }
	inline void SetLastSaveTime(time_t time){    m_lastSaveTime = time;    }

public:
	void UpdateActivityAllInfo(const pb::SC2G_activity_package& info);
    void OnGameServerRegist(GameServerSession* pSession);
	void UnloadGlobalData();
private:
    //打包功能模块的数据
	void SyncActivityInfo(GameServerSession* pSession);
	void SyncActivityCountryOccupyCityInfo(GameServerSession* pSession);

private:
	// 最后保存时间
	time_t m_lastSaveTime;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // country
public:
	CachePtrMap<uint64, ActivityProtoTable>				m_ActivityProtoTable;
	CachePtrMap<uint64,ActivityPlanTable>				m_ActivityPlanTable;
	CachePtrMap<uint64,ActivityRewardTable>				m_ActivityRewardTable;
	CachePtrMap<uint64,ServerOpenTimeTable>				m_ServerOpenTimeTable;
	CachePtrMap<uint64,ActivityCountryOccupyCityTable> m_ActivityCountryOccupyCityTable;
	std::map<uint64,bool> vec_modify[MODIFY_TYPE_COUNT];
};

#endif
