
#include "EventLogDataMgr.h"
#include "MySqlSingleton.h"

#include "OS.h"

#include "utility/TimeParser.h"
#include "utility/Utility.h"
#include "Enum.pb.h"

EventLogDataMgr::EventLogDataMgr( void )
:s_year(0)
,s_month(0)
,s_day(0)
{

}


EventLogDataMgr::~EventLogDataMgr( void )
{

}



bool EventLogDataMgr::InitData( void )
{
	s_year = 0;
	s_month = 0;
	s_day = 0;
	m_TableNameMap[1] = "event_log_";
	m_TableNameMap[(int)pb::EVENT_PALYER_ITEM_SYS] = "item_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_MONEY_SYS] = "money_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_BEHAVIOR_STAGE] = "stage_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_BEHAVIOR_LEVELUP] = "playerlvup_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_BEHAVIOR_VIPLEVELUP] = "viplvup_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_TAKE_GIFT_BAG] = "takegift_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_ARMY_LV_UP] = "armylvup_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_EQUIP_PURIFY] = "equippurify_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_GODSTONE_PURIFY] = "godstonepurify_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_CALL_PHANTOM] = "callphantom_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_ACTIVITY] = "activity_";
	m_TableNameMap[(int)pb::EVENT_PLAYER_COUNTRY_TASK] = "country_task_";
	return true ;
}


void EventLogDataMgr::CheckCreateNewEventTable( std::string& tableName )
{
	std::string checkTableSql( "SELECT 1 FROM " ) ;
	checkTableSql += tableName;
	if( !sDatabase.SyncExcute( checkTableSql ) )
	{
		std::string createSql ;
		EventLog::GetCreateTableSql( createSql ) ;
		while(!sDatabase.SyncExcute( createSql ))
		{
		}
	}
}

bool EventLogDataMgr::CheckCreatTodayTable(int eventId)
{
	int nowY =0, nowM =0, nowD =0 ;
	TimeT2Date( ::time( NULL ), nowY, nowM, nowD ) ;
	if( nowY != s_year || nowM != s_month || nowD != s_day )
	{
		s_year  =nowY;
		s_month =nowM;
		s_day   =nowD;
		std::map<int,std::string>::const_iterator nameIter = m_TableNameMap.begin();
		for (; nameIter != m_TableNameMap.end(); ++nameIter)
		{
		  const	std::string& newTable =nameIter->second;
		  int curTbId = nameIter->first;
		  std::string newTableName;
		  Utility::FormatString( newTableName, "%s%u%02u%02u",newTable.c_str(), nowY, nowM, nowD ) ;
		  EventLog::SetTableName(newTableName);
		  CheckCreateNewEventTable(newTableName);
		  m_logTableMap[curTbId] = newTableName;
		}
	}
	int curTableId = GetTableId(eventId);
	std::map<int,std::string>::const_iterator itemrTable = m_logTableMap.find(curTableId);
	if (itemrTable != m_logTableMap.end())
	{
		const std::string& reCod =itemrTable->second;
		EventLog::SetTableName(reCod);
		return true;
	}
	else
	{
		std::map<int,std::string>::const_iterator itemrTable = m_TableNameMap.find(curTableId);
		if (itemrTable != m_TableNameMap.end())
		{
			const	std::string& newTable = itemrTable->second;
			std::string newTableName;
			Utility::FormatString( newTableName, "%s%u%02u%02u",newTable.c_str(), nowY, nowM, nowD ) ;
			EventLog::SetTableName(newTableName);
			CheckCreateNewEventTable(newTableName);
			m_logTableMap[curTableId] = newTableName;
			EventLog::SetTableName(newTableName);
			return true;
		}
	}
	return false;
}

void EventLogDataMgr::SaveNewEventlog( EventLog &eventLog )
{
	if(CheckCreatTodayTable(eventLog.event_id))
	{
		sDatabase.AddAsyncQuery( eventLog.GetInsertSql());
	}
}

int EventLogDataMgr::GetTableId( int eventId )
{
	if (m_TableNameMap.find(eventId) != m_TableNameMap.end())
	{
		return eventId;
	}
	else
	{
		return 1;
	}
}