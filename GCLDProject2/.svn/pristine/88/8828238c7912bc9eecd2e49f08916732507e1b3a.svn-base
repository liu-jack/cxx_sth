#include "EventLogDefine.h"
#include "common/Enum.pb.h"

EventLogs::EventLogs()
{
  m_mapLogs.clear();
}

void EventLogs::SaveTo(pb::Server_Events& logsMsg )
{
	MutexGuard lock( m_mutex );
	MapLogsIter logsIter = m_mapLogs.begin();
	for (; logsIter != m_mapLogs.end(); ++logsIter)
	{
		VecLogs& vecLogs = logsIter->second;
		VecLogsIter logvIter = vecLogs.begin();
		for (; logvIter != vecLogs.end(); ++logvIter)
		{
			EventLogInfo& logInfo = (*logvIter);
			pb::Server_Event* evlogs = logsMsg.add_logs();
			logInfo.SaveTo(*evlogs);
		}
	}
}

void EventLogs::AddEventLog( EventLogInfo& info )
{
	MutexGuard lock( m_mutex );
	MapLogsIter logsIter = m_mapLogs.find(info.event_id);
	if (logsIter != m_mapLogs.end())
	{
		VecLogs& vecLogs = logsIter->second;
		if (info.event_id == (int32)pb::EVENT_PLAYER_MONEY_SYS || info.event_id == (int32)pb::EVENT_PALYER_ITEM_SYS || info.event_id == (int32)pb::EVENT_PLAYER_ARMY_LV_UP)
		{
			VecLogsIter logvIter = vecLogs.begin();
			for (; logvIter != vecLogs.end(); ++logvIter)
			{
				EventLogInfo& logInfo = (*logvIter);
				if (logInfo.IsSameType(info))
				{
					logInfo.CombinSameType(info);
					return;
				}
			}
		}
		vecLogs.push_back(info);
	}
	else
	{
		VecLogs newVecLogs;
		newVecLogs.push_back(info);
		m_mapLogs[info.event_id] = newVecLogs;
	}
}



void EventLogs::ClearAll()
{
	MutexGuard lock( m_mutex );
	MapLogsIter logsIter = m_mapLogs.begin();
	for (; logsIter != m_mapLogs.end(); ++logsIter)
	{
		VecLogs& vecLogs = logsIter->second;
		vecLogs.clear();
	}
	m_mapLogs.clear();
}

bool EventLogInfo::IsSameType( EventLogInfo& other )
{
	if(other.player_id  != player_id)
		return false;
	if(other.event_id  != event_id)
		return false;
	if(other.account_id  != account_id)
		return false;
	if (event_id == (int32)pb::EVENT_PLAYER_MONEY_SYS)
	{
		int32 resionId = params[1].value_int32;
		if (resionId == (int32)pb::IR_BUILDING_LV_UP_COST 
			|| resionId == (int32)pb::IR_CHARACTER_INCREASE_HP_COST
			|| resionId == (int32)pb::IR_OPERATION_REWARD_GET
			|| resionId == (int32)pb::IR_LOTTO_COST)
		{
			if (params[1].value_int32 == other.params[1].value_int32)
			{
				return (params[2].value_int32 == other.params[2].value_int32);
			}
		}
	}
	else if (event_id == (int32)pb::EVENT_PALYER_ITEM_SYS)
	{
		int32 resionId = params[3].value_int32;
		if (resionId == (int32)pb::IR_OPERATION_REWARD_GET
			|| resionId == (int32)pb::IR_LOTTO_COST)
		{
			if (params[3].value_int32 == other.params[3].value_int32)
			{
				return (params[1].value_int32 == other.params[1].value_int32);
			}
		}
	}
	else if (event_id == (int32)pb::EVENT_PLAYER_ARMY_LV_UP)
	{
		return (params[0].value_int32 == other.params[0].value_int32);
	}
	return false;
}

void EventLogInfo::CombinSameType( EventLogInfo& other )
{
	if (event_id == (int32)pb::EVENT_PLAYER_MONEY_SYS)
	{
		params[3].value_int32 += other.params[3].value_int32;
		time = other.time;
	}
	else if (event_id == (int32)pb::EVENT_PALYER_ITEM_SYS)
	{
		params[2].value_int32 += other.params[2].value_int32;
		time = other.time;
	}
	else if (event_id == (int32)pb::EVENT_PLAYER_ARMY_LV_UP)
	{
		params[1].value_int32 = other.params[1].value_int32;
		time = other.time;
	}
}

void EventLogInfo::SaveTo( pb::Server_Event& logMsg )
{
	logMsg.set_account_id(account_id);
	logMsg.set_player_id(player_id);
	logMsg.set_event_id(event_id);
	logMsg.set_time(time);
	std::vector<EventParmSt>::iterator parIter = params.begin();
	for (; parIter != params.end(); ++parIter)
	{
		EventParmSt& curParm = (*parIter);
		pb::EventParm* parm = logMsg.add_params();
		curParm.SaveTo(*parm);
	}
}

void EventParmSt::SaveTo( pb::EventParm& parmMsg )
{
	parmMsg.set_parm_type(parm_type);
	if (parm_type == pb::EventParm_ParmType_INT32)
	{
		parmMsg.set_value_int32(value_int32);
	}
	else if (parm_type == pb::EventParm_ParmType_UINT32)
	{
		parmMsg.set_value_uint32(value_uint32);
	}
	else if (parm_type == pb::EventParm_ParmType_INT64)
	{
		parmMsg.set_value_int64(value_int64);
	}
	else if (parm_type == pb::EventParm_ParmType_UINT64)
	{
		parmMsg.set_value_uint64(value_uint64);
	}
	else if (parm_type == pb::EventParm_ParmType_STRING)
	{
		parmMsg.set_value_str(value_str);
	}
	else if (parm_type == pb::EventParm_ParmType_FLOAT)
	{
		parmMsg.set_value_float((float)value_double);
	}
}
