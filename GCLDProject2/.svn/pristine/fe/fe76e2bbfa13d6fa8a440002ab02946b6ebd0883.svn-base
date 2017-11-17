#ifndef GUID_INFO_TABLE_H__
#define GUID_INFO_TABLE_H__

#include "structDef.h"

//set byte alignment 
#pragma pack(push,1)

struct GuidInfoTable : public BaseStruct
{
public:
	typedef uint64 Key ;

public:
	INIT_GAMEDATA( GuidInfoTable ) ;

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return id ;
	} 

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetTableName()
	{
		return "guid_info";
	}

	static const char* GetFormat()
	{
		return "buuuus";
	}

public:
	IdType      id ;
	uint32      regionId ;
	uint32      serverId ;
	uint32      type ;
	uint32      curSeed ;
	std::string reuseLowId ;
} ;



#pragma pack(pop)

#endif

