#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_MapLogic_Init;
	class CityEvent;
	class Struct_FOG_INFO;
}

//set byte alignment 
#pragma pack(push,1)

struct MapLogicTable : public BaseStruct {
public:
	
    INIT_GAMEDATA(MapLogicTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "player_map_logic"; }
	static const char* GetFormat()
	{
        return "bbcbbbbbbuu"
            "uuuu"
            "bbbbbb"
			"uuuuu";
	}
    void SaveTo(pb::GS2C_MapLogic_Init& msg);
    void LoadFrom(const pb::GS2C_MapLogic_Init& msg);

public:
    IdType      id;
    uint64      player_id;
    uint8       country_id;
    uint64      take_box;
    uint64      unlock_fog_city[3];
    uint64      country_donate_time;
    uint64      monster_donate_time;
    uint32      army_resource_update_day;
    uint32      army_resource_update_hour;
    IntPair     army_resource_item[2];
    uint64      tonden_begin_time[5];
	uint64		tonden_donate_end_time;
	uint32		tonden_hero[5];
};

// 城池事件
struct MapLogicCityEventTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(MapLogicCityEventTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const{ return event_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "event_id"; }
    static const char* GetTableName(){ return "player_map_city_event"; }
    static const char* GetFormat()
    {
        return "bbubuuccub";
    }
    void SaveTo(pb::CityEvent& msg);
    void LoadFrom(const pb::CityEvent& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      event_id;
    uint64      happen_time;
    uint32      daily_times;
    uint32      city_id;
    uint8       is_begin;
	uint8		is_celebrity;
    uint32      left_num;
    uint64      event_end_time;
};


struct FogRecMap : public BaseStruct{
public:
	
	INIT_GAMEDATA(FogRecMap);

	static const bool HasIndex() { return true; }
	const Key GetKey(void) const { return id; }
	uint64 GetLogicalKey(void) { return uniqueID; }
	void SetDbIncreaseId(const IdType& id_) { id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "uniqueID"; }
	static const char* GetTableName(){ return "player_fog_rec"; }
	static const char* GetFormat()
	{
		return "bbbu"
			"uuuuuuuuuuu";
	}
	void SaveTo(pb::Struct_FOG_INFO& msg);
	void LoadFrom(const pb::Struct_FOG_INFO& msg);

public:
	IdType id;				
	uint64 player_id;	
	uint64 uniqueID;			//独一的id
	uint32 fog_id;				//迷雾id，非关卡id
	IntPair monster_map[5];		//当前迷雾守卫信息
	uint32 monsterAllHp;		//迷雾关所有怪血量上限和	前台显示用

};

#pragma pack(pop)
