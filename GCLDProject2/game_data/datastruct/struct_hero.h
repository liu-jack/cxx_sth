#pragma once
#include "struct_base.h"

#pragma pack(push,1)

//Notice：在ActiveDbTables.h中注册

struct DB_Hero
{
    INIT_GAMEDATA(DB_Hero)
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
		return "char_proto";
	}
	static const char* GetFormat()
	{
		return "usuuus" "uuuuuuuusf" "suf" "uuu" "uu";
	}

    uint32   id;
    std::string name;
    uint32      name_id;
    uint32      quality;  //品质 蓝红紫橙
    uint32      display_id;
    std::string image;

    uint32      hp;
    uint32      attack;
    uint32      defence;
    uint32      hp_ratio;
    uint32      attack_ratio;
    uint32      defence_ratio;
    uint32      tong;
    uint32      yong;
    std::string talent_landform; //天赋地形
    float    talent_landform_buff;

    std::string special_zhanshu; // 地形id|战术id
    uint32       skill_range;
    float     skill_ratio;

    uint32      addition_city;      //城池克制加成
    uint32      addition_no_city;   //非城池克制加成
    uint32      addition;       //加强克制

    uint32      special_pro;    //特殊属性
    uint32      soldier_row;    //初始士兵排数
};

#pragma pack(pop)