#pragma once
#include <string>
#include "struct_base.h"
#include <map>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct BaseDefine
{
public:
    INIT_GAMEDATA(BaseDefine)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return id;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "base_define";
    }
    static const char* GetFormat()
    {
        return "uxfx";
    }
    uint32 id;
    float value;

};

#pragma pack(pop)