#pragma once

#include "struct_base.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
index string		'k'
*/

//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Item Table


struct ModuleCategory
{
	INIT_GAMEDATA(ModuleCategory)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "categoryID";
	}
	static const char* GetTableName()
	{
		return "module_proto_category";
	}
	static const char* GetFormat()
	{
		return "us";
	}

	uint32 Id;
	std::string Name;
};

struct ModuleProto
{
	INIT_GAMEDATA(ModuleProto)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "moduleID";
	}
	static const char* GetTableName()
	{
		return "module_proto";
	}
	static const char* GetFormat()
	{
		return "usu";
	}

	uint32 Id;
	std::string Name;
	uint32 categoryId;
};
 
struct ModuleTriggerProto
{
	INIT_GAMEDATA(ModuleTriggerProto)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "triggerID";
	}
	static const char* GetTableName()
	{
		return "module_trigger_proto";
	}
	static const char* GetFormat()
	{
		return "uuu" "uuuu";
	}

	uint32 Id;
	uint32 Type;
	uint32 Value;

	uint32 Delay;
	uint32 ActorType;
	uint32 ActorValue;
	uint32 ModuleId;

};
struct ModuleConditionProto
{
	INIT_GAMEDATA(ModuleConditionProto)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "conditionID";
	}
	static const char* GetTableName()
	{
		return "module_condition_proto";
	}
	static const char* GetFormat()
	{
		return "uuu" "iiiiiiiii" "uu";
	}

	uint32 Id;
	uint32 condition_type;
	uint32 Not;

	int32 param1;
	int32 param2;
	int32 param3;
	int32 param4;
	int32 param5;
	int32 param6;
	int32 param7;
	int32 param8;
	int32 param9;

	uint32 logicalRelationShip;
	uint32 triggerId;
};

struct ModuleActionProto
{
	INIT_GAMEDATA(ModuleActionProto)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "actionID";
	}
	static const char* GetTableName()
	{
		return "module_action_proto";
	}
	static const char* GetFormat()
	{
		return "uu" "fffffffff" "u";
	}

	uint32 Id;
	uint32 action_type;

	float param1;
	float param2;
	float param3;
	float param4;
	float param5;
	float param6;
	float param7;
	float param8;
	float param9;

	uint32 triggerId;
};

#pragma pack(pop)