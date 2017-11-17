#pragma once
#include "def/TypeDef.h"
struct DB_MapFog;

class MapFog {
    const DB_MapFog& m_dbMapFog;	//��ǰ����ص�������Ϣ

public:
    std::vector<int>  m_openCity;	//�������һ�ؿ�֮�������ʾ��Щ����

   explicit MapFog(const DB_MapFog& db);

    uint16 Id() const;	//������

    uint32 CountryId() const;	//������
    uint32 BattleCityId() const;	//�ǳ�id
    uint32 StageId() const;	//ս���ؿ�
	uint32 ShowLevel() const;	//��ʾ�ȼ�
};