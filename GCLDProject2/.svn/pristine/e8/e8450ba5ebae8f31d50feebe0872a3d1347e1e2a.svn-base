#include "PlayerStoryLog.h"
#include "StoryMap.h"
#include "../object/Player.h"
#include "../character/CharacterStorage.h"
#include "../Map/Table/MapTableMgr.h"
#include "../Map/WorldMap.h"
#include "../Map/PlayerMapLogic.h"
#include "../Map/city/city.h"

PlayerStoryLog::PlayerStoryLog()
    : m_pMap(NULL)
{

}
StoryMap* PlayerStoryLog::CreateMap(uint32 mapId) const
{
    StoryMap* ptr = new StoryMap(mapId, CMap::Crusade);

    ptr->Init();

    return ptr;
}
void PlayerStoryLog::MoveToMap(Player& player, CMap& map)
{
    struct _Temp_Hero_Func //辅助函数对象
    {
        CMap& _map;

        _Temp_Hero_Func(CMap& p) : _map(p) {}

        void operator()(Character* p)
        {
            // 先把武将全拖回主城
            p->FlyTo(sMapTableMgr.GetCapitalCity(p->GetCountryId()));
            p->SetMap(&_map);
        }
    };
    _Temp_Hero_Func  objFunc(map);
    player.m_characterStorage->ForEachBattleCharacter(objFunc);
}
bool PlayerStoryLog::EnterStory(Player* player,uint32 mapId)
{
    if (m_pMap) {
        ASSERT(0);
        return false;
    }
    m_pMap = CreateMap(mapId);

    m_pMap->AddToMap(*player);

    // 先分配国家，再进剧本地图中的城池
    player->m_worldMapLogic->SetStoryCountryId(1); //Notice：剧本中，玩家国家约定为 1 

    MoveToMap(*player, *m_pMap);
    return true;
}
void PlayerStoryLog::ExitStory(Player* player)
{
    if (m_pMap == NULL) return;

    m_pMap->DelFromMap(*player);

    player->m_worldMapLogic->SetStoryCountryId(0);

    MoveToMap(*player, sWorldMap);

    delete m_pMap;
    m_pMap = NULL;
}
void PlayerStoryLog::SetTargetCity(uint32 cityId)
{
    if (m_pMap == NULL) {
        ASSERT(0);
        return;
    }
    if (City* pCity = m_pMap->GetCity(cityId))
    {
        pCity->AddCallBack_BeOccupied(boost::bind(&PlayerStoryLog::_OnStoryEnd, this, _1));
    }
}
void PlayerStoryLog::_OnStoryEnd(City* pCity)
{

}
