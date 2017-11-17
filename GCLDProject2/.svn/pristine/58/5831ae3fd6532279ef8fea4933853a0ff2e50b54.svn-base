#pragma once
#include "def/TypeDef.h"
class City;
class Country;
namespace pb{
    class SD2G_Country_AllInfo;
	class Country_Levle_Up_Task_Rank;
}

class CountryDB {
public:
    static void SendBaseToDb(Country& data);
    static void SendNewCityToDb(int countryId, City& city);
    static void SendDelCityToDb(int countryId, City& city);


    static void RecvAllInfoFromDB(const pb::SD2G_Country_AllInfo& msg);
    static void SendAllInfoToDb();

    static void SendLevelUpTaskRankToDb(Country& data);
    static void RecvLevelUpTaskRankFromDB(const pb::Country_Levle_Up_Task_Rank& msg);
};