#pragma once
#include <string>
#include "struct_base.h"
#include <map>
#include "Base.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'295
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







//struct Aura_Runes
//{
//
//	Aura_Runes();
//	DECLARE_EXTRA_SIZE_ZERO
//	static const bool HasIndex()
//	{
//		return true;
//	}
//
//	const uint32 GetKey() const
//	{
//		return id;
//	}
//	static const char* GetKeyName()
//	{
//		return "id";
//	}
//	static const char* GetTableName()
//	{
//		return "aura_runes";
//	}
//	static const char* GetFormat()
//	{
//		return 
//			"u"//id
//			"uuss"//ui
//			"uuuu";//state;
//	}
//	uint32 id;
//
//	uint32 nameStrId;
//	uint32 descId;
//	std::string editName;
//	std::string icon;
//
//
//	enum	SKILL_VISUAL_STATES
//	{
//		STATE_ADD = 0,
//		STATE_BUFF,
//		STATE_REMOVE,
//		STATE_HIT,
//		//STATE_AOE,
//		STATE_COUNT
//	};
//	uint32	stateIndex[STATE_COUNT];
//
//	uint32 GetAuraId()
//	{
//		return id >> 2;
//	}
//};

//struct Skill_VisualPattern
//{
//	Skill_VisualPattern();
//	DECLARE_EXTRA_SIZE_ZERO
//	static const bool HasIndex()
//	{
//		return true;
//	}
//	const uint32 GetKey() const
//	{
//		return id;
//	}
//	static const char* GetKeyName()
//	{
//		return "id";
//	}
//	static const char* GetTableName()
//	{
//		return "skill_visual_pattern";
//	}
//	static const char* GetFormat()
//	{
//		return "uuuuuuuss";
//	}
//	uint32	id;
//	enum	SKILL_VISUAL_STATES
//	{
//		STATE_PRECAST = 0,
//		STATE_CASTING,
//		STATE_CHANNELLING,
//		STATE_HIT,
//		STATE_KEYF,
//		STATE_AOE,
//		STATE_COUNT
//	};
//	uint32	stateIndex[STATE_COUNT];
//
//	std::string editNameStr;
//	std::string descNameStr;
//};

struct CameraEffect
{
	enum ShakeDirection {
		SHAKE_RANDOM,
		SHAKE_LEFT,
		SHAKE_RIGHT,
		SHAKE_DOWN,
		SHAKE_UP,
		SHAKE_BACK,
		SHAKE_FRONT,
		SHAKE_SWING,
		NUM_SHAKE_DIRECTIONS,
	};

	uint32  cameraEffDirection;
	uint32 	cameraEffDuration;
	uint32  cameraEffCount;
	float   cameraEffAmplitude;
	uint32  cameraEffFrequency;
	uint32	cameraEffFadeInTime;
	uint32	cameraEffFadeOutTime;
	
	CameraEffect() { ZeroMemoryThis; }
};



//#define SKILL_EFFECT_GROUP_FLAG 0x80000000
//union EffectGroupFlags
//{
//	uint32 groupId;
//	struct
//	{
//		uint32 stateId : 26;
//		uint32 bindNode : 4;
//		uint32 isCaster : 1;
//		uint32 groupFlag : 1;
//
//	} groupFlag;
//};


struct Keyframe
{
	INIT_GAMEDATA(Keyframe)
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
		return "keyframe";
	}
	static const char* GetFormat()
	{
		return "usuuus";
	}

	uint32			id;
	std::string		path;

	uint32			keyframe;

	uint32			bladeStartFrame;
	uint32			bladeEndFrame;
	std::string		bladeEffectFile;
};







struct PlayerSetSkills
{
    INIT_GAMEDATA(PlayerSetSkills)
public:
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
        return "playerset_skills";
    }
    static const char* GetFormat()
    {
        return "uu" //id,index
            "uuu"
            "iiiiiii" //unlock:lv_skill,money1,item1,money2,item2,money3,item3
            ;
    }
    uint32 id;
    uint32 index_id;
    uint32 skill_id;
    uint32 skill_slot;
    uint32 skill_order;
    int unlock_lv_skill;
    int reqMoneyItemArr[6];
};

struct PlayerSetSpells
{
    INIT_GAMEDATA(PlayerSetSpells)
public:
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
        return "playerset_spells";
    }
    static const char* GetFormat()
    {
        return "uu" //id,index
            "uu"     //spell:id,order
            "ii"     //unlock:lv_spell,money_spell
            ;
    }
    uint32 id;
    uint32 index_id;
    uint32 spell_id;
    uint32 spell_order;
    int    unlock_lv_spell;
    int    unlock_money_spell;
};



#pragma pack(pop)