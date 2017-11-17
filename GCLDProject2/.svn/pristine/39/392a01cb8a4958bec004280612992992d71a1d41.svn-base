
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



struct SpellCreation
{
	const static uint32 MAX_ITEM_COST_KIND = 6;
	const static uint32 MAX_ITEM_CREATE_KIND = 6;
	const static uint32 MAX_COST_SPECIAL_CURRENCY_KIND = 5;
	const static uint32 MAX_SPECIAL_CURRENCY_KIND = 5;

	SpellCreation();
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
		return "spell_creation";
	}

	static const char* GetFormat()
	{
		const char* spellentryFormat =
			"u" // Id
			"u" // itemCost1
			"u" // costNum1
			"u" // itemCost2
			"u" // costNum2
			"u" // itemCost3
			"u" // costNum3
			"u" // itemCost4
			"u" // costNum4
			"u" // itemCost5
			"u" // costNum5
			"u" // itemCost6
			"u" // costNum6
			"u" //costspec cur1
			"u" 
			"u" //costspec cur2
			"u"
			"u" //costspec cur3
			"u"
			"u" //costspec cur4
			"u"
			"u" //costspec cur5
			"u"
			"u" // itemCreate1
			"u" // createNum1
			"u" // createPer1
			"u" // itemCreate2
			"u" // createNum2
			"u" // createPer2
			"u" // itemCreate3
			"u" // createNum3
			"u" // createPer3
			"u" // itemCreate4
			"u" // createNum4
			"u" // createPer4
			"u" // itemCreate5
			"u" // createNum5
			"u" // createPer5
			"u" // itemCreate6
			"u" // createNum6
			"u" // createPer6
			"u" //spec cur1
			"u"
			"u"
			"u" //spec cur2
			"u"
			"u"
			"u" //spec cur3
			"u"
			"u"
			"u" //spec cur4
			"u"
			"u"
			"u" //spec cur5
			"u"
			"u"
			;
		return 	spellentryFormat;
	}

	uint32 Id;
	struct ItemCost
	{
		uint32 itemId;
		uint32 costNum;
	};
	ItemCost itemCost[MAX_ITEM_COST_KIND];

	struct CostSpecCur
	{
		uint32 currencyId;
		uint32 currencyCost;
	};
	CostSpecCur costSpecCur[MAX_COST_SPECIAL_CURRENCY_KIND];

	struct ItemCreate
	{
		uint32 itemId;
		uint32 createNum;
		uint32 createPer;
	};
	ItemCreate itemCreate[MAX_ITEM_CREATE_KIND];

	struct SpecCur
	{
		uint32 currencyId;
		uint32 currencyNum;
		uint32 currencyRequireItemID;	// it is a itemID
	};
	SpecCur specCur[MAX_SPECIAL_CURRENCY_KIND];
};

struct Spell_Effect
{
	Spell_Effect();
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
		return "spell_effect";
	}
	static const char* GetFormat()
	{
		return "uuuuufuuiiiuuuuuuuuuusss";
	}
	uint32 Id;
	uint32 spell;
	uint32 reserve_duration;
	uint32 reserve_frequency;
	uint32 affectType;
	float  affectRadius;
	enum EFFECT_TYPE{
		eImmediately,
		eAddAura,
	};
	uint32 type;
	uint32 subType;
	int32 school;
	int32 baseValue;
	int32 randomValue;
	uint32 miscValue;
	uint32 requireTargetAura;
	uint32 requireTargetAuraStackCount;
	uint32 requireCasterAura;
	uint32 requireCasterAuraStackCount;
	uint32 requireTargetAuraState;
	uint32 requireCasterAuraState;
	uint32 requireTargetHPPercent;
	uint32 requireCasterHPPercent;
	enum TriggerByAura
	{																	
		ON_MELEE_ATTACK						= 0x1,						//OK
		ON_MELEE_ATTACK_VICTIM				= 0x2,						//OK
		ON_CRIT_ATTACK						= 0x4,						//OK
		ON_CRIT_HIT_VICTIM					= 0x8,						//OK
		ON_DODGE							= 0x10,						//OK
		ON_DODGE_VICTIM						= 0x20,						//OK
		ON_PARRY							= 0x40,						//OK
		ON_PARRY_VICTIM						= 0x80,						//OK
		ON_RANGED_ATTACK					= 0x100,					//UNDONE
		ON_RANGED_ATTACK_VICTIM				= 0x200,					//UNDONE
		ON_RANGED_CRIT_ATTACK				= 0x400,					//UNDONE
		ON_RANGED_CRIT_ATTACK_VICTIM		= 0x800,					//UNDONE
		ON_BLOCK							= 0x1000,					//50%
		ON_BLOCK_VICTIM						= 0x2000,					//50%
		ON_ANY_DAMAGE_VICTIM				= 0x4000,					//OK
		ON_SPELL_HIT						= 0x8000,					//OK
		ON_SPELL_HIT_VICTIM					= 0x10000,					//OK
		ON_SPELL_CRIT_HIT					= 0x20000,					//OK
		ON_SPELL_CRIT_HIT_VICTIM			= 0x40000,					//OK
		ON_SPELL_RESIST						= 0x80000,					//OK
		ON_SPELL_RESIST_VICTIM				= 0x100000,					//OK
		ON_GAIN_EXPIERIENCE					= 0x200000,					//UNDONE
		ON_AURA_INTERRUPTED					= 0x400000,					//UNDONE
		ON_DISPELL_AURA						= 0x800000,					//UNDONE
		ON_DIE								= 0x1000000,				//UNDONE
		ON_TARGET_DIE						= 0x2000000,				//UNDONE
		REMOVEONUSE							= 0x4000000,				//OK
		TARGET_SELF							= 0x8000000,				//OK
		ON_SPELL_CAST						= 0x10000000,
	};															
	uint32 auraTriggerOccasion;									
	std::string typeName;										
	std::string subTypeName;					
	std::string schoolName;						
};												

// Struct for the entry in Spell.dbc/Spell

struct SpellPrototype
{
	const static uint32 MAX_EFFECT_IN_SPELL = 6;
	const static uint32 SPELL_GROUP_SIZE	= 32;
	const static uint32 SPELL_GROUP_U32_LEN = 5;
	const static uint32 SPELL_GROUP_COUNT	= SPELL_GROUP_U32_LEN*SPELL_GROUP_SIZE;
	SpellPrototype();
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
		return "spell_new";
	}

	static const char* GetFormat()
	{
		const char* spellentryFormat =
			"u" // Id
			"u" // SkillSet
			"u" // filter
			"s" // editName
			"u" // visualID
			"u" // nameStringID
			"u" // rankStringID
			"u" // descStringID
			"u" // minLevel
			"u" // maxLevel
			"s" // icon
			"u" // requireTool
			"i" // requireEquipSlot
			"u" // requireEquipClass
			"u" // requireEquipSubClass
			"u" // hasCreation
			"u" // castingTime
			"u" // recoveryTime
			"f" // minFriendRange
			"f" // maxFriendRange
			"f" // minHostileRange
			"f" // maxHostileRange
			"u" // interruptCastingType
			"u" // interruptChannelType
			"u" // interruptAuraType
			"u" // interruptMapType
			"u" // spellVisualID
			"u" // recoveryTimeCategory
			"u" // startRecoveryTime
			"u" // startRecoveryCategory
			"u" // targetType
			"u" // duration
			"u" // frequency
			"u" // effect1
			"u" // effect2
			"u" // effect3
			"u" // effect4
			"u" // effect5
			"u" // effect6
			"f" // speed
			"u"	// powerType
			"u" // power cost
			"u" // facing flag
			"u" // attribute
			"uuuuu" // grouptype
			"uuu"	//castScript
			"u"		// max stack
			"u"		// power require
			"u" // requireAreaId
			;
		return 	spellentryFormat;
	}

	uint32 Id;
	uint32 skillset;
	uint32 filter;
	std::string  editName;
	uint32 visualID;
	uint32 nameStringID;
	uint32 rank;
	uint32 descStringID;
	uint32 minLevel;
	uint32 maxLevel;
	std::string  icon;
	uint32 requireTool;
	int32  requireEquipSlot;
	uint32 requireEquipClass;
	uint32 requireEquipSubClassMask;
	uint32 hasCreation;
	uint32 castingTime;
	uint32 recoveryTime;
	float minFriendRange;
	float maxFriendRange;
	float minHostileRange;
	float maxHostileRange;
	uint32 interruptCastingType;
	uint32 interruptChannelType;
	uint32 interruptAuraType;
	uint32 interruptMapType;
	uint32 spellVisualID;
	uint32 recoveryTimeCategory;
	uint32 startRecoveryTime;
	uint32 levelCategory;
	uint32 targetType;
	uint32 duration;
	uint32 amplitude;
	uint32 effect[MAX_EFFECT_IN_SPELL];
	//uint32 effect2;
	//uint32 effect3;
	//uint32 effect4;
	//uint32 effect5;
	//uint32 effect6;
	float speed;
	uint32 powerType;
	uint32 powerCost;
	uint32 casterFacingFlags;
	uint32 attribute[1];
	uint32 spellGroup[SPELL_GROUP_U32_LEN];
	uint32 castScript[3];
	uint32 maxStack;
	uint32 powerRequire;
	uint32 requireAreaId;
	// this marks the end of the fields loaded from db - don't remove the comment please 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//next things are runtime.

	const SpellCreation*	pCreation;
	const Spell_Effect*	EffectPtrs[MAX_EFFECT_IN_SPELL];



	uint32		spellCastType;	
	bool MatchGroup(const SpellPrototype* pProto) const
	{
		for( uint32 iGroupMask = 0; iGroupMask < SPELL_GROUP_U32_LEN; iGroupMask ++ )
		{
			if(  spellGroup[iGroupMask] & pProto->spellGroup[iGroupMask] )
			{
				return true;
			}
		}
		return false;
	}

	bool hasAttribute(uint32 attr) const
	{
		return (attribute[0] & attr) > 0;
	}
};



// Struct for the entry in Spell.dbc/Spell
struct SpellToolFilter
{
	SpellToolFilter();
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
		return "spell_tool_filter";
	}

	static const char* GetFormat()
	{
		const char* spellentryFormat =
			"u" // Id
			"u" // tooltype
			"u" // item
			;
		return 	spellentryFormat;
	}

	uint32 Id;
	uint32 tooltype;
	uint32 item;
};


STRING_DB_STRUCT(String_Spell)
//STRING_DB_STRUCT(String_Talent)

//struct String_Spell 
//{
//	INIT_GAMEDATA(String_Spell)
//	static const bool HasIndex()
//	{
//		return true;
//	}
//	const uint32 GetKey() const
//	{
//		return Id;
//	}
//	static const char* GetKeyName()
//	{
//		return "Id";
//	}
//	static const char* GetTableName()
//	{
//		return "string_spell";
//	}
//	static const char* GetFormat()
//	{
//		return "ussssssssss";
//	}
//	uint32 Id;
//	std::string str[LANG_MAX];
//
//};



struct SpellVisual
{
	SpellVisual();
	DECLARE_EXTRA_SIZE_ZERO
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return SpellVisualID;
	}
	static const char* GetKeyName()
	{
		return "SpellVisualID";
	}
	static const char* GetTableName()
	{
		return "c_spellvisual";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuuss";
	}
	uint32	SpellVisualID;
	enum	SPELL_VISUAL_STATES
	{
		STATE_PRECAST = 0,
		STATE_CASTING,
		STATE_CHANNELLING,
		STATE_HIT,
		STATE_AURA,
		STATE_AOE,
		STATE_REMOVE,
		STATE_COUNT
	};
	uint32	state_index[STATE_COUNT];
	uint32 	missile_effect;
	uint32  stripe_color;
	uint32	hit_delay;
	std::string editNameStr;
	std::string descNameStr;
};

struct SpellVisualState
{
	SpellVisualState();
	DECLARE_EXTRA_SIZE_ZERO
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return visual_state_id;
	}
	static const char* GetKeyName()
	{
		return "visual_state_id";
	}
	static const char* GetTableName()
	{
		return "c_spellvisualstate";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuus";
	}
	uint32	visual_state_id;
	enum SPELL_CASTER_ANIM
	{
		CASTER_ANIM_NONE = 0,
		CASTER_ANIM_SPELL_TO_TARGET ,
		CASTER_ANIM_SPELL_NO_TARGET,
		CASTER_ANIM_MELEE,
		CASTER_ANIM_MELEE01,
		CASTER_ANIM_MELEE_CRITICAL,
		CASTER_ANIM_MELEE_SPECIAL,
		CASTER_ANIM_HURT,
		CASTER_ANIM_MELEE_SPECIAL_SKILL_01,
		CASTER_ANIM_MELEE_SPECIAL_SPELL_02,
		CASTER_ANIM_MINING	=	10,
		CASTER_ANIM_HERB,
		CASTER_ANIM_POTION_USING,
		CASTER_ANIM_RECOVERING,
	};
	uint32	caster_anim;
	enum SPELL_STATE_EFFECTNODE
	{
		EFFECT_OVER_HEAD = 0,
		EFFECT_CHEST,
		EFFECT_GROUND,
		EFFECT_RHAND,
		EFFECT_LHAND,
		EFFECT_AOE,
		EFFECT_COUNT

	};
	uint32  effect_index[EFFECT_COUNT];

	uint32 	sound;
	uint32 	camera_shakes;
	enum SPELL_STATE_ANIM_COMBINATION
	{
		COMBINATION_NONE = 0,// this animation has no combination and can be combined by other upper animation, like running
		COMBINATION_UPPER,	// combine to current animation upper body
		COMBINATION_ALL_BODY,// no combination and prevent being comined by other upper animation
	};
	uint32  anim_combination;
	std::string desc;

};



struct SpellVisualEffect
{
	SpellVisualEffect();
	DECLARE_EXTRA_SIZE_ZERO
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return effect_id;
	}
	static const char* GetKeyName()
	{
		return "effect_id";
	}
	static const char* GetTableName()
	{
		return "c_spellvisualeffect";
	}
	static const char* GetFormat()
	{
		return "ussiiiuuuu";
	}
	uint32	effect_id;
	std::string name;
	std::string pathname;
	int32	x_stretch;
	int32	y_stretch;
	int32	z_stretch;
	uint32	scale;
	enum SPELL_EFFECT_TYPE
	{
		TYPE_BIND_TO_NODE = 0,
		TYPE_FOLLOW_FACE_DIR,
		TYPE_HIT,
		TYPE_SET_NODE_POS,
		TYPE_BIND_TO_RHAND_WEAPON,
		TYPE_BIND_TO_LHAND_WEAPON,
	};
	uint32 type;
	enum SPELL_MISSILE_EFFECT_TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_FAST_DRAIN_SOUL,
		TYPE_SLOW_DRAIN_SOUL,
		MISSILE_EFFECT_TYPE_NUM,
	};
	uint32 missile_type;
	uint32 missile_num;
};


struct Spell_Filter
{
	Spell_Filter();
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
		return "spellfilter";
	}
	static const char* GetFormat()
	{
		return "usu";
	}
	uint32 Id;
	std::string filterName;
	uint32 filterParent;

};

struct Spell_Imm_Sub
{
	Spell_Imm_Sub();
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
		return "spell_imm_sub";
	}
	static const char* GetFormat()
	{
		return "us";
	}
	uint32 Id;
	std::string name;
};

struct Spell_Aura_Sub
{
	Spell_Aura_Sub();
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
		return "spell_aura_sub";
	}
	static const char* GetFormat()
	{
		return "us";
	}
	uint32 Id;
	std::string name;
};

struct Spell_Affect_Type
{
	Spell_Affect_Type();
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
		return "spell_affect_type";
	}
	static const char* GetFormat()
	{
		return "us";
	}
	uint32 Id;
	std::string name;
};


struct Spell_Talent
{
	const static uint32 MAX_TALENT_RANK = 5;
	const static uint32 MAX_TALENT_SPELL = 5;
	const static uint32 MAX_REQ_TALENT  = 3;
	INIT_GAMEDATA(Spell_Talent)
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
		return "spell_talent";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuusuuuuuuu";
	}
	uint32 Id;
	uint32 iRow;
	uint32 iCol;
	uint32 iSkillset;
	uint32 nRequireCategoryTalentCount;
	uint32 iRequireRefTalent[MAX_REQ_TALENT];
	uint32 nMaxRank;
	uint32 iRefSpellIDLearn[MAX_TALENT_RANK][MAX_TALENT_SPELL];
	std::string strSpellIconName;
	uint32 iNameStrID;
	uint32 iDescStrID[5];
	uint32 iSlot;
};

struct Spell_Group
{
	Spell_Group();
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
		return "spell_group";
	}
	static const char* GetFormat()
	{
		return "uss";
	}
	uint32 Id;
	std::string name;
	std::string desc;
};


struct Spell_Skillset
{
	INIT_GAMEDATA(Spell_Skillset)
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
		return "spell_skillset";
	}
	static const char* GetFormat()
	{
		return "uus";
	}
	
	uint32 Id;
	uint32 refStrNameID;
	string icon;

	// this marks the end of the fields loaded from db - don't remove the comment please 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//next things are runtime.
	string strName;
};

#pragma pack(pop)