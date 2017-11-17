#include "PalaceSkill.h"
#include "PalaceMgr.h"
#include "Base.h"
#include "../object/Player.h"
#include "../character/Character.h"
#include "../character/CharacterStorage.h"
#include "../Map/city/city.h"
#ifdef _MMO_SERVER_
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "Map/WorldMap.h"

bool PalaceSkill::HaveOffice(Player& player, int officeId)
{
    const int countryId = player.GetCountryId();

    const int curOfficeId = sPalaceMgr(countryId)->GetOfficeId(player.GetGuid());

    return curOfficeId == officeId;
}
bool PalaceSkill::RefreshSkillCD(Player& player, int officeId)
{
    const int countryId = player.GetCountryId();

    std::map<uint8, PalaceMgr::OfficeInfo>& refMap = sPalaceMgr(countryId)->m_OfficeMap;
    std::map<uint8, PalaceMgr::OfficeInfo>::iterator it = refMap.find(officeId);
    if (it == refMap.end()) return false;

    uint64 timeNow = sOS.TimeSeconds();
    if (timeNow >= it->second.skillUseTime + 10/*12 * 3600*/)
    {
        it->second.skillUseTime = timeNow;
        return true;
    }
    return false;
}

#define PalaceSkill_Common_Logic(officeId) \
    if (!HaveOffice(player, officeId)) {ASSERT(0); return;} \
    if (!RefreshSkillCD(player, officeId)) return;

void PalaceSkill::FlyTo(Player& player, int cityId)   //»ÊµÛ-Ææ±ø
{
    PalaceSkill_Common_Logic(1);

    struct _Temp_Fly_Hero //¸¨Öúº¯Êý¶ÔÏó
    {
        const int kCityId;

        _Temp_Fly_Hero(int id) : kCityId(id) {}
        bool operator()(Character* p)
        {
            return p->FlyTo(kCityId);
        }
    };
    _Temp_Fly_Hero  objFunc(cityId);
    player.m_characterStorage->ForEachBattleCharacter(objFunc);
}

#ifdef  _MMO_SERVER_
static void _Empty_Assist(City* city)
{
    city->SetStatus(City::ForbidEnemy, false);
}
#endif
void PalaceSkill::Empty(Player& player, int cityId)   //´¢¾ý-¿Õ³Ç
{
    PalaceSkill_Common_Logic(2);

    if (City* city = sWorldMap.GetCity(cityId))
    {
        if (city->GetCountryId() == player.GetCountryId())
        {
#ifdef _MMO_SERVER_
            city->SetStatus(City::ForbidEnemy, true);
            // 10min
            sAutoUpdateGame.AddEvent(
                boost::bind(_Empty_Assist, city),
                10 * 60
            );
#endif
        }
    }
}
#ifdef  _MMO_SERVER_
static void _Confuse_Assist(City* city)
{
    city->SetStatus(City::Confuse, false);
}
#endif
void PalaceSkill::Confuse(Player& player, int cityId) //ÔªÀÏ-»ìÂÒ
{
    PalaceSkill_Common_Logic(3);

    if (City* city = sWorldMap.GetCity(cityId))
    {
        if (city->GetCountryId() == player.GetCountryId())
        {
#ifdef _MMO_SERVER_
            city->SetStatus(City::Confuse, true);
            // 10min
            sAutoUpdateGame.AddEvent(
                boost::bind(_Confuse_Assist, city),
                10 * 60
                );
#endif
        }
    }
}
#ifdef  _MMO_SERVER_
static void _Maze_Assist(City* city, int countryId)
{
    city->SetMazeStatus(countryId, false);
}
#endif
void PalaceSkill::Maze(Player& player, int cityId)    //Ì«Ê¦-ÃÔ¹¬
{
    PalaceSkill_Common_Logic(4);

    if (City* city = sWorldMap.GetCity(cityId))
    {
        int countryId = player.GetCountryId();

        city->SetMazeStatus(countryId, true);
#ifdef _MMO_SERVER_
        // 10min
        sAutoUpdateGame.AddEvent(
            boost::bind(_Maze_Assist, city, countryId),
            10 * 60
            );
#endif
    }
}

#ifdef  _MMO_SERVER_
static void _Trap_Assist(City* city, int countryId)
{
    city->SetTrapStatus(countryId, false);
}
#endif
void PalaceSkill::Trap(Player& player, int cityId)    //´ó½«-ÏÝÚå
{
    PalaceSkill_Common_Logic(5);

    if (City* city = sWorldMap.GetCity(cityId))
    {
        int countryId = player.GetCountryId();

        city->SetTrapStatus(countryId, true);
#ifdef _MMO_SERVER_
        // 10min
        sAutoUpdateGame.AddEvent(
            boost::bind(_Trap_Assist, city, countryId),
            10 * 60
            );
#endif		
    }
}
#undef PalaceSkill_Common_Logic