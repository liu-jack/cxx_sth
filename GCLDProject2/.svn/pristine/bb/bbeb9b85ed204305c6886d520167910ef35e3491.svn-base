#pragma once
#include "structDef.h"

namespace pb{
   // class C2GS_Technology_All_Info;
    class TechnologyStudy;
}

//set byte alignment 
#pragma pack(push,1)

struct TechnologyStudyTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(TechnologyStudyTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const{ return tec_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "tec_id"; }
    static const char* GetTableName(){ return "technology_study"; }
    static const char* GetFormat()
    {
        return "bbuubu";
    }
    void SaveTo(pb::TechnologyStudy& msg);
    void LoadFrom(const pb::TechnologyStudy& msg);

public:
    IdType      id;
    uint64      player_id;
    uint32      tec_id;
    uint32      star;
    uint64      end_time;
	uint32		is_finished;
};

#pragma pack(pop)
