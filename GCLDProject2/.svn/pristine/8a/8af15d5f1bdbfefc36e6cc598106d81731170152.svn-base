
#pragma once
#include <string>
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
struct MountPrototype
{
	MountPrototype();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "ID";
	}
	static const char* GetTableName()
	{
		return "mountprototype";
	}
	static const char* GetFormat()
	{
		return "uussuuuus";
	}

	uint32 Id;
	uint32 nameId;
	std::string modelPath;
	std::string skin;
	uint32 talentSpellId;
	uint32 quality;
	uint32 race;
	uint32 attribute;
	std::string soundPrefix;
};

enum MOUNT_ATTRIBUTE
{
	MOUNT_ATTRIBUTE_NONE					= 0x0,
	MOUNT_ATTRIBUTE_HIDEPLAYER				= 0x1,
	MOUNT_ATTRIBUTE_UNMOUNT_AFTER_TALENT	= 0x2,
};

#pragma pack(pop)