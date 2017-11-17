
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

struct BaseListCama
{
	INIT_GAMEDATA(BaseListCama)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return ID;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "base_list_camera";
	}
	static const char* GetFormat()
	{
		return "usuu";
	}
	uint32 ID;
	std::string camera_name;
    uint32 camera_category;
    uint32 camera_allframe;
};


struct CameraProto
{
    INIT_GAMEDATA(CameraProto)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return ID;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "camera_proto";
    }
    static const char* GetFormat()
    {
        return "ussu";
    }
    uint32 ID;
    std::string desc;
    std::string camera_main;
    uint32 map_info;
};


struct CameraWindow
{
    INIT_GAMEDATA(CameraWindow)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return ID;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "camera_window";
    }
    static const char* GetFormat()
    {
        return "uuuuu";
    }
    uint32 ID;
    uint32 camera_id;
    uint32 frame_start;
    uint32 frame_end;
    uint32 camera_effect;
};


#pragma pack(pop)