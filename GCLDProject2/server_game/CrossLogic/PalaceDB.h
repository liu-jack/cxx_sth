#pragma once
#include "def/TypeDef.h"
class City;
class Country;
namespace pb{
    class GS2C_Palace_All_Info;
}
class Player;
class PalaceDB {
public:
    static void RecvAllInfoFromDB(const pb::GS2C_Palace_All_Info& msg);
    static void SendAllInfoToDb();
    static void UpdateOfficeInfoToDb(int countryId, int officeId);
	static void SendAchievementInfoToDb(Player& player,int office_id);
};