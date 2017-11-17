#ifndef EVENT_TABLE_H__
#define EVENT_TABLE_H__

#include "structDef.h"
namespace pb
{
	class SD2G_EventRecord;
	class SD2G_EventScheduler;
}
//set byte alignment 
#pragma pack(push,1)

struct EventRecodeTable : public BaseStruct
{
public:
	typedef IdType Key;

public:
	INIT_GAMEDATA(EventRecodeTable);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return id ;
	}

	const Key GetLogicalKey( void ) const
	{
		return event_id ;
	}
	
	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "event_id" ;
	}

	static const char* GetTableName()
	{
		return "event_recode";
	}

	static const char* GetFormat()
	{
		return "bbb";
	}

	void SaveTo(pb::SD2G_EventRecord & record) const;
	void LoadFrom(const pb::SD2G_EventRecord & record);
public:
	IdType      id ;
	uint64      event_id;
	uint64      player_id;
} ;

struct EventSchedulerTable : public BaseStruct
{
public:
	typedef IdType Key;

public:
	INIT_GAMEDATA(EventSchedulerTable);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey(void) const
	{
		return id;
	}

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint64 GetLogicalKey( void) const
    {
        return id;
    }

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName(void)
	{
		return "id";
	}

	static const char* GetTableName()
	{
		return "event_scheduler";
	}

	static const char* GetFormat()
	{
		return "buuuussttbs";
	}

	void SaveTo(pb::SD2G_EventScheduler &event) const;
	void LoadFrom(const pb::SD2G_EventScheduler &event);

public:
	IdType      id;
	uint32      region_id;
	uint32      server_id;
	uint32	    event_type;
	uint32	    send_type;
	string      platform;
	string		channel;
	time_t		begin_time;
	time_t		end_time;
	uint64      mail_id;
	string      receivers;
};

#pragma pack(pop)

#endif

