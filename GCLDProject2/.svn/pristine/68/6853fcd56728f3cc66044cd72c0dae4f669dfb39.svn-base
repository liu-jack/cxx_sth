#include <string>
#include "struct_base.h"



#pragma pack(push,1)

struct Struct_Config
{
public:
	INIT_GAMEDATA(Struct_Config)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	static const char* GetKeyName()
	{
		return "Id";
	}
	static const char* GetTableName()
	{
		return "config";
	}
	static const char* GetFormat()
	{
		return "uss";
	}
	
	uint32 Id;
	std::string key;
	std::string value;
};



#pragma pack(pop)