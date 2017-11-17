#pragma once

#include "structDef.h"


namespace pb
{
    class GS2C_SellRandomProgram;
    class GS2C_SellProgramInfo;
    class GS2C_SellRecordList;
    class GS2C_ItemSellRecord;
}

//set byte alignment 
#pragma pack(push,1)

struct SellProgramInfoTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( SellProgramInfoTable ) ; 

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
        return sell_program_id;
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
        return "sell_program_info";
    }

    static const char* GetFormat()
    {
        return "bb"
            "ut";
    }

    void SaveTo(pb::GS2C_SellProgramInfo& msg);
    void LoadFrom(const pb::GS2C_SellProgramInfo& msg);
public:
    IdType      id ;
    uint64      player_id;
    uint32      sell_program_id;
    time_t      record_valid_time;
};

struct SellRandomProgramTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }


    INIT_GAMEDATA( SellRandomProgramTable ) ; 

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
        return victim_type;
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
        return "sell_random_program";
    }

    static const char* GetFormat()
    {
        return "bb"
            "uuutt";
    }

    void SaveTo(pb::GS2C_SellRandomProgram& msg);
    void LoadFrom(const pb::GS2C_SellRandomProgram& msg);
public:
    IdType      id ;
    uint64      player_id;
    uint32      victim_type;
    uint32      random_program_id;
    uint32      today_manual_refresh_count;
    time_t      auto_refresh_time;
    time_t      last_maual_refresh_time;
};

struct SellRecordTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( SellRecordTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    }
    uint32 GetLogicalKey( void) const
    {
        return sell_id;
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
        return "sell_record";
    }

    static const char* GetFormat()
    {
        return "bb"
            "uu";
    }

    void SaveTo(pb::GS2C_ItemSellRecord& msg);
    void LoadFrom(const pb::GS2C_ItemSellRecord& msg);
public:
    IdType      id ;
    uint64      player_id;
    uint32      sell_id;
    uint32      sell_count;
};



#pragma pack(pop)
