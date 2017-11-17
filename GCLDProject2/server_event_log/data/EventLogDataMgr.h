#ifndef EVENT_LOG_DATA_MGR_H__
#define EVENT_LOG_DATA_MGR_H__


#include "Singleton.h"
#include "EventLog.h"
class EventLogDataMgr: public Singleton<EventLogDataMgr>
{
private:
	friend class Singleton<EventLogDataMgr> ;

private:
	EventLogDataMgr( void ) ;

public:
	~EventLogDataMgr( void ) ;

public:
	bool InitData( void ) ;

	void CheckCreateNewEventTable(std::string& tableName) ;

	bool CheckCreatTodayTable(int eventId);
	void SaveNewEventlog( EventLog &eventLog ) ;
	int GetTableId(int eventId);

private:
	std::map<int,std::string> m_logTableMap;
	std::map<int,std::string> m_TableNameMap;
	int         s_year;
	int         s_month;
	int         s_day;
};


#define sDataMgr (EventLogDataMgr::Instance())


#endif
