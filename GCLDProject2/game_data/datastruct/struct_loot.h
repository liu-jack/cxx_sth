#ifndef __STRUCT_LOOT_H__
#define __STRUCT_LOOT_H__

#pragma once
#include "struct_base.h"
#include <string>
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Item Table


enum LOOTTYPE{
	LOOTTYPE_NONE = 0,
	LOOTTYPE_STAGELEVEL   = 1,
	LOOTTYPE_ITEM         = 2,
    LOOTTYPE_LOTTERY      = 3,
    LOOTTYPE_EQUIPSHOP    = 4,
    LOOTTYPE_COUNTRYQUEST = 5,
	LOOTTYPE_FREEPURIFY   = 6,
	LOOTTYPE_GOLDENPURIFY = 7,
	LOOTTYPE_GODSTONPURIFY = 8,
    LOOTTYPE_ACT_INVESTMENT = 9, //��սͶ��
    LOOTTYPE_ACT_OCCUPY_CITY = 10, //���ع���
    LOOTTYPE_ACT_DEFENCE_EDGE = 11, //���ر߽�
    LOOTTYPE_ACT_TOURNAMENT = 12, //�������
	LOOTTYPE_REBUILD = 13, //���Ǹ���
	LOOTTYPE_EXPLOIT_BOX = 14,//��ѫ����
	LOOTTYPE_EXPLOIT_RANK = 15,//��ѫ���а���
	LOOTTYPE_DAILY_QUEST_REWARD = 16,//ÿ��������
    LOOTTYPE_COUNT,
};

#define  SHOP_EQUIP_SLOT_COUNT  6
#define  SHOP_EQUIP_BUY_COOLDOWN_SEC_TIME  1800
#define  SHOP_EQUIP_BUY_ONCE_COOLDOWN_SEC_TIME  180

struct LootProto
{	
	INIT_GAMEDATA(LootProto);
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
		return "loot_proto";
	}
	static const char* GetFormat()
	{
		return "uuuu";
	}
	uint32 Id;
	uint32 lootType;    // enum LOOTTYPE{}
	uint32 victimID;
	uint32 lootFilterID;//lootFeatureID;
};

enum LootOwerFlagType
{
	LOOT_OWNER_NORAML =0, // ÿ��һ��
	LOOT_OWNER_ONE    =1, // ����һ��
} ;

struct Loot_Filter//Loot_Display_Filter
{
	INIT_GAMEDATA(Loot_Filter);
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
		return "loot_filter";
	}
	static const char* GetFormat()
	{
		return
			"usuufuuttttu";
	}
	uint32 Id;
	std::string filterName;
	uint32 filterParent;

	enum LootBoxTypes
	{
		Normal = 0,
		Random = 1,
		Correct = 2,
        FixCorrect = 3,
	};

	enum OwnerBoxTypes
	{
		PrivateOwner = 0,
		TeamOwner = 1,
	};

	enum MainWeatherTypes
	{
		Sunny = 0,
		Cloudy = 1,
		Rainy = 2,
		Snowy = 3,
	};

	uint32 loot_option; //
	float  loot_random_rate;
	uint32 loot_correct_count ;
	uint32 loot_correct_mod ;

	// �����޶����ڷ�Χ( ������ )
	int64 lootDateBegin;
	int64 lootDateEnd;

	// �����޶�ʱ�䷶Χ( ʱ���� )
	int64 lootTimeBegin;
	int64 lootTimeEnd;



	uint32 priority;

};


enum IS_MF_FLAGS
{
	IS_MF_FLAG_NOT_TEAM_NEED_VIP =0x00000001,
};

struct LootFeature
{
	LootFeature();
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
		return "loot_feature";
	}
	static const char* GetFormat()
	{
		return "uuuuu";
	}

	uint32 id;
	uint32 filter_id;
	uint32 require_lv_min;
	uint32 require_lv_max;
	uint32 require_vip;
};


struct LootFeatureItems
{
	LootFeatureItems();
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
		return "loot_feature_items";
	}

	static const char* GetFormat()
	{
		return "uuufffuu";
	}

	uint32 id;
	uint32 feature_id;
	uint32 item_id;
	float item_weight;
	float weight_min_rate;  // ���� item_weight �ɱ༭�� �����0~100�ķ�Χֵ
	float weight_max_rate;  // ���� item_weight �ɱ༭�� �����0~100�ķ�Χֵ
	uint32 count_min;
	uint32 count_max;	
};

enum LootCycleCountType
{
	CYCLE_COUNT_TYPE_NOT      =0, //0����Ч
	CYCLE_COUNT_TYPE_ROLE     =1, //1����ɫ
	CYCLE_COUNT_TYPE_ACCOUNT  =2, //2���˺�
	CYCLE_COUNT_TYPE_GUILD    =3, //3������
	CYCLE_COUNT_TYPE_ALL_GAME =4, //4��ȫ��
	CYCLE_COUNT_TYPE_COUNT
};

enum LootCycleCountResetType
{
	CYCLE_COUNT_RESET_TYPE_NULL  =0, //0����Ч
	CYCLE_COUNT_RESET_TYPE_HOUR  =1, //1��ÿСʱ
	CYCLE_COUNT_RESET_TYPE_DAY   =2, //2��ÿ��
	CYCLE_COUNT_RESET_TYPE_WEEK  =3, //3��ÿ��
	CYCLE_COUNT_RESET_TYPE_MONTH =4, //4��ÿ��
	CYCLE_COUNT_RESET_TYPE_COUNT
};

struct LootCycleCount
{
	LootCycleCount();
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
		return "loot_cycle_count";
	}

	static const char* GetFormat()
	{
		return "uusuu";
	}

	uint32       id ;
	uint32       type ;
	std::string info ;
	uint32       count ;
	uint32       reset ;
};

#pragma pack(pop)

#endif // __STRUCT_LOOT_H__
