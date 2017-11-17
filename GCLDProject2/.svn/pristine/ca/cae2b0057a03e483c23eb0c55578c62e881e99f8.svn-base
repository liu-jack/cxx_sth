#ifndef __STRUCT_QUEST_H__
#define __STRUCT_QUEST_H__

#pragma once
//#include <string>
//#include <set>
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


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Item Table
struct DB_QuestProto
{
	INIT_GAMEDATA(DB_QuestProto)

    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return Id; }
    static const char* GetKeyName(){ return "Id"; }
    static const char* GetTableName(){ return "quest_proto"; }
	static const char* GetFormat() {
		return 
			"uuuuuuuuuuuuuuuu";
	}

    enum QUEST_TYPE{
        QUEST_TYPE_MAIN         = 0,
        QUEST_TYPE_DAILY        = 1,
        QUEST_TYPE_ACHIEVEMENT  = 2,
        QUEST_TYPE_ACHIEVEMENT_DAILY = 3,
        QUEST_TYPE_VIP          = 4,
        QUEST_TYPE_VIP_DAILY    = 5,
        QUEST_TYPE_VIP_ACHIEVEMENT = 6,
        QUEST_TYPE_BRANCH       = 7,
        QUEST_TYPE_COUNT
    };

    bool IsRepeatable() const;
    bool IsSequentQuest() const; // 任务链型的任务
	bool IsQuestBranch() const;
	bool IsMainQuest() const;

	uint32 Id;
	uint32 name_id;
	uint32 description_id;
	uint32 filter_id;
	uint32 special_flag;
	uint32 quest_type;
    uint32 quest_unlock_lv;
    uint32 quest_unlock_vip;

	uint32 quest_next;
	uint32 objective_category_1;
	uint32 objective_category_2;
	uint32 objective_category_3;
	uint32 objective_condition_1;
	uint32 objective_value;
	uint32 skip_id;
	uint32 trigger_id;
};
struct DB_QuestReward
{
	INIT_GAMEDATA(DB_QuestReward)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return Id; }
    void SetKey(uint32 _id){ Id = _id; }
    static const char* GetKeyName(){ return "id"; }
    static const char* GetTableName(){ return "quest_reward"; }
    static const char* GetFormat(){ return "uuuuuss"; }

	uint32	Id;
	uint32	QuestId;
	uint32	reward_category1;
	uint32  reward_category2;
	uint32  reward_count;
	string  reward_trigger;
	string  reward_equip_id;
}; 

/*
struct QuestPrototype
{
	const static  int MAX_CHOICE_REWARD_SLOT =  6;
	const static  int MAX_QUEST_REQUIRE_TYPE_NUM  = 4;
	const static  int MAX_QUEST_REQUIRE_OBJECT_NUM  = 4;
	const static  int MAX_QUEST_ZONE  = 4;
	const static  int MAX_QUEST_RECEIVE_NUM = 4;
    enum RepeatType
    {
        RepeatType_NotRepeat = 0,
        RepeatType_Repeat = 1,
        RepeatType_Daily = 2,
        RepeatType_Weekly = 3,
    };
	INIT_GAMEDATA(QuestPrototype)
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
		return "quests";
	}
	static const char* GetFormat()
	{
		return 
            "uuuuu" // Id
            "uuu"   // repeat_type
            "uuuu"  // required_races
            "uuu"   // limit_time
            "uuuuu" // titleStrID
            "uuuu"  // incompleteTextStrID
            "uuuii" // first_reward_money_rate
            "uuuuuu"// reward_token_data
            "sss"
            "usuuu" // suggestedplayers
            "uuuu"  // StartGiverType
            "uu"    // ClearType
			;
	}

    bool isParentQuest() const          { return (special_flags & flagParentQuest) != 0; }
    bool isParentQuestListRepeat() const{ return (special_flags & flagParentQuestListRepeat) != 0; }
    bool isRequirePreQuestFail() const  { return (special_flags & flagRequirePreQuestFail) != 0;  }
    bool isHaveMoreTime() const         { return (special_flags & flagHaveMoreTime) != 0;  }
    bool haveMoneyReward() const        { return (special_flags & flagNotHaveMoneyReward) == 0;  }
    bool haveExpReward() const          { return (special_flags & flagNotHaveExpReward) == 0; }
    bool runOnAccQuestWhenLogin() const { return (special_flags & flagRunOnAccQuestWhenLogin) != 0; }
    bool isRepeatable() const           { return repeat_type != RepeatType_NotRepeat; }


	uint32 Id;
	uint32 map_id;					// not used in server yet
	uint32 min_level;				//
	uint32 quest_level;				// 任务等级
	uint32 quest_type;

    uint32 repeat_type;                 // 重复任务类型
    uint32 repeat_count_limit;          // 重复任务在时间周期内限制完成的次数
    uint32 repeat_interval;             // 重复任务的可接取时间间隔  ，单位：分钟    

	uint32 required_races;							// 需求种族
	uint32 required_class;							// 需求职业
    uint32 required_currency_type;
    uint32 required_currency_count;

	uint32 limit_time;								// 完成任务时间限制，单位：分钟
    uint32 when_to_show_limit_time;                 // 什么时候开始提示玩家剩余时间， 单位分钟
	enum SpecialFlags
	{
		flagParentQuest             = 0x0001,       // 是否为父任务
		flagParentQuestListRepeat   = 0x0002,		// 
		flagRequirePreQuestFail 	= 0x0004,		// 需要前置任务失败， 区共用
		flagHaveMoreTime            = 0x0008,       // 区共任务结算时间
        flagNotHaveMoneyReward      = 0x0010,       // 是否有金钱奖励
        flagNotHaveExpReward        = 0x0020,       // 是否有经验奖励
        flagRunOnAccQuestWhenLogin  = 0x0040,       // 
	};
	uint32 special_flags;


	uint32 titleStrID;					// 文本
	uint32 detailsStrID;				// 文本
	uint32 objectivesStrID;				// 文本
	uint32 logtextStrID;				// 文本
	uint32 completionTextStrID;			// 文本

	uint32 incompleteTextStrID;			// 文本
	uint32 endTxtID;					// 文本
    uint32 AcceptTextID;                // 文本
    uint32 CommitTextID;                // 文本

	uint32 first_reward_money_rate;	    // 首次 金钱 奖励百分比
	uint32 first_reward_xp_rate;		// 首次 经验 奖励百分比

    uint32 reward_reputation_type;      // 声望奖励类型
    int32 reward_reputation_value;      // 声望奖励数值
    int32 first_reward_reputation_extra;// 首次完成额外奖励数值 

    enum{ REWARD_TOKEN_MAX = 2};
    
    struct RewardToken
    {
        uint32 reward_token_type;        // 奖励代币类型
        uint32 reward_token_value;       // 奖励代币数值
        uint32 first_reward_token_extra;  // 首次完成额外奖励数值
    };
    
    RewardToken reward_token_data[REWARD_TOKEN_MAX];  

	std::string optional_pre_quests;
	std::string require_quests;
	std::string conflic_quests;
	
	uint32 suggestedplayers;			// not used in server yet, but in GS2C_QuestDetail
	std::string script; 
	uint32 AutoShowAccept;
	uint32 AutoShowFinish;              // 自动弹出交任务界面。

	uint32 FilterId;					// only used in quest editor

	enum GiverType
	{
        GiverType_none = 0,
		GiverType_Creature  = 1,
		GiverType_StaticObj = 2, 
		GiverType_AreaTrigger = 3, 
		GiverType_AreaGroupTrigger = 4,
	};

	uint32 StartGiverType;
	uint32 StartGiverId;
	uint32 FinishGiverType;
	uint32 FinishGiverId;

    enum CLEAR_TYPE
    {
        ClearType_NotSave = 0,
        ClearType_ClearRelate = 1,
        ClearType_None = 2,
    };
    enum FinishType
    {
        FinishType_Success = 0,
        FinishType_Fail = 1,
        FinishType_Count = 2,
    };
    uint32 ClearType[FinishType_Count];
};*/

/*
struct QuestObjective
{
	INIT_GAMEDATA(QuestObjective)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "quest_objective";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuu";
	}

	uint32      Id;
	uint32      QuestId;
	uint32      description_id;
	uint32		objective_category_1;
	uint32      objective_category_2;
	uint32      objective_category_3;
	uint32      objective_value_1;
    uint32      objective_value_2;
};*/

struct QuestFilter
{
	INIT_GAMEDATA(QuestFilter)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return Id; }
    static const char* GetKeyName(){ return "Id"; }
    static const char* GetTableName(){ return "quest_filter"; }
	static const char* GetFormat() {
		return "usu";
	}

	uint32		Id;
	string		Name;
	uint32		Parent; 
};

STRING_DB_STRUCT(String_Quest)

struct QuestZone
{
	INIT_GAMEDATA(QuestZone)
    static const bool HasIndex(){ return true; }
    const uint32 GetKey() const{ return Id; }
    void SetKey(uint32 _id){ Id = _id; }
    static const char* GetKeyName(){ return "Id"; }
    static const char* GetTableName(){ return "quests_zones"; }
	static const char* GetFormat() {
		return "uufffffffs";
	}
	uint32 Id;
	uint32 map_id;
	float x;
	float y;
	float z;
//	float radius;
	float box_x;
	float box_y;
	float box_z;
	float box_orientation;
	std::string Comment;
public:
	bool IsCotain( float px, float pz ) const
	{
		return ::abs(px - x) < box_x / 2 && ::abs(z - pz) < box_z / 2 ;
	}
};

struct AreaTriggerGroup
{
	INIT_GAMEDATA(AreaTriggerGroup)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id)
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "quest_area_trigger_group";
	}
	static const char* GetFormat()
	{
		return "us";
	}
	uint32 Id;
	string name;
};

struct QuestGroup
{
    INIT_GAMEDATA(QuestGroup)
     static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return parent_quest_id;
    }
    void SetKey(uint32 _id)
    {
        parent_quest_id = _id;
    }
    static const char* GetKeyName()
    {
        return "parent_quest_id";
    }
    static const char* GetTableName()
    {
        return "quest_group";
    }
    static const char* GetFormat()
    {
        return "uuuuuuu";
    }
    bool isRandomAccordingToChance() const { return random_type == 0;}
    bool isAutoAccept() const              { return auto_accept != 0; }
 

    uint32 parent_quest_id;
    uint32 auto_accept;
    uint32 random_type;
    enum ADDITION_TYPE
    {
        ADDITION_TYPE_NONE = 0,
        ADDITION_TYPE_FULLY = 1,
        ADDITION_TYPE_IMMEDIATELY = 2,
    };
    uint32 addition_type;
    uint32 repeat_type;
    uint32 min_count;
    uint32 max_count;
};


struct QuestGroupSub
{
    INIT_GAMEDATA(QuestGroupSub)
    static const bool HasIndex()
    {
        return true;
    }
    const uint32 GetKey() const
    {
        return id;
    }
    void SetKey(uint32 _id)
    {
        id = _id;
    }
    static const char* GetKeyName()
    {
        return "id";
    }
    static const char* GetTableName()
    {
        return "quest_group_sub";
    }
    static const char* GetFormat()
    {
        return "uuuuuu";
    }

    bool isRequired() const{ return required != 0;}

    uint32 id;
    uint32 parent_quest_id;
    uint32 child_quest_id;
    uint32 chance;
    uint32 required;
    uint32 value;
};


struct DB_QuestCounter
{
    INIT_GAMEDATA(DB_QuestCounter);
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
        return "quest_counter";
    }
    static const char* GetFormat()
    {
        return "uuuuu";
    }

    uint32 index_id;
    uint32 event_type;
    uint32 category_1;
    uint32 category_2;
    uint32 category_3;
};




#pragma pack(pop)

#endif // __STRUCT_QUEST_H__
