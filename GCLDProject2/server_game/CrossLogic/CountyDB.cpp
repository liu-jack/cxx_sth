#include "CountyDB.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "InterServer.pb.h"
#include "City.pb.h"
#include "Country.pb.h"
#include "../Map/country/country.h"
#include "../Map/city/city.h"
#include "Map/WorldMap.h"


void CountryDB::SendBaseToDb(Country& data)
{
#ifdef _MMO_SERVER_
    pb::CountyInfo msg;
    data.SaveTo(msg);
    sDbProxyClient.Send(pb::SG2D_COUNTRY_BASE, msg);
#endif
}
void CountryDB::SendLevelUpTaskRankToDb(Country& data)
{
#ifdef _MMO_SERVER_
    pb::Country_Levle_Up_Task_Rank msg;
    data.SaveTo(msg);
	sDbProxyClient.Send(pb::SG2D_COUNTRY_LEVLE_UP_TASK_RANK, msg);
#endif
}
void CountryDB::SendNewCityToDb(int countryId, City& city)
{
#ifdef _MMO_SERVER_
    pb::Struct_City_Info msg;
    city.SaveTo(msg);
    msg.set_country(countryId);
	sDbProxyClient.Send(pb::SG2D_COUNTRY_ADD_CITY, msg);
#endif
}
void CountryDB::SendDelCityToDb(int countryId, City& city)
{
#ifdef _MMO_SERVER_
    pb::Struct_City_Info msg;
    city.SaveTo(msg);
    msg.set_country(countryId);
	sDbProxyClient.Send(pb::SG2D_COUNTRY_DEL_CITY, msg);
#endif
}
void CountryDB::RecvAllInfoFromDB(const pb::SD2G_Country_AllInfo& msg)
{
#ifdef _MMO_SERVER_
	LLOG("CountryDB::RecvAllInfoFromDB");
    if (msg.base_info_size() > 0)
    {
		LLOG("CountryDB::RecvAllInfoFromDB  get all city info");
        for (int i = 0; i < msg.base_info_size(); ++i)
        {
            if (Country* country = sWorldMap.GetCountry(i + 1))
            {
                country->LoadFrom(msg.base_info(i));
                country->m_setCity.clear();
            }
        }
	}
	if(msg.city_lst_size() > 0)
	{
        for (int i = 0; i < msg.city_lst_size(); ++i)
        {
            const pb::Struct_City_Info& cityInfo = msg.city_lst(i);
            if (Country* country = sWorldMap.GetCountry(cityInfo.country()))
            {
                country->m_setCity.insert(cityInfo.city_id());
            }
            sWorldMap.GetCity(cityInfo.city_id())->LoadFrom(cityInfo);
        }
	}
	sWorldMap.ResetEncircledCity();
    
	if(msg.base_info_size() == 0 || msg.city_lst_size() == 0)
	{
		LLOG("CountryDB::RecvAllInfoFromDB  No");
        //开服，数据库中没有，发过去
        SendAllInfoToDb();
    }
	sWorldMap.InitArmysAfterDB();
#endif
}
void CountryDB::RecvLevelUpTaskRankFromDB(const pb::Country_Levle_Up_Task_Rank& msg)
{
    if (Country* country = sWorldMap.GetCountry(msg.country()))
    {
        country->LoadFrom(msg);
    }
}

void CountryDB::SendAllInfoToDb()
{
#ifdef _MMO_SERVER_
    pb::SD2G_Country_AllInfo msg;
	sWorldMap.SaveAllCountryData(msg);
	sDbProxyClient.Send(pb::SG2D_COUNTRY_ALL_INFO, msg);
#endif
}