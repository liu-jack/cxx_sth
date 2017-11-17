#ifndef PLAYERINFO_TABLE_H__
#define PLAYERINFO_TABLE_H__

#include "structDef.h"

//namespace pb
//{
//    class ObjectFullValue;
//    class ObjectValueUpdate;
//    class GS2C_PlayerString;
//}
//set byte alignment 
#pragma pack(push,1)

struct PlayerInfoTable
{
public:
    INIT_GAMEDATA( PlayerInfoTable );
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
public:
    static const bool HasIndex()
    {
        return true;
    }
    const Key& GetKey( void ) const
    {
        return playerId ;
    }
    static const char* GetLogicalKeyName( void )
    {
        return "playerId" ;
    }

    static const char* GetTableName()
    {
        return "player_info";
    }

    static const char* GetFormat()
    {

        return "bbb"
            "uucuuuuuuuuus";
    }

  /*  void SaveTo( pb::ObjectFullValue& msg) const;
    void LoadFrom( const pb::ObjectFullValue& msg);
    void LoadUpdateFrom( const pb::ObjectValueUpdate& msg);


    float* GetIndexFloatPtr( uint32 index);
    int32* GetIndexInt32Ptr( uint32 index);
    uint64* GetIndexUint64Ptr( uint32 index);
    std::string* GetIndexStringPtr( uint32 index);

    const float* GetIndexFloatCPtr( uint32 index) const;
    const int32* GetIndexInt32CPtr( uint32 index) const;
    const uint64* GetIndexUint64CPtr( uint32 index) const;
    const std::string* GetIndexStringCPtr( uint32 index) const;*/


public:
    uint64      id;
    uint64      accountId;
    uint64      playerId;

    uint32      regionId;
    uint32      serverId;
	uint8      country_id;
    uint32      slot;
	uint32      platformId;
	uint32      channelId;
	
	uint32      flags;
	uint32      headIcon ;//Íæ¼ÒÍ·Ïñid
    uint32      level;
	uint32      vipLv;
	uint32      honor;
    uint32      palaceLowOfficeId;
    std::string name;
} ;

#pragma pack(pop)

#endif

