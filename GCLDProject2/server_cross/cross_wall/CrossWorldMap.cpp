#include "CrossWorldMap.h"
#include "def/ConstDef.h"
#include "object/Player.h"
#include "Map/city/city.h"
#include "Map/country/country.h"
#include "Map/Table/MapTableMgr.h"
#include "Service/ServiceMgr.h"
#include "session/PlayerPool.h"
#include "Map/PlayerMapLogic.h"
#include "CrossWar.pb.h"
#include "Opcode.pb.h"

using namespace pb;

static uint32 expedition_id_fore = 0;


CrossWorldMap::CrossWorldMap(uint32 mapTid, MapEnum typ)	//mapTid��ʼ��Ϊ0��
: CMap(mapTid, typ)
{
	countrycount = 0;
	NLOG("mapTid=%d",mapTid);
}

//1 ��ū - ����
//2 ���� - ����
//3 �� - ����
//4 ����
void CrossWorldMap::Init()
{
	CMap::Init();
}	

uint32 CrossWorldMap::GetCountryidByUnique(uint32 uniqueid)
{
	std::map<uint32, uint32>::iterator it = m_uniqueToCountry.find(uniqueid);
	if(it!= m_uniqueToCountry.end())
		return it->second;
	else
		return 0;
}

void CrossWorldMap::NoticeOfDurabilityChange(uint32 countryid, uint32 attcountryid)
{
	if(Country* country = sCrossWorldMap.GetCountry(countryid))
	{
		GS2C_Cross_Update_Country_Durable sendmsg;
		sendmsg.set_crosscountryid(countryid);
		sendmsg.set_countrydurable(country->GetCrossCountryDurability());
		sendmsg.set_attcountryid(attcountryid);
		sPlayerPool.BroadcastMsgToAll(SMSG_CROSS_COUNTRY_DURABLE_UPDATE,sendmsg);
	}
}