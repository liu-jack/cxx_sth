
//#pragma once
#ifndef _STRUCT_STATICOBJECT_H_
#define _STRUCT_STATICOBJECT_H_

// ANSI C .h
#include <string.h>
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

struct  StaticObjectInteractiveBinding
{
	StaticObjectInteractiveBinding();
	DECLARE_EXTRA_SIZE_ZERO
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return entry;
	}
	void SetKey(uint32 _id) //only for editor
	{
		entry = _id;
	}
	static const char* GetKeyName()
	{
		return "entry";
	}
	static const char* GetTableName()
	{
		return "staticobject_quest_interactive_binding";
	}
	static const char* GetFormat()
	{
		return "uuu";
	}
	uint32	entry;
	uint32	quest;
	uint32	reserve;
};

struct  StaticObjectItemBinding
{
	StaticObjectItemBinding();
	DECLARE_EXTRA_SIZE_ZERO
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return entry;
	}
	void SetKey(uint32 _id) //only for editor
	{
		entry = _id;
	}
	static const char* GetKeyName()
	{
		return "entry";
	}
	static const char* GetTableName()
	{
		return "staticobject_quest_item_binding";
	}
	static const char* GetFormat()
	{
		return "uuuu";
	}
	uint32	entry;
	uint32	quest;
	uint32	item;
	uint32	reserve;
};


struct StaticObjectGuidepost
{
	INIT_GAMEDATA(StaticObjectGuidepost)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return entry;
	}
	static const char* GetKeyName()
	{
		return "entry";
	}
	static const char* GetTableName()
	{
		return "staticobject_guidepost";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuu";
	}
	//entry of static object.
	uint32	entry;							
    uint32	zone_id[10];
};

struct ObjectFilter
{
	ObjectFilter();
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
		return "Id";
	}
	static const char* GetTableName()
	{
		return "obj_filter";
	}
	static const char* GetFormat()
	{
		return "usu";
	}
	uint32 Id;
	std::string filterName;
	uint32 filterParent;

};

#pragma pack(pop)

#endif
