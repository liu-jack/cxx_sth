#pragma once

#include "def/TypeDef.h"
struct DB_MapCity;

class MapCity {
    const DB_MapCity& m_db;
    enum {
        CITY_TPPE_NORMAL     = 0,    //��ͨ����
        CITY_TYPE_CAPITAL    = 1,    //�׶�
        CITY_TYPE_FORTRESS   = 2,    //Ҫ��
        CITY_TYPE_TONDEN     = 3,    //������
		CITY_TYPE_MONSTER = 4,   //�������
    };
public:
    enum SpecialCity {
        Black_Market,   //���ٺ���CD����Դ��ȡ������10%
        Market,         //���й��򷭱�������Ҳ����
        Equipment,      //װ���̵�ˢ��CD����
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