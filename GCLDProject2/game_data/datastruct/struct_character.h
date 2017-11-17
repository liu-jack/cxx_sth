
#ifndef __STRUCT_CHAR_H__
#define __STRUCT_CHAR_H__
#include <string>
#include "struct_base.h"
#include "def/ObjectDefines.h"

#define CURFACTIONNUM 4
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint64				'l'
int64				'b'
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
struct DB_CharProto
{
    INIT_GAMEDATA(DB_CharProto);

    enum CHAR_TYPE
    {
        CHAR_TYPE_HERO,
        CHAR_TYPE_DOG_FOOD,
    };



    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return index;
    }
    static const char* GetKeyName()
    {
        return "index";
    }
    static const char* GetTableName()
    {
        return "char_proto";
    }
    static const char* GetFormat()
    {
        return 	"usuuu"
            "ssuuuuuuuusf"
            "sufuuuuuu";
    }

    uint32          index;
    std::string     comment;
    uint32          name_id;
    uint32          description_id;
    uint32          display_id;

    std::string     image;
    std::string     image_charactor;
    uint32          type;
    uint32          class_option;
    uint32          combat_option;
    uint32          attack_option;
    uint32          char_lv;
    uint32			char_quality;
    uint32          char_reinforce;
    uint32          char_row;
    std::string     tian_fu;
	float           tian_fu_buff;

	std::string     teshuzhanshu;
	uint32          zhanfafanwei;
	float           zhanfaxishu;
	uint32          kezhijiacheng1;
	uint32          kezhijiacheng2;
	uint32          kezhijiacheng3;
	uint32          unlock_stage_level;
	uint32          choose_cost;
	uint32          skill_id;
};



struct DB_CharAttrBase
{
    INIT_GAMEDATA(DB_CharAttrBase)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return character_id;
    }
    static const char* GetKeyName()
    {
        return "character_id";
    }
    static const char* GetTableName()
    {
        return "char_attr_base";
    }
    static const char* GetFormat()
    {
        return 	"u"
            "ffffffffffff"; //f*12
    }

    uint32 character_id;
    float  base_value[CHARACTER_VALUES_COUNT];
};


struct DB_CharAttrReinforce
{
    INIT_GAMEDATA(DB_CharAttrReinforce);
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return reinforce_lv;
    }
    static const char* GetKeyName()
    {
        return "reinforce_lv";
    }
    static const char* GetTableName()
    {
        return "char_attr_reinforce";
    }
    static const char* GetFormat()
    {
        return 	"u"
            "ffffffffffff"; //f*12
    }

    uint32 reinforce_lv;
    float  value[CHARACTER_VALUES_COUNT];
};


struct DB_CharAttrAdd
{
    INIT_GAMEDATA(DB_CharAttrAdd)
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
        return "char_attr_add";
    }
    static const char* GetFormat()
    {
        return 	"uuu"
            "ffffffffffff"; //f*12
    }
    
    uint32 id;
    uint32 character_id;
    uint32 quality;
    float  add_value[CHARACTER_VALUES_COUNT];
};




struct DB_CharUpgradeXP
{
    INIT_GAMEDATA(DB_CharUpgradeXP)
    
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return index_id;
    }
    static const char* GetKeyName()
    {
        return "index_id";
    }
    static const char* GetTableName()
    {
        return "char_upgrade_lv";
    }
    static const char* GetFormat()
    {
        return 	"u"
            "uuuuuuuuuu"//1
            "uuuuuuuuuu"//2
            "uuuuuuuuuu"//3
            "uuuuuuuuuu"//4
            "uuuuuuuuuu"//5
            "uuuuuuuuuu"//6
            "uuuuuuuuuu"//7
            "uuuuuuuuuu"//8
            "uuuuuuuuuu"//9
            "uuuuuuuuuu"//10
            "uuuuuuuuuu"//11
            "uuuuuuuuuu"//12
            "uuuuuuuuuu"//13
            "uuuuuuuuuu"//14
            "uuuuuuuuuu"//15
            "uuuuu"
            ;
    }
    
    bool CanUpgrade( uint32 curlevel) const
    {
        return curlevel >= 1 && curlevel < MAX_CHARACTER_LEVEL;
    }

    uint32 GetUpgradeXP( uint32 level) const
    {
        if ( CanUpgrade( level))
        {
            return xp_value[level - 1];
        }
        return 0;
    }

    uint32          index_id;
    uint32          xp_value[MAX_CHARACTER_LEVEL-1];
};



struct DB_CharSetSkills
{
    static const uint32 MAX_SKILL_INDEX = 5;
	INIT_GAMEDATA(DB_CharSetSkills)

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
		return "char_set_skills";
	}
	static const char* GetFormat()
	{
		return 	
			"uuuuuuu"
			;
	}
	uint32 id;
	uint32 index_id;
	uint32 skill_id;
	uint32 skill_index;
    enum SKILL_TYPE
    {
        SKILL_TYPE_ACTIVE_SKILL = 0,
        SKILL_TYPE_PASSIVE_SKILL = 1,
        SKILL_TYPE_CAPTAIN_SKILL = 2,
    };

    uint32 skill_type;
	uint32 skill_slot;
	uint32 unlock_quality;
};

struct DB_CharUpgradeLevelItem
{
    INIT_GAMEDATA(DB_CharUpgradeLevelItem)

    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return item_id;
    }
    static const char* GetKeyName()
    {
        return "item_id";
    }
    static const char* GetTableName()
    {
        return "char_upgrade_lv_item";
    }
    static const char* GetFormat()
    {
        return 	"uuu";
    }

    uint32 item_id;
    uint32 slot;
    uint32 xp_value;
};


struct DB_SoldierProto
{
	INIT_GAMEDATA(DB_SoldierProto)

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
		return "soldier_proto";
	}
	static const char* GetFormat()
	{
		return 	"ussusssfffffffffsufuuffffs";
	}

	uint32 id;
	string description;
	string module_name;
	uint32 vfx_attack_type;
	string vfx_attack;
	string vfx_hited;
	string vfx_death;
	float size_scale;
	float ui_scale;
	float time_scale;
	float ground_offset;
	float ui_ground_offset;
	float head_offset;
	float trajectory_time;
	float attack_interval;
	float hpbar_scaling;
	string perfab_position_site;
	uint32 model_num_in_row;
	float line_scaling;
	uint32 attack_sound;
	uint32 tactics_sound;
	float head_buff_offset;
	float ui_x_offset;
	float unlock_ground_offset;
	float unlock_x_offset;
	string vfx_prefeb_skill;
};


struct DB_CharAttrRandom
{
    INIT_GAMEDATA(DB_CharAttrRandom);

    static const uint32 MAX_ATTR_INDEX_CAN_RANDOM = 4;
    static const uint32 RANDOM_ATTR_SIZE = 12;

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
        return "char_attr_random";
    }
    static const char* GetFormat()
    {
        return 	"uuu"
            "ffffffffffff";
    }
    uint32 id;
    uint32 char_class;
    uint32 quality;

    float random_value[RANDOM_ATTR_SIZE]; 
};


STRING_DB_STRUCT(String_Char)
STRING_DB_STRUCT(String_Buff)


struct DB_FightValue
{
	INIT_GAMEDATA(DB_FightValue);
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
		return "fight_value";
	}
	static const char* GetFormat()
	{
		return 	"uuf";
	}
	uint32 id;
	uint32 attr_id;
	float attr_fight_value; 
};


#pragma pack(pop)


#endif //
