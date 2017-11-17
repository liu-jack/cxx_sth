#pragma once
#include "structDef.h"
#include "Country.pb.h"
namespace pb{
   // class CountyInfo;
    class Struct_City_Info;
    //class CountyLevelUpTaskPlayer;
	//class CrossWarInfo;
	//enum CROSS_STATE;
}

//set byte alignment 
#pragma pack(push,1)

struct CountryBaseTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(CountryBaseTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const { return (uint32)country_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "country_id"; }
    static const char* GetTableName(){ return "country"; }
    static const char* GetFormat()
    {
        return "buucuuuuu"
            "buucc"
            "uuu"
            "uuu"
            "uuu"
            "uuu"
            "uuu"
			"bbuucccssssbuubu"
            ;
    }
    void SaveTo(pb::CountyInfo& msg);
    void LoadFrom(const pb::CountyInfo& msg);
	void SaveCrossWarInfo(pb::CrossWarInfo &info );
	void UpdateCrossWarInfo(const pb::CrossWarInfo&info);

public:
    IdType      id;
    uint32      region_id;
    uint32      server_id;
    uint8       country_id;
    uint32      exp;
    uint32      level;
    uint32      donate_times;
    uint32      npc_lv;
    uint32      npc_exp;

    uint64      m_OpenLevelUpTaskTime;
    uint32      m_CurLevelDoTaskCnt;
    uint32      m_KillMonsterNum;
    uint8       m_isMonsterTaskOpen;
    uint8       m_isMonsterTaskSucceed;

    //蛮族城池：捐赠、发动入侵
    uint32      donationMoney[3];
    uint32      friendVal[3];
    uint32      MonsterLevel[3];
    uint32      invadeCnt[3];
    uint32      today_invade_count[3];
	uint64      cross_war_start_time;//跨服战开时间
	uint64      anabasis_start_time;//远征开始时间
	pb::CROSS_STATE      cross_state;
	pb::CROSS_STATE      anabasis_state;
	uint8       isAnabasisSide;//远征方,false为被远征方
	uint8       isOpenedAnabasis;//是否已开启远征
	uint8       isOpenedCrossWar;//是否已开启跨服战
	string      crossServerIp;
	string      crossServerPort;
	string      anabasisServerIp;
	string      anabasisServerPort;
	uint64		check_npc_exp_time;	//上次检验npc经验时间
	uint32		npcLevelID;			//npc等级对应countryGuardUp表的id;
	uint32		tuntianLevel;
	uint64		tuntianExp;
	uint32		m_Prosperity_Degree;	//繁荣度
};

struct CountryCityTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(CountryCityTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint32 GetLogicalKey(void) const { return city_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "city_id"; }
    static const char* GetTableName(){ return "country_city"; }
    static const char* GetFormat()
    {
        return "buucuu";
    }
    void SaveTo(pb::Struct_City_Info& msg);
    void LoadFrom(const pb::Struct_City_Info& msg);

public:
    IdType      id;
    uint32      region_id;
    uint32      server_id;
    uint8       country_id;
    uint32      city_id;
    uint32      city_status;
};

struct CountryLevelUpTaskPlayerTable : public BaseStruct {
public:
    
    INIT_GAMEDATA(CountryLevelUpTaskPlayerTable);

    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return id; }
    uint64 GetLogicalKey(void) const { return player_id; }
    void SetDbIncreaseId(const IdType& id_){ id = id_; }
    static const char* GetDbIncreaseIdName(){ return "id"; }
    static const char* GetLogicalKeyName(void){ return "player_id"; }
    static const char* GetTableName(){ return "country_level_up_task_rank"; }
    static const char* GetFormat()
    {
        return "buuc"
            "buuc";
    }
    void SaveTo(pb::CountyLevelUpTaskPlayer& msg);
    void LoadFrom(const pb::CountyLevelUpTaskPlayer& msg);

public:
    IdType      id;
    uint32      region_id;
    uint32      server_id;
    uint8       country_id;

    uint64      player_id;
    uint32      rank;
    uint32      statisticKill;
    uint8       isTakeReward;
};

#pragma pack(pop)
