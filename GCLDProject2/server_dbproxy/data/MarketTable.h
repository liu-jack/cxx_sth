#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_Market_All_Info;
    class GS2C_Market_Collect;
    class GS2C_Market_Black;
    class GS2C_Market_Buy;
    class GS2C_Market_Mystery_Shop;
}

//set byte alignment 
#pragma pack(push,1)

struct MarketTable : public BaseStruct {
public:
	
    INIT_GAMEDATA(MarketTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "market"; }
	static const char* GetFormat()
	{
        return "bbbbb" 
            "uuuuu" "uuuuu" "uuuuu" 
            "uuuuu" "bbbbb"
            "uu" //1
            "uu" //2
            "uu" //3
            "uu" //4
            "uu" //5
            "uu" //6
            "uu" //7
            "uu" //8
            "uu" //9
            "uu" //10
            "uu" //11
            "uu" //12
            "uu" //13
            "uu"//14
			"uuuuu""ubuubu";
	}
    void SaveTo(pb::GS2C_Market_All_Info& msg);
    void LoadFrom(const pb::GS2C_Market_Mystery_Shop& msg);
    void LoadFrom(const pb::GS2C_Market_Collect& msg);
    void LoadFrom(const pb::GS2C_Market_Black& msg);
    void LoadFrom(const pb::GS2C_Market_Buy& msg);
	struct ItemPro
	{
		uint32 item_id;
		time_t use_time;
		uint32 left_times;
	};
public:
    IdType      id;
    uint64      player_id;
    uint64      black_time;
    uint64      leftBuyNumAddTime;
    uint64      buyContentRefreshTime;

    uint32      buy_content[3][5];

    uint32      mystery_shop_progress[5];
    uint64      mystery_shop_take_item_time[5];

    uint32      suits_part_item[14][2];
	uint32		collect_times[5];
	ItemPro     item_lst[2];
};


#pragma pack(pop)
