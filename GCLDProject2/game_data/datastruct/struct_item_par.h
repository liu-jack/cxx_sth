#pragma once

#include "struct_base.h"
#include <string>
#include <numeric>
#include <sstream>
#include <utility>
#include "def/ObjectDefines.h"


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

namespace ItemEnum{

 

	// Inventory Type 格子类型
	enum E_IVTR_TYPE
	{
		IT_NONE,
		IT_RING,
		IT_ACCESSORY,
		IT_HAIR,
		IT_UNDERWEAR,
		IT_NECK,
		IT_HEAD,
		IT_SHOULDER,
		IT_CHEST,
		IT_HAND,
		IT_LEG,
		IT_FEET,
		IT_SINGLE_HAND_WEAPON,
		IT_DOUBLE_HAND_WEAPON,
		IT_PAIR_WEAPON,
		IT_COUNT,
	};

	// 武器握取类型	
	enum E_WEAPON_HOLD
	{
		WEAPON_HOLD_ANY_HAND    =0,  // 任意一手
		WEAPON_HOLD_MAIN_HAND   =1,  // 主单手( 右手 )
		WEAPON_HOLD_OFF_HAND    =2,  // 副单手( 左手 )
		WEAPON_HOLD_DOUBLE_HAND =3,  // 双手
	};

	enum E_BONDING_TYPE
	{
		BD_NONE,        // 无
		BD_ON_PICK,     // 拾取绑定
		BD_ON_EQUIP,    // 装备绑定
		BD_ON_USE,      // 使用绑定
		BD_COUNT,
	};

	//  NPC Entry/ OBJ Entry/ NPC Spawn/ OBJ Spawn/ Quest Zone
	enum E_USE_REQUIRE_TYPE
	{
		USE_REQUIERE_NONE       =0,
		USE_REQUIERE_NPC_ID     =1,
		USE_REQUIERE_OBJ_ID     =2,
		USE_REQUIERE_NPC_SPAWN  =3,
		USE_REQUIERE_OBJ_SPAWN  =4,
		USE_REQUIERE_QUEST_ZONE =5,
	};

	enum E_SPECAIL_FLAG
	{
		E_SPECAIL_FLAG_UNSELLABLE = 0x00000001, // 无法出售
		E_SPECAIL_FLAG_QUEST_ITEM = 0x00000002, // 【保留】有新的枚举 ， 请覆盖。 
		E_SPECAIL_FLAG_LOCK       = 0x00000004, // 物品是否初始为锁定
		E_SPECAIL_FLAG_IDENTIFY   = 0x00000008, // 物品是否初始为需要鉴定
		E_SPECAIL_FLAG_COUNT,
	};

	enum E_QUALITY_TYPE
	{
		QUALITY_0 = 1, // 白
		QUALITY_1 = 2, // 绿
		QUALITY_2 = 3, // 蓝
		QUALITY_3 = 4, // 红
		QUALITY_4 = 5, // 紫
		QUALITY_5 = 6, // 橙
		QUALITY_COUNT
	};

	enum E_UNIQUE_TYPE
	{
		UNIQUE_NONE,
		UNIQUE_EQUIP,
		UNIQUE_HOLD,
		UNIQUE_COUNT,
	};

	// 过期类型
	enum E_EXPIRED_TYPE
	{
		EXPIRED_NONE     =0,
		EXPIRED_NORMAL   =1,
		EXPIRED_ONLINE   =2,
		EXPIRED_COUNT,
	};

	enum E_REQUIRE_ATTR_TYPE
	{
		REQUIRE_ATTR_NONE         =0,
		REQUIRE_ATTR_TARGET_LEVEL =1,
		REQUIRE_ATTR_TYPE_COUNT,
	}; 

	enum E_STAT_TYPE
	{
		STT_NONE                         =0,
		STT_UNIT_WEAPON_DMG_MIN          =1,
		STT_UNIT_WEAPON_DMG_MAX          =2,
		STT_UA_CRITICAL_RATE             =3,
		STT_UA_ATTACK_SPEED_RATE         =4,
		STT_UA_DEVIATE_RATE              =5,
		STT_UA_DODGE_RATE                =6,
		STT_UA_ITEM_COOLDOWN_REDUCE_RATE =7,
		STT_UNIT_FIELD_POWER_MAX         =8,
		STT_UNIT_STAMINA                 =9,
		STT_UNIT_STRENGTH                =10,
		STT_UNIT_AGILITY                 =11,
		STT_UNIT_WISDOM                  =12,
		STT_UNIT_WEAPON_STR              =13,
		STT_UNIT_PHYSICAL_STR            =14,
		STT_UNIT_MAGIC_STR               =15,
		STT_UNIT_ARMOR                   =16,
		STT_UNIT_RESISTANCE_LV           =17,
		STT_UNIT_CRITICAL_LV             =18,
		STT_UNIT_CRITICAL_RESULT_LV      =19,
		STT_UNIT_ATTACK_SPEED_LV         =20,
		STT_UNIT_DEVIATE_LV              =21,
		STT_UNIT_DEVIATE_RESULT_LV       =22,
		STT_UNIT_DODGE_LV                =23,
		STT_UNIT_HP_RESTORE_PER_TIME     =24,
		STT_UNIT_HP_RESTORE_PER_HIT      =25,
		STT_UA_POWER_RESTORE_RATE        =26,
		STT_UA_POWER_CONSUME_FACTOR      =27,
		STT_UNIT_ITEM_COOLDOWN_REDUCE_LV =28,
		STT_COUNT,
	};

	enum ItemUpgradeType
	{
		ITEM_UPGARETE_NOE           =0,
		ITEM_UPGARETE_SMALL         =1,
		ITEM_UPGARETE_BIG           =2,
		ITEM_UPGRADE_SMALL_AND_BIG  =3,
	};

	enum SKILL_ATTR_TYPE
	{
		ATTR_BINGLI = 3000000,							//兵力
		ATTR_ATTACK = ATTR_BINGLI + 10000,				//攻击
		ATTR_DEFENCE = ATTR_ATTACK + 10000,				//防御
		ATTR_CHUANTOU = ATTR_DEFENCE + 10000,			//穿透
		ATTR_DIDANG = ATTR_CHUANTOU + 10000,			//抵挡
		ATTR_POHUAI = ATTR_DIDANG + 10000,				//破坏
		ATTR_SHOUHU = ATTR_POHUAI + 10000,				//守护
		ATTR_COUNT = 7,
	};

	enum ITEM_XILIANSHU_TYPE
	{
		PRIMER_XILIAN = 5030000,            //初级洗练术
		MIDDLE_XILIAN = 5030001,			//中级洗练术
		HIGH_XILIAN = 5030002				//高级洗练术
	};
	enum ITEM_ENTRY_ID
	{
		IRON_COLLECT_MULTI_TEN = 5031500,///征收暴击券
		IRON_COLLECT_MULTI_FOUR = 1,
		SHENQI_CRITICAL_FOUR = 5031600,
		SHENQI_CRITICAL_TEN	 = 5031601,
		SHENQI_CRITICAL_BUFF = 5031602,
	};
} //namespace ItemEnum



class CStringRow;
struct AttrBase ;
struct AttrParam ;
struct ItemTemplateExtra
{
public:
	AttrBase  *pBase ;
	AttrParam *pParam ;

public:
	ItemTemplateExtra( void )
	{
		::memset( this, 0, sizeof( ItemTemplateExtra )) ;
	}
} ;



struct ItemRandomAttr
{
	struct AttrCount
	{
		uint32 min;
		uint32 max;
	};

	ItemRandomAttr();

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
	static const char* GetTableName()
	{
		return "item_random_attr";
	}
	//struct format
	static const char* GetFormat()
	{
		return "us"				
			"uuuu"				
			"uuuu"				
			"uuuu";
	}

	const AttrCount* GetAttrCountByQuality( ItemEnum::E_QUALITY_TYPE quality) const
	{
		if ( quality >= ItemEnum::QUALITY_0 && quality < ItemEnum::QUALITY_COUNT)
		{
			return & (attr_count[ quality - 1]);
		}
		return NULL; 
	}

	void GenAttrPoolFromFlag()
	{
		attrPool.clear();
		uint64 Flag = (uint64)attr_scope_flag1 | (uint64(attr_scope_flag2)<<32);
		uint64 mask = 1;
		for(uint8 i = 1; i <= 64; ++i)
		{
			if (mask & Flag)
			{
				attrPool.push_back( i);
			}
			mask = mask << 1;
		}
	}

	bool operator<( const ItemRandomAttr& p ) const
	{
		return id < p.id;
	}

	bool operator!=(const ItemRandomAttr& p) const  //for  CItemAttrCache IsChanged() 
	{
		bool NotEqual = id != p.id || description != p.description || attr_scope_flag1 != p.attr_scope_flag1 || attr_scope_flag2 != p.attr_scope_flag2;
		if (NotEqual)
		{
			return true;
		}

		return memcmp(&attr_count, &p.attr_count, sizeof(AttrCount)*(ItemEnum::QUALITY_COUNT - 1)) != 0;	
	}

	uint32 id;
	std::string description;



	AttrCount attr_count[ItemEnum::QUALITY_COUNT - 1]; // Q0 not in this. 
	//enum QUALITY
	//{
	//	Q0
	//	Q1,  
	//	Q2,
	//	Q3,
	//	Q4,
	//	Q5,
	//	QUALITY_COUNT
	//};

	uint32 attr_scope_flag1;
	uint32 attr_scope_flag2;

	DECLARE_EXTRA_SIZE_BY_MEMBER(ItemRandomAttr, attrPool);
public:
	std::vector<uint8> attrPool; 
};






STRING_DB_STRUCT(String_Item)



#pragma pack(pop)
