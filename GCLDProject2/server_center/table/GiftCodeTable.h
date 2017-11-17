#pragma once
#include "mysql_common/structDef.h"
namespace pb
{
	class CreateGiftCode;
}

//set byte alignment 
#pragma pack(push,1)

struct GiftCodeTable{
public:
    INIT_GAMEDATA(GiftCodeTable);
	DECLARE_INCREASEID_FUCTION(uint64,string,id,"id");
	static const bool HasIndex(){ return true; }
	const Key& GetKey( void ) const{ return giftCode ;}
    static const char* GetLogicalKeyName(void){ return "giftCode"; }
    static const char* GetTableName(){ return "giftcode"; }
    static const char* GetFormat()
    {
        return "busubsts";
    }
  void LoadFrom(const pb::CreateGiftCode& info);
  bool CheckChannel(int channel);
public:
    uint64      id;
    uint32      giftId;
    string      giftCode;
    uint32      serverId;
	uint64      playerId;
	string      platNo;
    time_t      useTime;
    string      mark;
};


struct GiftCodeTableUse{
public:
	INIT_GAMEDATA(GiftCodeTableUse);
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
	static const bool HasIndex(){ return false; }
	const Key& GetKey( void ) const{ return id;}
	static const char* GetTableName(){ return "giftcodeuse"; }
	static const char* GetFormat()
	{
		return "busubst";
	}
	void LoadFrom(const GiftCodeTable& table);
public:
	uint64      id;
	uint32      giftId;
	string      giftCode;
	uint32      serverId;
	uint64      playerId;
	string      platNo;
	time_t      useTime;
};

#pragma pack(pop)
