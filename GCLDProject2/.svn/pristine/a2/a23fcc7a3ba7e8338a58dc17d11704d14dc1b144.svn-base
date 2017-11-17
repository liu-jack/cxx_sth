#pragma once
// operation 运营
// operation 运营
// operation 运营
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



// operation 运营

//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DB_FirstPay
{
public:
    INIT_GAMEDATA(DB_FirstPay);
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
        return "first_pay";
    }
    static const char* GetFormat()
    {
        return "uussu";
    }
    uint32 id;
    uint32 reward01_type;
	std::string reward01_id;
    std::string reward01_num;
    uint32 name_id;
};

#pragma pack(pop)