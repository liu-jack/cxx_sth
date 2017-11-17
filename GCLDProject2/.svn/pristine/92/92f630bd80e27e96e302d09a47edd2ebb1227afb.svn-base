#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_BaoQi_All_Info;
}

//set byte alignment 
#pragma pack(push,1)

struct BaoQiTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(BaoQiTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "bao_qi"; }
    static const char* GetFormat()
    {
        return "bb"
            "ccccu"
            "ccccu"
            "ccccu"
            "ccccu"
            "ccccu"
            "ccccu"
			"uubuubuub";
    }
    void SaveTo(pb::GS2C_BaoQi_All_Info& msg);
    void LoadFrom(const pb::GS2C_BaoQi_All_Info& msg);

    struct BaoQiPro
    {
        uint8  level;
        uint8  exp;
        uint8  star;
        uint8  have_hole;
        uint32 gem;
    };
	struct ItemPro
	{
		uint32 item_id;
		uint32 count;
		uint64 use_time;
	};
public:
    IdType      id;
    uint64      player_id;

    BaoQiPro    pro_lst[6];
	ItemPro     item_lst[3];
};


#pragma pack(pop)
