

#ifndef SERVER_GOBAL_VALUE_TABLE_H__
#define SERVER_GOBAL_VALUE_TABLE_H__

#include "structDef.h"


//set byte alignment 
#pragma pack(push,1)


struct ServerGobalValueTable : public BaseStruct
{
public:
	typedef uint64 Key ;

public:
	INIT_GAMEDATA( ServerGobalValueTable ) ;

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey( void ) const
	{
		return id ;
	} 

	uint32 GetLogicalKey( void) const
	{
		return index;
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
		return "index" ;
	}

	static const char* GetTableName()
	{
		return "server_gobal_value";
	}

	static const char* GetFormat()
	{
		return "buuuulus" ;
	}

public:
	IdType      id ;
	uint32      regionId ;
	uint32      serverId ;
	uint32      index ;
	uint32      type ;
	int64       int64Value ;
	uint32      intValue ;
	string      strValue ;

} ;


#pragma pack(pop)

#endif

