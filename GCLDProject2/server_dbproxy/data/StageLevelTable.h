#ifndef STAGElEVEL_TABLE_H__
#define STAGElEVEL_TABLE_H__

#include "structDef.h"

namespace pb{
	class GS2C_StageReward;
	class GS2C_StageLevel;
}

//set byte alignment 
#pragma pack(push,1)

struct StageLevelTable : public BaseStruct
{
public:
	typedef IdType Key;

public:
	INIT_GAMEDATA( StageLevelTable);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey(void) const
	{
		return id;
	}

	uint32 GetLogicalKey( void) const
	{
		return level_id;
	}
    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
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
		return "stage_level";
	}

	static const char* GetFormat()
	{
		return "bbuuu";
	}

	void SaveTo( pb::GS2C_StageLevel& msg);
	void LoadFrom( const pb::GS2C_StageLevel& msg);

public:
	IdType      id;
	uint64      player_id;
	uint32      level_id;
	uint32      fight_count;
	uint32      is_clearance;
};


struct EndlessLevelTable : public BaseStruct
{
public:
    typedef IdType Key;

public:
    INIT_GAMEDATA( EndlessLevelTable);
public:
    static const bool HasIndex()
    {
        return true;
    }

    const Key GetKey(void) const
    {
        return id;
    }

    uint32 GetLogicalKey( void) const
    {
        return level_id;
    }
    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
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
        return "stage_level_endless";
    }

    static const char* GetFormat()
    {
        return "bbuuuuut";
    }

    void SaveTo( pb::GS2C_StageLevel& msg);
    void LoadFrom( const pb::GS2C_StageLevel& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      level_id;

    uint32      cur_floor ;
    uint32      history_max_floor ;
    uint32      reset_count ;
    uint32      sweep_count ;

    time_t      data_valid_time ;
};

#pragma pack(pop)              

#endif

