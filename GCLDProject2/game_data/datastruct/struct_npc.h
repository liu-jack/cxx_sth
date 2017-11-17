
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
#define  ATTR_VALUE_COUNT 12

//set byte alignment 
#pragma pack(push,1)


struct CreatureVenderItem
{
	NO_KEY_FUNC
	CreatureVenderItem();
	DECLARE_EXTRA_SIZE_ZERO
	static const char* GetTableName()
	{
		return "creature_vender";
	}
	static const char* GetFormat()
	{
		return "uuuuu";
	}
	uint32 entry;
	uint32 itemId;
	uint32 stackcount;
	uint32 maxcount;
	uint32 recoverySecond;

};

struct CreatureAI
{
	INIT_GAMEDATA(CreatureAI)
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
		return "creature_ai";
	}
	static const char* GetFormat()
	{
		return "uusss";
	}
	uint32 Id;
	uint32 name;
	std::string idel;
	std::string combat;
	std::string death;
};

STRING_DB_STRUCT(String_AI)
STRING_DB_STRUCT(String_Achievement)


struct Script
{
	Script();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "scripts";
	}
	static const char* GetFormat()
	{
		return "uss";
	}

	uint32 Id ;
	std::string scriptFile;
	std::string desc;
};


struct CameraWayPoint
{
	CameraWayPoint();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "id";
	}
	static const char* GetTableName()
	{
		return "to_create_camera_waypoint";
	}
	static const char* GetFormat()
	{
		return "uuufffffff";
	}
	uint32 Id;
	uint32 FirstPointId;
	uint32 NextPointId;	
	float x;		
	float y;
	float z;	
	float dx; 
	float dy; 
	float dz; 
	float speed;

};

enum NPC_RANDOM_STYLES
{
	NPC_STYLE_ALL,
	NPC_STYLE_FACE,
	NPC_STYLE_HAIR,
	NPC_STYLE_BEARD,
	NPC_STYLE_EAR,
	NPC_STYLE_EARRING,
	NPC_STYLE_HORN,
	NPC_STYLE_SPECIAL,
	NPC_STYLE_SKIN_COLOR,
	NPC_STYLE_HAIR_COLOR,

	NPC_STYLE_COUNT,
};

struct CreatureRandomAppearance
{
	INIT_GAMEDATA(CreatureRandomAppearance)

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
		return "id";
	}
	static const char* GetTableName()
	{
		return "creature_random_appearance";
	}
	static const char* GetFormat()
	{
		return "uuuuuuu";
	}

	uint32 Id;
	uint32 Entry;
	uint32 Race;	// enum PLAYER_RACE
	uint32 Gender;	// 0:Male, 1:Female
	uint32 Style;	// enum NPC_RANDOM_STYLES
	uint32 Value;
	uint32 Chance;
};

struct NpcDisplay
{
	NpcDisplay();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "id";
	}
	static const char* GetTableName()
	{
		return "npc_display";
	}
	static const char* GetFormat()
	{
		return "u"         //
			"ssssssss"            //
			"ssssssss"         //
			"uu"         //
			"fffff"            //
            "uuu"
            "fffu"
			;
	}
	enum {
		FLAG_TYPE_CREATURE = 0x0001, //Is Creature or Human NPC
		FLAG_TYPE_HUMAN	   = 0x0002,
		FLAG_TYPE_PLAYABLE = 0x0004,

		FLAG_TYPE_AVATAR   = FLAG_TYPE_HUMAN|FLAG_TYPE_PLAYABLE,
	};
	static const int	MODEL_EFFECT_COUNT = 6;

	uint32 Id;
	std::string description;	// 
	std::string model_icon;
	std::string model_main;
	std::string texture_diff;
	std::string vfx_node[MODEL_EFFECT_COUNT];
	std::string vfx_node_binding[MODEL_EFFECT_COUNT];
	uint32 sound_info;
	uint32 model_scale;
	float capsule_x; //npc包围盒X轴大小
	float capsule_y; //npc包围盒Y轴大小
	float capsule_z; //npc包围盒Z轴大小
	float shadow_x; //阴影宽度
	float shadow_z; //阴影长度
    uint32 move_speed;
    uint32 combat_interval;
    uint32 combat_skill_id;

    float capsule_x_offset;
    float capsule_y_offset;
    float capsule_z_offset;
	
	uint32 model_id;

};

STRING_DB_STRUCT(String_Npc)


struct NpcProto
{
	NpcProto();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "entry";
	}
	static const char* GetTableName()
	{
		return "npc_proto";
	}
	static const char* GetFormat()
	{
		return "usuuuu"
            "uuuuuuu"
            "susfsuf"
            "uuuuuuuu";
	}

	uint32 Id;
    string comment;
    uint32 name_id;
	uint32 type_name_id;
    uint32 display_id;
    uint32 special_flag;

    uint32 faction_option;
    uint32 grade_option;
    uint32 combat_option;
    uint32 attack_option;
    uint32 immune_flag;
    uint32 filter_id;
    uint32 model_id;

    string script;
    uint32 char_row;
    string tian_fu;
    float  tian_fu_buff;
    string teshuzhanshu;
    uint32 zhanfafanwei;
    float  zhanfaxishu;

    uint32 kezhijiacheng1;
    uint32 kezhijiacheng2;
    uint32 kezhijiacheng3;
	uint32 extra_resouce_id;
	uint32 extra_resouce_num;
	uint32 npc_quaility;
	uint32 skill_id;
	uint32 army_type;
};





struct NpcAttr
{
    INIT_GAMEDATA(NpcAttr);	 
    static const bool HasIndex()
    {
        return true;
    }
    //index key
    const uint32 GetKey() const
    {
        return id;
    }
    //index key field name
    static const char* GetKeyName()
    {
        return "id";
    }
    //table name
    static const char* GetFormat()
    {
        return "u"
            "ffffffffffff";
    }

    uint32 id;
    float value[12];
};


struct NpcAttrBase : public NpcAttr
{
    INIT_GAMEDATA(NpcAttrBase);	 
    static const char* GetTableName()
    {
        return "npc_attr_base";
    }
};

struct NpcAttrParamCombat : public NpcAttr
{
    INIT_GAMEDATA(NpcAttrParamCombat);	 
    static const char* GetTableName()
    {
        return "npc_attr_param_combat";
    }
};

struct NpcAttrParamGrade : public NpcAttr
{
    INIT_GAMEDATA(NpcAttrParamGrade);	 
    static const char* GetTableName()
    {
        return "npc_attr_param_grade";
    }
};

struct NpcAttrBaseAdditon:public NpcAttr
{
	INIT_GAMEDATA(NpcAttrBaseAdditon);	 
	static const char* GetTableName()
	{
		return "npc_attr_base_addition";
	}
};

struct DB_SellProgram
{
    INIT_GAMEDATA( DB_SellProgram)
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
        return "id";
    }
    static const char* GetTableName()
    {
        return "sell_program";
    }
    static const char* GetFormat()
    {
        //return "usu";
        return "usuftt";
    }
    uint32 Id;
    std::string description;
    uint32 tabNameStrID;//sellProgStrID;   
    float priceScale;
    time_t beginTime;
    time_t endTime;
};

struct DB_SellProgItems
{
	INIT_GAMEDATA( DB_SellProgItems);
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
		return "sell_program_items";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuufuuuu";
	}

    void GetCurrType2Count( CurrType2Count& type2count, uint32 buyCount) const
    {
        float factor = 1.0f;
        if ( curr_raise_count != 0)
        {
            factor = powf(curr_raise_ratio, static_cast<float>(buyCount/curr_raise_count));
        }
        if ( curr_value_1)
        {
            type2count[ curr_type_1] += uint32(curr_value_1*factor);
        }
        if ( curr_value_2)
        {
            type2count[ curr_type_2] += uint32(curr_value_2*factor);
        }         
    }
    enum REQUIRE_TYPE{
        REQUIRE_TYPE_NONE = 0,
        REQUIRE_TYPE_PLAYER_LV = 1,
        REQUIRE_TYPE_VIP = 2,
    };


	uint32 id;                    // 商品序号ID
	uint32 programID;             // 该商品所在的列表ID
	uint32 itemID;                // 商品的物品protoid
	uint32 vip_option;
	uint32 curr_type_1;
	uint32 curr_type_2;
	uint32 curr_value_1;
	uint32 curr_value_2;
	uint32 curr_raise_count;
	float curr_raise_ratio;
	uint32 buy_limit;
	uint32 bulk_count;
	uint32 require_trigger;
	uint32 require_value;
};

struct DB_SellProto
{
	INIT_GAMEDATA( DB_SellProto);
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
		return "sell_proto";
	}
	static const char* GetFormat()
	{
		return "uuu";
	}

	uint32 id;                    // 商品序号ID
	uint32 victim_type;            
	uint32 programID;             // 该商品所在的列表ID
};



#pragma pack(pop)
