
#pragma once
#include <string>
#include "struct_base.h"
#include "struct_faction.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



struct DB_BasePlayer
{
	INIT_GAMEDATA(DB_BasePlayer);
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return level;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "char_up";
    }
    static const char* GetFormat()
    {
        return 
            "uus" 
            ;
    }

	uint32 level;
	uint32 xp;
	string level_up_reward;
};


struct DB_VipLevelPro
{
    INIT_GAMEDATA(DB_VipLevelPro);
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id ;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "vip_proto" ;
	}
	static const char* GetFormat()
	{
		return "usuuuussuussuuss" ;
	}

public:
	uint32        id;
	std::string    commetns;
	uint32        vip_level ;
	uint32        vip_exp ;
	uint32        reward01_type ;
	uint32        reward01_description ;
	std::string   reward01_id ;
	std::string   reward01_num ;
	uint32        reward02_type ;
	uint32        reward02_description ;
	std::string   reward02_id ;
	std::string   reward02_num ;
	uint32        reward03_type ;  
	uint32        reward03_description;
	std::string   reward03_id ;
	std::string   reward03_num ;
};



enum PLAYER_PREVIEW_FEATURES_INFO
{
	PLAYER_PREVIEW_FEATURES = 12,
};

struct PlayerPreviewFeatures
{
	PlayerPreviewFeatures();
	DECLARE_EXTRA_SIZE_ZERO
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
		return "playerpreview_features";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuuu"
			"uuuuuu"
			"uuuuuuu"
			"iiiiiiiiiiii"
			"iiiiiiiiiiii"
			"iiiiiiiiiiii"
			"iiiiiiiiiiii"
			"uuuuuuuu"
			"iiiiiiiiiiii";
	}

	uint32 id;
	uint32 race;// 种族标识
	uint32 gender;// 性别标识
	uint32 cls;// 职业标识
	uint32 face_default_set;
	uint32 hair_default_set;

	uint32 skin_color_default_set;
	uint32  hair_color_default_set;
	uint32  model_scale;
	uint32  head_show;

	uint32 chest_show;
	uint32 leg_show;
	uint32 shoulder_show;
	uint32 hand_show;
	uint32 foot_show;

	uint32 vogue;
	uint32 mount;
	uint32 weapon_flag;
	uint32 main_hand_show;
	uint32 off_hand_show;
	uint32 display_map;

	int32 face_option[PLAYER_PREVIEW_FEATURES];
	int32 hair_option[PLAYER_PREVIEW_FEATURES];
	int32 skin_color_option[PLAYER_PREVIEW_FEATURES];
	int32 hair_color_option[PLAYER_PREVIEW_FEATURES];

	uint32 head_initial;
	uint32 chest_initial;
	uint32 leg_initial;
	uint32 shoulder_initial;
	uint32 hand_initial;
	uint32 foot_initial;
    uint32 brief_initial;
    uint32 bra_initial;
	
	int32 skin_color_show[PLAYER_PREVIEW_FEATURES];

	// this marks the end of the fields loaded from db - don't remove the comment please 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//next things are runtime.
};

struct DB_HeadIconUnlock
{
	INIT_GAMEDATA(DB_HeadIconUnlock)

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
		return "char_image_unlock";
	}
	static const char* GetFormat()
	{
		return 	"uusuuuusu";
	}

	uint32 id;
	uint32 role_image_id;
	string image_name;

	uint32 unlock_condition; 
	uint32 is_need_buy;
	uint32 cost_type;
	uint32 cost_num;
	string huge_image_name;
	uint32 is_new;
};


struct DB_ResourceInit
{
	INIT_GAMEDATA(DB_ResourceInit)

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
		return "resource_initialize";
	}
	static const char* GetFormat()
	{
		return 	"usuu";
	}

	uint32 id;
	string image_name;
	uint32 item_id; 
	uint32 init_num;
};

#pragma pack(pop)