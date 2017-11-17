#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice����ActiveDbTables.h��ע��

struct DB_MapCity
{
    INIT_GAMEDATA(DB_MapCity)
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
        return "country";
    }
    static const char* GetFormat()
    {
        return 	
			"uuusuuuss"
			"ssssssssssssu";
    }

    uint32      id;
    uint32      map_id;
    uint32      type;
    string      comment;
    uint32      name_id;
    uint32      landform;  //����
    uint32      country;   //��ʼ��������
    string      link_city;
	string		link_road_id;
	string      guard_npc[10]; //�س�npc
    string      mini_map_color_range;
	string		world_map_color_range;
	uint32		city_module_id;
};
struct DB_MapBox
{
    INIT_GAMEDATA(DB_MapBox)
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
        return "world_boxes";
    }
    static const char* GetFormat()
    {
        return  "ussss";
    }

    uint32      id;
    std::string reward_lst;
    std::string pos_country1;
    std::string pos_country2;
    std::string pos_country3;
};
struct DB_MapEvent
{
    INIT_GAMEDATA(DB_MapEvent)
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
        return "world_events";
    }
    static const char* GetFormat()
    {
        return  "usssuusss" "uussssuu";
    }

    uint32          id;
    std::string     event_display;  //ͼ��
    std::string     happen_trigger;	//��������
    std::string     stop_trigger;
    uint32          interval_sec;	//���Ž������ʱ��
    uint32          times_limit;    //ÿ����༸��
    std::string     reward_lst;     //����
    std::string     select_lst;     //��ѡһ
    std::string     cost_lst;       //����

    uint32          type;			//�����¼����������¼�
    uint32          reward_max_cnt; //ÿ�������� 
    std::string     city_id;
    std::string     headline_text;
    std::string     content_text;
    std::string     half_image_name;
	uint32			event_last_time;
	uint32			type_id;		//��������id
};
struct DB_MapFog
{
    INIT_GAMEDATA(DB_MapFog)
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
        return "fog";
    }
    static const char* GetFormat()
    {
        return  "uuuusussu";
    }

    uint32     id;
    uint32     country;
    uint32     battle_city;
    uint32     show_lv;
    std::string open_list;
    uint32     stageId;
	std::string add_cloud_site;
	std::string  reward_item_id;
	uint32 priority_id;
};


struct DB_TerrainText
{
	INIT_GAMEDATA(DB_TerrainText)
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
		return "terrain_text";
	}
	static const char* GetFormat()
	{
		return  "usus";
	}

	uint32     id;
	string     comment;
	uint32     name_id;
	string     icon_name;
};

struct DB_Military_Supplies
{
    INIT_GAMEDATA(DB_Military_Supplies)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "military_supplies"; }
    static const char* GetFormat() {
        return
            "uuuu";
    }

    uint32          id;
    uint32          time;
    uint32          silver_reward_times;
    uint32          iron_reward_times;
};

struct DB_TunTian_Up
{
	INIT_GAMEDATA(DB_TunTian_Up)
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "tuntian_up"; }
	static const char* GetFormat() {
		return
			"uuu";
	}

	uint32          id;
	uint32          exp;
	uint32          reward_factor;
};

struct DB_Expedition
{
	INIT_GAMEDATA(DB_Expedition);
	static const bool HasIndex(){ return true; }
	const uint32 GetKey() const{ return id; }
	static const char* GetKeyName(){ return "id"; }
	static const char* GetTableName(){ return "troops_move_way"; }
	static const char* GetFormat() {
		return
			"usuuuus";
	}

	uint32	id;
	string	comment;
	uint32	troopCountry;
	uint32	troopType;
	uint32	npcid;
	uint32	npcNum;
	string	npcWay;
};

struct DB_OpenFog
{
	INIT_GAMEDATA(DB_OpenFog)
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
		return "open_fog";
	}
	static const char* GetFormat()
	{
		return 	"uuuu";
	}

	uint32      id;
	uint32 open_num;
	uint32 reward_id;
	uint32 reward_num;
};


#pragma pack(pop)