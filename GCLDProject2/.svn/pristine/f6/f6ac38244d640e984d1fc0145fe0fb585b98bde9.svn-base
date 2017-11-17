
#pragma once
#include <string>
#include <sstream>
#include "struct_base.h"
#include "struct_item_par.h"

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
bool                'c'
*/

#pragma pack(push,1)


#define CLASS_1_VAR(category) CLASS_1_##category
#define CLASS_2_VAR(category) CLASS_2_##category
#define CLASS_3_VAR(category) CLASS_3_##category

#define DEF_FIRST_CLASS(index,class1)\
    static const int CLASS_1_VAR(class1) = index;\
    bool Is##class1() const { return category_1 == index;}\

#define DEF_SECOND_CLASS(index, class1, class2 )\
    static const int CLASS_2_VAR(class2) = index;\
    bool Is##class2() const { return Is##class1() && category_2 == index;}\

#define DEF_THIRD_CLASS(index, class2, class3 )\
    static const int CLASS_3_VAR(class3) = index;\
    bool Is##class3() const { return Is##class2() && category_3 == index;}\


struct DB_ItemProtoType
{
    static const int CLASS_3_VAR(default) = 0;

    DEF_FIRST_CLASS( 0, Card );//del
        DEF_SECOND_CLASS( 0, Card, Mc);//del
        DEF_SECOND_CLASS( 1, Card, Npc);//del

	DEF_FIRST_CLASS( 1, Equip );
		DEF_SECOND_CLASS( 0, Equip, Weapon);
		DEF_SECOND_CLASS( 1, Equip, Horse);//Head
		DEF_SECOND_CLASS( 2, Equip, Chest);
		DEF_SECOND_CLASS( 3, Equip, Book);//Leg
		DEF_SECOND_CLASS( 4, Equip, Jade);//new
		DEF_SECOND_CLASS( 5, Equip, Flag);//new
		DEF_SECOND_CLASS( 6, Equip, Suit);//new
		DEF_SECOND_CLASS( 7, Equip, Decoration)//new 饰品
			DEF_THIRD_CLASS( 0, Suit, prisuit);//new
			DEF_THIRD_CLASS( 1, Suit, midsuit);//new
			DEF_THIRD_CLASS( 2, Suit, advsuit);//new
		DEF_SECOND_CLASS( 7, Equip, Acc);//acc


	//DEF_FIRST_CLASS( 2, Accessory );
	//	DEF_SECOND_CLASS( 0, Accessory, Holy);//Ring
	//	DEF_SECOND_CLASS( 1, Accessory, Necklace);//del

    //DEF_FIRST_CLASS( 3, Rune );
    //    DEF_SECOND_CLASS( 0, Rune, Rune01);//del
    //    DEF_SECOND_CLASS( 1, Rune, Rune02);//del
    //    DEF_SECOND_CLASS( 2, Rune, Rune03);//del
    //    DEF_SECOND_CLASS( 3, Rune, Rune04);//del
    //    DEF_SECOND_CLASS( 4, Rune, Rune05); //del


	DEF_FIRST_CLASS( 3, Attr );
		DEF_SECOND_CLASS( 0, Attr, Troops);
		DEF_SECOND_CLASS( 1, Attr, Attack);
		DEF_SECOND_CLASS( 2, Attr, Defense);
		DEF_SECOND_CLASS( 3, Attr, Puncture);
		DEF_SECOND_CLASS( 4, Attr, Parry);
		DEF_SECOND_CLASS( 5, Attr, BrokenArray);
		DEF_SECOND_CLASS( 6, Attr, Impeccable);
		DEF_SECOND_CLASS( 7, Attr, Tong);
		DEF_SECOND_CLASS( 8, Attr, Yong);
		DEF_SECOND_CLASS( 9, Attr, ArmyExp);
		

	//DEF_FIRST_CLASS( 4, Gem );
	//	DEF_SECOND_CLASS( 0, Gem, RedGem);
	//	DEF_SECOND_CLASS( 1, Gem, GreenGem);
	//	DEF_SECOND_CLASS( 2, Gem, BlueGem);
	//	DEF_SECOND_CLASS( 3, Gem, YelloGem);
	//	DEF_SECOND_CLASS( 4, Gem, CyanGem);//del
	//	DEF_SECOND_CLASS( 5, Gem, PurpleGem);//del

		DEF_FIRST_CLASS( 4, Gem );
			DEF_SECOND_CLASS( 0, Gem, Jewel);
			DEF_SECOND_CLASS( 1, Gem, Spar);
			DEF_SECOND_CLASS( 2, Gem, SkillStone);
				DEF_THIRD_CLASS( 0, SkillStone, KillStone);//new
				DEF_THIRD_CLASS( 1, SkillStone, StickStone);//new
				DEF_THIRD_CLASS( 2, SkillStone, CriticalStone);//new
				DEF_THIRD_CLASS( 3, SkillStone, PeerlessStone);//new
				DEF_THIRD_CLASS( 4, SkillStone, FlurryStone);//new
				DEF_THIRD_CLASS( 5, SkillStone, AssaultStone);//new
			DEF_SECOND_CLASS( 3, Gem, GodSkillStone);
				DEF_THIRD_CLASS( 0, GodSkillStone, GodKillStone);//new
				DEF_THIRD_CLASS( 1, GodSkillStone, GodStickStone);//new
				DEF_THIRD_CLASS( 2, GodSkillStone, GodCriticalStone);//new
				DEF_THIRD_CLASS( 3, GodSkillStone, GodPeerlessStone);//new
				DEF_THIRD_CLASS( 4, GodSkillStone, GodFlurryStone);//new
				DEF_THIRD_CLASS( 5, GodSkillStone, GodAssaultStone);//new

    DEF_FIRST_CLASS( 5, Prop );
        DEF_SECOND_CLASS( 0, Prop, Currency);
            DEF_THIRD_CLASS( 0, Currency, Coin);//CurrCoin
            DEF_THIRD_CLASS( 1, Currency, SysGold);//CurrCrystal
            DEF_THIRD_CLASS( 2, Currency, Iron);//CurrHornor
            DEF_THIRD_CLASS( 3, Currency, Food);//CurrSoul
            DEF_THIRD_CLASS( 4, Currency, Wood);//CurrSpar
            DEF_THIRD_CLASS( 5, Currency, Hornor);//CurrBounty
            DEF_THIRD_CLASS( 6, Currency, BuildHornor);//CurrEssence
			DEF_THIRD_CLASS( 7, Currency, Training); 
			DEF_THIRD_CLASS( 8, Currency, TroopsNum);
			DEF_THIRD_CLASS( 9, Currency, FreeCollectNum);
			DEF_THIRD_CLASS( 10, Currency, SuperCollectNum);
			DEF_THIRD_CLASS( 11, Currency, TroopsOder);
			DEF_THIRD_CLASS( 12, Currency, GoldHammer);
			DEF_THIRD_CLASS( 13, Currency, Coupon);
			DEF_THIRD_CLASS( 14, Currency, MarketBuyNum);
			DEF_THIRD_CLASS( 15, Currency, FreePurify);
			DEF_THIRD_CLASS( 16, Currency, ExtrmePurify);
			DEF_THIRD_CLASS( 17, Currency, GodStonePurify);
			DEF_THIRD_CLASS( 18, Currency, CashGold);
			DEF_THIRD_CLASS( 19, Currency, PlayerExp);//new
			DEF_THIRD_CLASS( 20, Currency, VipExp);//new
			DEF_THIRD_CLASS( 21, Currency, DailyScore);//new
			DEF_THIRD_CLASS( 22, Currency, TrainingFight);
			DEF_THIRD_CLASS( 23, Currency, TrainingDouble);
			DEF_THIRD_CLASS( 24, Currency, TrainingReduce);
			DEF_THIRD_CLASS( 25, Currency, PeopleLoyal);
			DEF_THIRD_CLASS( 26, Currency, ZhengWu);
			DEF_THIRD_CLASS( 27, Currency, Renown);
			DEF_THIRD_CLASS( 28, Currency, StarCrystal);
			DEF_THIRD_CLASS( 29, Currency, Board);//new
			DEF_THIRD_CLASS( 30, Currency, GeneCode);//new
			DEF_THIRD_CLASS( 31, Currency, Beef);//new
			DEF_THIRD_CLASS( 32, Currency, Wine);//new
            static const int CLASS_3_VAR(CurrencyTypeCount) = CLASS_3_VAR( Wine) + 1;


        DEF_SECOND_CLASS( 1, Prop, Material);//del
            DEF_THIRD_CLASS( 0, Material, MatCardLV);//del
            DEF_THIRD_CLASS( 1, Material, MatCardQuality);//del
            DEF_THIRD_CLASS( 2, Material, MatCardReinforce);//del
            DEF_THIRD_CLASS( 3, Material, MatCardSkill);//del
            DEF_THIRD_CLASS( 4, Material, MatCardAttr);//del
            DEF_THIRD_CLASS( 5, Material, MatEquipLV);//del
            DEF_THIRD_CLASS( 6, Material, MatEquipReinforce);//del
            DEF_THIRD_CLASS( 7, Material, MatAccLV);//del
            DEF_THIRD_CLASS( 8, Material, MatAccReinforce);//del

        DEF_SECOND_CLASS( 2, Prop, Puzzle);//del
            DEF_THIRD_CLASS( 0, Puzzle, PuzzleCard);//del
            DEF_THIRD_CLASS( 1, Puzzle, PuzzleEquip);//del
            DEF_THIRD_CLASS( 2, Puzzle, PuzzleAcc);//del
            DEF_THIRD_CLASS( 3, Puzzle, PuzzleRune);//del

        DEF_SECOND_CLASS( 3, Prop, Voucher);
            DEF_THIRD_CLASS( 0, Voucher, Purify);//VoucherLottery
            DEF_THIRD_CLASS( 1, Voucher, CollectIron);//VoucherShop
            DEF_THIRD_CLASS( 2, Voucher, FarmOrder);//VoucherPvP
			DEF_THIRD_CLASS( 3, Voucher, DwellingsOrder);
			DEF_THIRD_CLASS( 4, Voucher, WoodOrder);
			DEF_THIRD_CLASS( 5, Voucher, CampOrder);
			DEF_THIRD_CLASS( 6, Voucher, Darksteel);
			DEF_THIRD_CLASS( 7, Voucher, InvestProof);
			DEF_THIRD_CLASS( 8, Voucher, PushExp);
			DEF_THIRD_CLASS( 9, Voucher, GemVouchers);
			DEF_THIRD_CLASS( 10, Voucher, PitPaper);
			DEF_THIRD_CLASS( 11, Voucher, SuitScrap);
			DEF_THIRD_CLASS( 12, Voucher, SmithyPaper);
			DEF_THIRD_CLASS( 13, Voucher, EvolveScroll);
			DEF_THIRD_CLASS( 14, Voucher, PropSuipian);
			DEF_THIRD_CLASS( 15, Voucher, IronTenPunk);
			DEF_THIRD_CLASS( 16, Voucher, ShenQiPunk);
			//DEF_THIRD_CLASS( 17, Voucher, ShenQiBuff);
			DEF_THIRD_CLASS( 18, Voucher, ActiveChar);
			DEF_THIRD_CLASS( 19, Voucher, CharLevelUpCost);

        DEF_SECOND_CLASS( 4, Prop, Box); //del
            DEF_THIRD_CLASS( 0, Box, BoxImm);//del
            DEF_THIRD_CLASS( 1, Box, BoxMed);//del

        //DEF_SECOND_CLASS( 5, Prop, Misc);//del
        //    DEF_THIRD_CLASS( 0, Misc, AP);//del
        //    DEF_THIRD_CLASS( 1, Misc, SP);//del



    INIT_GAMEDATA(DB_ItemProtoType);

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
        return "item_proto";
    }

    //struct format
    static const char* GetFormat()
    {
        return "usuus"	//id			
            "uuu"		//category_1		
            "uuu"		//special_flag		
            "uiuuuu"    //require_level
            "uu"    //attr_flag_base    
            "uuuu"
            "uuuuuu"
			"uuuuuuusu";
    }

    uint32 id;
    string comment;
    uint32 name_id;
    uint32 description_id;
    std::string icon;

    uint32 category_1;
    uint32 category_2;
    uint32 category_3;
    
    uint32 special_flag; // enum E_SPECAIL_FLAG{}
    uint32 quality_option; // enum E_QUALITY_TYPE {}
    uint32 bonding_option; // enum E_BONDING_TYPE {}
        
    uint32      require_level;
    int32      require_char;    // enum CHAR_CLASS
    uint32      require_attr_type; // enum E_REQUIRE_ATTR_TYPE{}
    uint32      require_attr_value;
    uint32      require_item_id;    // 
    uint32      require_item_num;   //

    
    uint32 attr_flag_base;
    uint32 attr_flag_reinforce;
    
    uint32 item_random_socket_id;
    uint32 item_set_id;
    uint32 object_id;
    uint32 skill_id;

    uint32 use_count;
    uint32 cooldown_category;
    uint32 cooldown_time; // enum E_EXPIRED_TYPE{}
    uint32 expired_type;
    uint32 expired_time;
    uint32 filter_id;

	uint32 value01; //可变参数
	uint32 value02;
	uint32 value03;
	uint32 is_use;
	uint32 is_all_use; //批量使用
	uint32 is_sell;
	uint32 sell_price;
	std::string quality_image_name;
	uint32 tips_id;
};

struct ItemAttr
{
    INIT_GAMEDATA(ItemAttr);	 
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
        return "uuu"
            "ffffffffffff";
    }

    uint32 id;
    uint32 equip_type;
    uint32 quality;

    float value[12];
};

struct ItemAttrBase : public ItemAttr
{
    INIT_GAMEDATA(ItemAttrBase);	 
    static const char* GetTableName()
    {
        return "item_attr_base";
    }
};

struct ItemAttrAdd : public ItemAttr
{
    INIT_GAMEDATA(ItemAttrAdd);	 
    static const char* GetTableName()
    {
        return "item_attr_add";
    }
};


struct ItemEquipXiLian
{
public:
	INIT_GAMEDATA(ItemEquipXiLian); 
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
		return "equip_xilian";
	}

	static const char* GetFormat()
	{
		return "uuuuuuuu";
	}

public:
	uint32 id;
	uint32 troops;    
	uint32 attack;
	uint32 defense;
	uint32 chuanci;
	uint32 gedang;
	uint32 pozhen;
	uint32 wuxie;
};


struct ItemSuitComplex
{
public:
	INIT_GAMEDATA(ItemSuitComplex); 
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
		return "suit_complex";
	}

	static const char* GetFormat()
	{
		return "uusuuuuuuuuuuuuuuuuu";
	}

public:
	uint32 id;
	uint32 item_id;
	string comment;
	uint32 need_paper_id;
	uint32 paper_sell_id;
	uint32 need_paper_num;
	uint32 creat_req_level;
	uint32 show_req_level;
	uint32 req_equip[6];
	uint32 req_attr[6];
};

struct ItemBagSoltSell
{
public:
	INIT_GAMEDATA(ItemBagSoltSell); 
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
		return "buy_bag";
	}

	static const char* GetFormat()
	{
		return "uuu";
	}

public:
	uint32 id;
	uint32 need_item;
	uint32 need_num;
};

#pragma pack(pop)

