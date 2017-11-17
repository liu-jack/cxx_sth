#pragma once

#include "structDef.h"


namespace pb
{
    class GxDB_Quest;
    class GxDB_Behavior;
    class FinishMask;
}

//set byte alignment 
#pragma pack(push,1)


struct QuestLogTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( QuestLogTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    } 

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint32 GetLogicalKey( void) const
    {
        return quest_id;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "questlog";
    }

    static const char* GetFormat()
    {
        return "bbut";
    }

    void SaveTo(pb::GxDB_Quest& msg);
    void LoadFrom(const pb::GxDB_Quest& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      quest_id;
    time_t      active_time;   
} ;


struct QuestCompleteTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( QuestCompleteTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    } 

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "quest_completed";
    }

    static const char* GetFormat()
    {
        return "bbs";
    }

    void SaveTo(pb::FinishMask& msg);
    void LoadFrom(const pb::FinishMask& msg);
    void SetFinishMask(uint32 finishQuestId);
public:
    IdType      id ;
    uint64      player_id;
    string      finish_mask;
} ;

struct QuestCounterTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( QuestCounterTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    } 

    uint64 GetLogicalKey( void) const
    {
        return behavior_type;
    }

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "quest_counter";
    }

    static const char* GetFormat()
    {
        return "bbbut";
    }

    void SaveTo(pb::GxDB_Behavior& msg);
    void LoadFrom(const pb::GxDB_Behavior& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint64      behavior_type;
    uint32      count;
    time_t      last_trigger_time;
} ;



#pragma pack(pop)