#pragma once
#include "structDef.h"

namespace pb{
    class GS2C_Smithy_All_Info;
}

//set byte alignment 
#pragma pack(push,1)

struct SmithyTable : public BaseStruct {
public:
	
    INIT_GAMEDATA(SmithyTable);
    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const{ return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "smithy_smelter"; }
	static const char* GetFormat()
	{
        return "bbuuuuuuuuuuuu";
	}
    void SaveTo(pb::GS2C_Smithy_All_Info& msg);
    void LoadFrom(const pb::GS2C_Smithy_All_Info& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      levle;
    uint32      silver_coin;
    uint32      start[5];
    uint32      smelt_times[5];
};


#pragma pack(pop)
