#pragma once
#include "def/TypeDef.h"
#include "Base.h"


namespace pb{
	class C2GS_Start_Auto_Country_War;
	class C2GS_Quick_Training;
	class GS2C_Auto_Country_War_Info;
	class C2GS_Reauto_Country_War;
	class C2GS_Change_Target_City;
	class GS2C_QuickTraining_Ret;
	class GS2C_Start_AC_Ret;
	class GS2C_REAUTO_HERO_Ret;
	class GS2C_Change_Des_Ret;
}

class Player;
class Character;
class Country;

class PlayerAutoCountryWarLogic
{
public:
	PlayerAutoCountryWarLogic();
	bool Init();

	void Reset();

	void Update(Player* player, uint64 timeMS);

	void Stop(bool isShutDown = false);

public:
	void SaveTo(pb::GS2C_Auto_Country_War_Info& msg, bool isShutDown = false);

public:
	void SetAutoCountryWar(bool value) { m_isAutoCountryWar = value; }
	bool GetAutoCountryWar() { return m_isAutoCountryWar; }

	void SetUseCoinConscript(bool value) { m_isUseCoinConscript = value; }
	bool GetUseCoinConscript() { return m_isUseCoinConscript; }

	void SetAutoCallUp(bool value) { m_isAutoCallUp = value; }
	bool GetAutoCallUp() { return m_isAutoCallUp; }

	void SetDesCityId(uint32 cityid) { m_desCityId = cityid; }
	uint32 GetDesCityId() { return m_desCityId; }

	void SetCallUpCity(uint32 cityid, uint64 callid) { m_callUpCity = cityid; m_callUpId = callid; }
	uint32 GetCallUpCity() { return m_callUpCity; }
	uint64 GetCallUpId() { return m_callUpId; }

	void SetIsAttackGoal(bool val) { m_isAttackGoal = val; }
	bool GetIsAttackGoal() { return m_isAttackGoal; }
	void ChechAutoWarEnd(uint32 belongCountry);

	void AddKillCount(uint32 killcount) { m_totalKillCount += killcount; }
	uint32 GetKillCount() { return m_totalKillCount; }

	void AddExpGet(uint32 expget) { m_totalExpGet += expget; }
	uint32 GetExpGet() { return m_totalExpGet; }

	void AddCityOccuty(uint32 cityOccupy) { m_totalCityOccuty += cityOccupy; }
	uint32 GetCityOccupy() { return m_totalCityOccuty;}
 
public:
	void QuickTraining(Player* player, pb::C2GS_Quick_Training& msg, pb::GS2C_QuickTraining_Ret& sendmsg);
	void StartAutoCountryWar(Player* player, pb::C2GS_Start_Auto_Country_War& msg, pb::GS2C_Start_AC_Ret& sendmsg);
	void ReautoHero(Player* player, pb::C2GS_Reauto_Country_War& msg, pb::GS2C_REAUTO_HERO_Ret& sendmsg);
	void SetDesCity(Player* player, pb::C2GS_Change_Target_City& msg, pb::GS2C_Change_Des_Ret& sendmsg);

private:
	bool m_isAutoCountryWar;
	bool m_isUseCoinConscript;
	bool m_isAutoCallUp;

	uint32 m_desCityId;
	uint32 m_callUpCity;	//待征召城池
	uint64 m_callUpId;		//待征召id

	Country* myCountry;	//自己的国家
	bool m_isAttackGoal;	//是否是进攻城市 进攻城市的结束条件为攻下城市，防守城市的结束条件为结束战斗

	uint32 m_totalKillCount;
	uint32 m_totalExpGet;
	uint32 m_totalCityOccuty;

	Player* m_player;
public:
	std::set<Character*> m_herolst;
	uint64 m_autoWarEndTime;
};