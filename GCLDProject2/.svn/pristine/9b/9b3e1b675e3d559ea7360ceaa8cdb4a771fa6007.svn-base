#pragma once

#include "mysql_common/structDef.h"

namespace pb
{
    class SG2C_FeedBack;
}
#pragma pack(push,1)

struct FeedbackTable
{
public:
    INIT_GAMEDATA( FeedbackTable ) ;
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return id ;
	}

    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "feedback";
    }

    static const char* GetFormat()
    {

        return "bbsts";
    }

	void LoadFrom(const pb::SG2C_FeedBack& info);
public:
    uint64			id;
    uint64			playerId;
    std::string		playerName;
    time_t			feedTime;
	std::string		feedback;
} ;

#pragma pack(pop)

