#ifndef ITEM_BASE_TABLE_H__
#define ITEM_BASE_TABLE_H__

#include "structDef.h"


namespace pb
{
    class GxDB_ItemInfo;
    class GxDB_EquipInfo;
    class GS2C_LootRecord;
    class GS2C_LotteryFreeRecord;
}

//set byte alignment 
#pragma pack(push,1)

#define  EQUIPITEMSKILLATTRCOUNT  4

struct ItemBaseTable : public BaseStruct
{
public:
	

public:
	static const bool HasIndex()
	{
		return true;
	}
	
	INIT_GAMEDATA( ItemBaseTable ) ;


	const Key& GetKey( void ) const
	{
		return id ;
	}

    uint32 GetLogicalKey( void) const
    {
        return slot_id;
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
		return "item_base";
	}

	static const char* GetFormat()
	{
		return "bbuuuuut";
	}

    void SaveTo(pb::GxDB_ItemInfo& msg);
    void LoadFrom(const pb::GxDB_ItemInfo& msg);

public:
	IdType      id ;
	uint64      player_id;
	uint32      slot_id;
    uint32      proto_id;
	uint32      count;
    uint32      is_new;
	uint32      is_sold;
	time_t      repo_end_time;
} ;


struct ItemEquipTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( ItemEquipTable ) ;

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
        return slot_id;
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
        return "item_equip";
    }

    static const char* GetFormat()
    {
        return "bbuuuuuuuuuu";
    }

    void SaveTo(pb::GxDB_EquipInfo& msg);
    void LoadFrom(const pb::GxDB_EquipInfo& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      slot_id;
    uint32      equip_char_id;
	uint32      attr[EQUIPITEMSKILLATTRCOUNT];
    uint32      level[EQUIPITEMSKILLATTRCOUNT];
} ;



//////////////////////////////////////////////////////////////////////////
// loot 

struct LootRecordTable : public BaseStruct
{
public:
    
    typedef uint64 PlayerKey;

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( LootRecordTable ) ;

    const Key& GetKey( void ) const
    {
        return id ;
    }

    PlayerKey GetLogicalKey( void) const
    {
        return loot_type_id;
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
        return "loot_round_record";
    }

    static const char* GetFormat()
    {
        return "bbbuu";
    }

    void SaveTo(pb::GS2C_LootRecord& msg) const;
    void LoadFrom(const pb::GS2C_LootRecord& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint64      loot_type_id;
    uint32      roll_success_count;
    uint32      roll_count;
} ;

//////////////////////////////////////////////////////////////////////////

struct LotteryFreeTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( LotteryFreeTable ) ;

    const Key& GetKey( void ) const
    {
        return id ;
    }

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint32 GetLogicalKey() const
    {
        return lottery_type;
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
        return "lottery_free_record";
    }

    static const char* GetFormat()
    {
        return "bbuut";
    }

    void SaveTo(pb::GS2C_LotteryFreeRecord& msg) const;
    void LoadFrom(const pb::GS2C_LotteryFreeRecord& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      lottery_type;
    uint32      remain_free_count;
    time_t      next_free_time;
} ;

#pragma pack(pop)

#endif

