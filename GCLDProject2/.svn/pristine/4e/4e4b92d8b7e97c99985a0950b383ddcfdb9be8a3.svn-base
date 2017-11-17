#pragma once
#include "structDef.h"

namespace pb{
    class OfficeInfo;
	class PalaceAchievementInfo;
}

//set byte alignment 
#pragma pack(push,1)

struct PalaceOfficeTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(PalaceOfficeTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const{ return office_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "office_id"; }
    static const char* GetTableName(){ return "palace_office"; }
    static const char* GetFormat()
    {
        return "buu"
            "ccbb" "bbbbb" "bbbbbbbbbb";
    }
    void SaveTo(pb::OfficeInfo& msg);
    void LoadFrom(const pb::OfficeInfo& msg);

public:
    IdType      id;
    uint32      region_id;
    uint32      server_id;

    uint8       country_id;
    uint8       office_id;
    uint64      playerId;
    uint64      skillUseTime;
    uint64      aideLst[5];
    uint64      aideApplyLst[10];
};

struct	PalaceOfficeAchievement :public BaseStruct
{
	
	INIT_GAMEDATA(PalaceOfficeAchievement);

	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return id; }
	uint32 GetLogicalKey(void) const{ return office_id; }
	void SetDbIncreaseId(const IdType& id_){ id = id_; }
	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void){ return "office_id"; }
	static const char* GetTableName(){ return "palace_office_achievement"; }
	static const char* GetFormat()
	{
		return "bbuu";
	}
	void SaveTo(pb::PalaceAchievementInfo& msg);
	void LoadFrom(const pb::PalaceAchievementInfo& msg);

public:
	IdType      id;
	uint64      player_id;
	uint32		office_id;
	uint32		office_state;
};

#pragma pack(pop)
