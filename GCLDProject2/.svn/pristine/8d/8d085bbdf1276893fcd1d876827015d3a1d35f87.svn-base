#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"

class Player;
class Country;
namespace pb
{
    class MonsterCityLst;
	class GS2C_Monster_City_Donate;
	class MonsterCity;
	class GS2C_Monster_City_Invade_Rsp;
}

class MonsterCity {
    struct Info {
		uint32 donationMoney;
		uint32 MonsterLevel;   //捐赠金钱
        uint32 friendVal;		//亲密度
        uint8  invadeCnt;	//有效入侵次数
		uint8  today_invade_count; //当天发动的入侵次数
      //  uint64 invadeTime;	//入侵时间
		
    };
    Info m_CountryLst[MAX_PLAYER_COUNTRY_ID];

public:
    MonsterCity();

    void SaveTo(pb::MonsterCityLst& msg);
    void LoadForm(const pb::MonsterCityLst& msg);
	void SaveTo(const Info& info,pb::MonsterCity& msg);

	bool Donate(Player& player,Country& myCountry,Country& dest_country,pb::GS2C_Monster_City_Donate& msg);    //哪个国家的蛮族
    static bool ClearDonateCD(Player& player);
	bool StartInvade(Player& player,Country& dest_country,pb::GS2C_Monster_City_Invade_Rsp& msg,const uint32 mycountryLevel); //发动蛮族入侵

public:
	void RefreshTodayInvadeCnt(const uint32 country_id);
};
