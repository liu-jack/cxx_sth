#ifndef PAY_ITEM_TABLE_H__
#define PAY_ITEM_TABLE_H__

#include <map>
#include <string>

#include "def/TypeDef.h"

//set byte alignment 
#pragma pack(push,1)

struct PayItemIntValue
{
	uint32         id ;
	std::string    product_id;
	uint32         platfrom;
	float          need_rmb;
	uint32         reward_diamond ;
} ;

struct PayItem
{
public:
	PayItemIntValue iVal ;
	std::string     tex ;
} ;

struct PayMailInfo
{
	uint64      id ;
	std::string items;
	std::string title ;
	std::string content ;
};

#pragma pack(pop)

#endif
