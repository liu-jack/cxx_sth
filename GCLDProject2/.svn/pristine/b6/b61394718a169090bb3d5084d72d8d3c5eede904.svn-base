#include "MapExpedition.h"
#include "datastruct/struct_city.h"
#include "utility/Utility.h"

MapExpedition::MapExpedition(const DB_Expedition& db) : m_dbMapExpedition(db)
{
	Utility::SplitStr(m_dbMapExpedition.npcWay, citys, ',');
}

uint32 MapExpedition::GetTroopCountryID()
{
	return m_dbMapExpedition.troopCountry;
}

uint32 MapExpedition::GetNpcID()
{
	return m_dbMapExpedition.npcid;
}

uint32 MapExpedition::GetNpcNum()
{
	return m_dbMapExpedition.npcNum;
}

uint32 MapExpedition::GetTroopType()
{
	return m_dbMapExpedition.troopType;
}

std::vector<int>& MapExpedition::GetCitys()
{
	return citys;
}