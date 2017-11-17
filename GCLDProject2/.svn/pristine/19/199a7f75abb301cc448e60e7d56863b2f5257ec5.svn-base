#pragma once

#include "def/TypeDef.h"
struct DB_MapCity;

class MapCity {
    const DB_MapCity& m_db;
    enum {
        CITY_TPPE_NORMAL     = 0,    //普通城市
        CITY_TYPE_CAPITAL    = 1,    //首都
        CITY_TYPE_FORTRESS   = 2,    //要塞
        CITY_TYPE_TONDEN     = 3,    //屯田区
		CITY_TYPE_MONSTER = 4,   //蛮族城市
    };
public:
    enum SpecialCity {
        Black_Market,   //减少黑市CD，资源获取量增加10%
        Market,         //集市购买翻倍，花费也翻倍
        Equipment,      //装备商店刷新CD减半
        Special_City_Cnt
    };
	struct NpcInfo
	{
		uint32 npc_id;
		uint32 npc_lv;
		uint32 npc_cnt;
	};
	static  uint32  GetSpacialCity(MapCity::SpecialCity cityType);
	
    std::vector<int>		m_linkCity;
	std::vector<NpcInfo>    m_npcInfo[10];

    explicit MapCity(const DB_MapCity& db);
    uint32 Id() const;
    uint32 MapId() const;

    bool IsCapital() const;
    bool IsTonden() const;
    bool IsFortress() const;
    bool IsNormalCity() const;
	bool IsMonsterCity() const;

    uint32 LandForm() const;
    uint32 CountryId() const;

    const std::vector<NpcInfo>& GetNpcLst(uint32 countryId) const;
};