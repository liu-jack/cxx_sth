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
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BusinessSkillProto
struct BSkillProto
{
	BSkillProto();
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
		return "business_skill_proto";
	}
	static const char* GetFormat()
	{
		return 	"uuuuuuuu";
	}

    uint32 id;
    uint32 business_type;
    uint32 level_rank;
    uint32 level_min;
    uint32 level_max;
    uint32 level_exp_col;
    uint32 require_level;  // 需求人物等级
    uint32 require_money;  // 学习需要消耗货币
};

// BusinessSkillFormula

#ifdef	MYSQL_SUPPORT
  class CStringRow;
#endif

struct BSkillFormula
{
    enum{
        REQUIRE_METERIAL_MAX_NUM = 4,
    };

    enum LEARN_TYPE
    {
        LEARN_TYPE_FORMULA_ITEM = 0,
        LEARN_TYPE_LEVEL_UP,
        LEARN_TYPE_NPC,
    };

    BSkillFormula();
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
        return "business_skill_formula";
    }
    static const char* GetFormat()
    {
        return "uuuuuu"
            "uuuu"
            "uuuuuuuu"
            "uu";
    }

    bool operator!=(const BSkillFormula& p) const  //for  CItemAttrCache IsChanged() 
    {
        bool notequal = memcmp( this, &p, sizeof( BSkillFormula)) != 0;
        return notequal;				
    }

#ifdef	MYSQL_SUPPORT
    bool GetNameStringRow(CStringRow& names) const; 
#endif


    uint32 id;
    uint32 name_id;             // 名字id sting_item
    uint32 business_type;
    uint32 learn_type;
    uint32 learn_level;
    uint32 learn_money;

    uint32 product_id;          // 
    uint32 product_num_min;     // 
    uint32 product_num_max;     // 
    uint32 product_exp_base;

    struct RequireMeterial
    {
        uint32 item_id;
        uint32 item_count;
    };
    RequireMeterial require_meterial[REQUIRE_METERIAL_MAX_NUM];
    uint32 need_vitality;
    uint32 need_money;

};


#pragma pack(pop)