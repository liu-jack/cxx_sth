#include "MapCity.h"
#include "datastruct/struct_city.h"
#include "utility/Utility.h"
#include "def/ConstDef.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"

uint32 MapCity::GetSpacialCity( MapCity::SpecialCity cityType )
{
	if (cityType ==  MapCity::Black_Market)
	{
		return GET_BASE_DEF_UINT(pb::BD_BLACKMARKET_FUNCTION_CITY);
	}
	else if (cityType ==  MapCity::Market)
	{
		return GET_BASE_DEF_UINT(pb::BD_MARKET_FUNCTION_CITY);
	}
	else if (cityType ==  MapCity::Equipment)
	{
		return GET_BASE_DEF_UINT(pb::BD_EQUIPSHOP_FUNCTION_CITY);
	}
	return 0;
}

MapCity::MapCity(const DB_MapCity& db) : m_db(db)
{
	MMO_ASSERT(ARRAY_SIZE(m_db.guard_npc) == ARRAY_SIZE(m_npcInfo));

    Utility::SplitStr(m_db.link_city, m_linkCity, '|');

	IntPairVec vec;
	for (int i = 0; i < ARRAY_SIZE(m_db.guard_npc); ++i) {
		vec.clear();
		Utility::SplitStr2(m_db.guard_npc[i], vec);

		NpcInfo tmp;
		for (IntPairVec::iterator it = vec.begin(); it != vec.end(); ++it) {
			tmp.npc_id = it->first / 100000;
			tmp.npc_lv = it->first % 1000;
			tmp.npc_cnt = it->second;
			m_npcInfo[i].push_back(tmp);
		}
	}
}
uint32 MapCity::Id() const { return m_db.id; }
uint32 MapCity::MapId() const { return m_db.map_id; }

bool MapCity::IsCapital() const{ return m_db.type == CITY_TYPE_CAPITAL; }
bool MapCity::IsTonden() const{ return m_db.type == CITY_TYPE_TONDEN; }
bool MapCity::IsFortress() const{ return m_db.type == CITY_TYPE_FORTRESS; }
bool MapCity::IsNormalCity() const{ return m_db.type == CITY_TPPE_NORMAL; }

bool MapCity::IsMonsterCity() const
{
	return m_db.type == CITY_TYPE_MONSTER;
}

uint32 MapCity::LandForm() const{ return m_db.landform; }
uint32 MapCity::CountryId() const{ return m_db.country; }


const std::vector<MapCity::NpcInfo>& MapCity::GetNpcLst(uint32 countryId) const
{
    return m_npcInfo[countryId - 1];
}