#pragma once

#include "def/TypeDef.h"
struct DB_Expedition;

class MapExpedition
{
	const DB_Expedition& m_dbMapExpedition;

public:
	std::vector<int>  citys;	//城池路线或者检测点

	explicit MapExpedition(const DB_Expedition& db);
	uint32 GetTroopCountryID();
	uint32 GetNpcID();
	uint32 GetNpcNum();
	uint32 GetTroopType();
	std::vector<int>& GetCitys();
};