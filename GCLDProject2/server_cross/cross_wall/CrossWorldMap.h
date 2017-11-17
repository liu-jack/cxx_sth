#pragma once
#include "../../server_game/Map/base/BaseMap.h"

class CrossWorldMap : public CMap 
{
	CrossWorldMap(uint32 mapTid, MapEnum typ);
public:
	// ����������ͼ
	static CrossWorldMap& Instance(){ static CrossWorldMap T(1, CMap::World); return T; }

public:
	void Init();
	uint32 GetCountryidByUnique(uint32 uniqueid);

	void NoticeOfDurabilityChange(uint32 countryid, uint32 attcountryid);

	std::map<uint32, uint32>	m_uniqueToCountry; //uniqueid��Ӧ�Ĺ���
	uint32 countrycount;

};
#define sCrossWorldMap CrossWorldMap::Instance()