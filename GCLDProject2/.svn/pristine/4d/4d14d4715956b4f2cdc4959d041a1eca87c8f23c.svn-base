
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

enum AUCTION_TYPE
{
	AT_STARTTIME = 0,
	AT_ENDTIME,
	AT_LEVEL_UP,
	AT_LEVEL_DOWN,
	AT_QUALITY,
	AT_BID,
	AT_BN,
	AT_SELLER,
	AT_TEMPOWNER,
	AT_ITEM,
	AT_ID,
	AT_TYPE,
	AT_SUBTYPE,
	AT_SLOT,
	AT_NUM,
};

enum AUCTION_OPER_FAIL_TYPE
{
	AF_NOT_ENOUGH_MONEY = 0,
	AF_AUCTION_NOT_EXIST,
	AF_NO_RESPONSE,

};

const char s_type_string[AT_NUM][64]
=
{
	"startTime",
	"endTime",
	"level",
	"level",
	"quality",
	"BidPrice",
	"BNPrice",
	"seller",
	"tempOwner",
	"item",
	"Id",
	"type",
	"subType",
	"slot",
};

const char s_type_select_string[AT_NUM][64]
=
{
	">",
	"<",
	">=",
	"<=",
	"<=",
	"=",
	"=",
	"=",
	"=",
	"=",
	"=",
	"=",
	"=",
	"=",
};

struct Auction
{
	INIT_GAMEDATA(Auction)
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
		return "auction_hall";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuuuussu";
	}
	uint32 Id;
	uint32 item;
	uint32 number;
	uint32 level;
	uint32 quality;
	uint32 BidPrice;
	uint32 BNPrice;
	uint32 seller;
	uint32 startTime;
	uint32 endTime;
	uint32 tempOwner;
	uint32 type;
	uint32 subType;
	std::string sellerName;
	std::string tempOwnerName;
	uint32 slot;
};
#pragma pack(pop)