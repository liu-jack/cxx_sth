#ifndef EVENTSLOGSDEFINE_
#define EVENTSLOGSDEFINE_
#include "InterServer.pb.h"
#include "TypeDef.h"
#include "../lock_free/Mutex.h"
#ifdef USE_PTR_HASH_MAP
#include <boost/unordered_map.hpp>
#endif // USE_PTR_HASH_MAP
struct EventParmSt
{
	EventParmSt()
	:parm_type(pb::EventParm_ParmType_INT32)
	,value_int32(0)
	,value_uint32(0)
	,value_int64(0)
	,value_uint64(0)
	,value_str("")
	,value_double(0)
	{

	}
public:
	void SaveTo(pb::EventParm& parmMsg);
public:
	pb::EventParm_ParmType parm_type;
	int32  value_int32;
	uint32 value_uint32;
	int64  value_int64;
	uint64 value_uint64;
	string value_str;
	double  value_double;
};

class EventLogInfo
{
public:
	EventLogInfo()
		:event_id(0)
		,account_id(0)
		,player_id(0)
		,time(0)
	{
	}
public:
	bool IsSameType(EventLogInfo& other);
	void CombinSameType(EventLogInfo& other);
	void SaveTo(pb::Server_Event& logMsg);
public:
	int32 event_id;
	uint64 account_id;
	uint64 player_id;
	int64 time;
	std::vector<EventParmSt> params;
};

class EventLogs
{
public:
	EventLogs();
	void SaveTo(pb::Server_Events& logsMsg);
	void AddEventLog(EventLogInfo& info);
	void ClearAll();

public:
	typedef	std::vector<EventLogInfo> VecLogs;
#ifndef USE_PTR_HASH_MAP
	typedef std::map<int32,VecLogs> MapLogs;
#else
	typedef typename boost::unordered_map<int32,VecLogs> MapLogs;
#endif // USE_PTR_HASH_MAP
	typedef MapLogs::iterator MapLogsIter;
	typedef VecLogs::iterator VecLogsIter;
private:
	MapLogs m_mapLogs;
	mutable Mutex   m_mutex;
};

#endif //EVENTSLOGSDEFINE_