#pragma once
#include "structDef.h"
//#include "Country.pb.h"
//namespace pb{
//   // class CountyInfo;
//    class Struct_City_Info;
//    //class CountyLevelUpTaskPlayer;
//	//class CrossWarInfo;
//	//enum CROSS_STATE;
//}

//set byte alignment 
#pragma pack(push,1)

struct CountryWarTable {
public:
    INIT_GAMEDATA(CountryWarTable);
	DECLARE_INCREASEID_FUCTION(uint64,CombinId,id,"id");
    static const bool HasIndex(){ return true; }
    const Key GetKey(void) const{ return Key(server_id,country_id); }
    static const char* GetLogicalKeyName(void){ return "country_id"; }
    static const char* GetTableName(){ return "country_war"; }
    static const char* GetFormat()
    {
        return "buucssuuu";
    }
  /*  void SaveTo(pb::CountyInfo& msg);
    void LoadFrom(const pb::CountyInfo& msg);
	void SaveCrossWarInfo(pb::CrossWarInfo &info );
	void UpdateCrossWarInfo(const pb::CrossWarInfo&info);*/

public:
    uint64      id;
    uint32      region_id;
    uint32      server_id;
    uint8       country_id;
	string      crossServerIp;
	string      crossServerPort;
	uint32      glory;
    uint32      exp;
    uint32      level;
};

struct CountryWarMatchTable{
public:
	INIT_GAMEDATA(CountryWarMatchTable);
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
	static const bool HasIndex(){ return true; }
	const Key& GetKey(void) const{ return id; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "country_war_match"; }
	static const char* GetFormat()
	{
		return "buuucccut";
	}
	//void SaveTo(pb::CountyInfo& msg);
	//void LoadFrom(const pb::CountyInfo& msg);
	//void SaveCrossWarInfo(pb::CrossWarInfo &info );
	//void UpdateCrossWarInfo(const pb::CrossWarInfo&info);

public:
	uint64      id;
	uint32      server_id1;
	uint32      server_id2;
	uint32      server_id3;
	uint8      country_id1;
	uint8      country_id2;
	uint8      country_id3;
	uint32      war_state;
	uint64      war_start_time;
};

struct CountryAnabasisTable{
public:
	INIT_GAMEDATA(CountryAnabasisTable);
	DECLARE_INCREASEID_FUCTION(uint64,CombinId,id,"id");
	static const bool HasIndex(){ return true; }
	const Key GetKey(void) const{ return Key(server_id,country_id); }
	static const char* GetLogicalKeyName(void){ return "country_id"; }
	static const char* GetTableName(){ return "country_anabasis"; }
	static const char* GetFormat()
	{
		return "buuccssuuu";
	}
	/*void SaveTo(pb::CountyInfo& msg);
	void LoadFrom(const pb::CountyInfo& msg);
	void SaveCrossWarInfo(pb::CrossWarInfo &info );
	void UpdateCrossWarInfo(const pb::CrossWarInfo&info);*/

public:
	uint64      id;
	uint32      region_id;
	uint32      server_id;
	uint8       country_id;
	uint8       isAnabasisSide;
	string      anabasisServerIp;
	string      anabasisServerPort;
	uint32      glory;
	uint32      exp;
	uint32      level;
};

struct CountryAnabasisMatchTable {
public:
	INIT_GAMEDATA(CountryAnabasisMatchTable);
	DECLARE_INCREASEID_FUCTION(uint64,uint64,id,"id");
	static const bool HasIndex(){ return true; }
	const Key& GetKey(void) const{ return id; }
	static const char* GetLogicalKeyName(void){ return "id"; }
	static const char* GetTableName(){ return "country_anabasis_match"; }
	static const char* GetFormat()
	{
		return "bucucccut";
	}
	//void SaveTo(pb::CountyInfo& msg);
	//void LoadFrom(const pb::CountyInfo& msg);
	//void SaveCrossWarInfo(pb::CrossWarInfo &info );
	//void UpdateCrossWarInfo(const pb::CrossWarInfo&info);

public:
	uint64      id;
	uint32      req_server_id;
	uint8       req_country_id;
	uint32      accp_server_id;
	uint8       accp_country1;
	uint8       accp_country2;
	uint8       accp_country3;
	uint32		anabasis_strart;
	time_t      anabasis_start_time;
};


#pragma pack(pop)
