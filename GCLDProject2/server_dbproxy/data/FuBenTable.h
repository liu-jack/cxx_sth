#pragma once
#include "structDef.h"
namespace pb{
    class SG2D_UpdataFuBenBase;
    class Stage_Info;
    class GS2C_Resource_Stage;
	class BranchFubenInfo;
	class HideFubenInfo;
}

//set byte alignment 
#pragma pack(push,1)

struct FuBenBaseTable : public BaseStruct {
public:
	
    INIT_GAMEDATA(FuBenBaseTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "fuben_base"; }
	static const char* GetFormat()
	{
		return "bbuuuuuuuuuuuuuuuuuuuu";
	}
    void SaveTo(pb::SG2D_UpdataFuBenBase& msg);
    void LoadFrom(const pb::SG2D_UpdataFuBenBase& msg);

public:
    IdType      id;
    uint64      player_id;
	uint32      cur_fuben_id;
    uint32      cur_stage_id;
	uint32		NR_stars[18];
};

struct FuBenHideTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(FuBenHideTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const{ return stage_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "stage_id"; }
    static const char* GetTableName(){ return "fuben_hide_record"; }
    static const char* GetFormat()
    {
        return "bbuuuu";
    }
    void SaveTo(pb::HideFubenInfo& msg);
    void LoadFrom(const pb::HideFubenInfo& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      stage_id;
    uint32      free_tiems;
    uint32      today_fight_times;
	uint32		kill_monsters;
};

struct FuBenPassStageTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(FuBenPassStageTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const{ return stage_type; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "stage_type"; }
    static const char* GetTableName(){ return "fuben_pass_stage"; }
    static const char* GetFormat()
    {
        return "bbuuu";
    }
    void SaveTo(pb::Stage_Info& msg);
    void LoadFrom(const pb::Stage_Info& msg);


public:
    IdType      id;
    uint64      player_id;
    uint32      stage_type;
	uint32		pass_count;
	uint32		is_take_reward;
};

struct BranchFuBenTable : public BaseStruct {
public:

	INIT_GAMEDATA(BranchFuBenTable);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return stage_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "stage_id"; }
	static const char* GetTableName(){ return "branch_fuben_record"; }
	static const char* GetFormat()
	{
		return "bbuucc";
	}
	void SaveTo(pb::BranchFubenInfo& msg);
	void LoadFrom(const pb::BranchFubenInfo& msg);


public:
	IdType      id;
	uint64      player_id;
	uint32      stage_id;
	uint32		branch_fuben_group_id;
	uint8 		is_passed;
	uint8		is_take_star;
};


#pragma pack(pop)
