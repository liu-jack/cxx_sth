#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
    class CreateGiftCode;
}
#pragma pack(push,1)

struct GiftBagTable
{
public:
    INIT_GAMEDATA( GiftBagTable ) ;
	DECLARE_INCREASEID_FUCTION(uint64,uint32,id,"id");
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return giftId ;
	}

    static const char* GetLogicalKeyName( void )
    {
        return "giftId" ;
    }

    static const char* GetTableName()
    {
        return "giftbag";
    }

    static const char* GetFormat()
    {

        return "busttusut";
    }

	void LoadFrom(const pb::CreateGiftCode& info);
public:
    uint64			id;
    uint32			giftId;
    std::string		giftName;
    time_t			beginTime;
    time_t			endTime;
	uint32			valid;
    std::string		gifts;
	uint32			reuse;
	time_t			createTime;
} ;

#pragma pack(pop)

