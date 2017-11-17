#pragma once
#include "def/TypeDef.h"

class Player;
class CMap;
class City;
class StoryMap;
namespace pb
{
    class GS2C_Story_All_Info;
}

class PlayerStoryLog {
private:
    StoryMap* m_pMap;
public:
    PlayerStoryLog();
    
    bool EnterStory(Player* player,uint32 mapId);
    void ExitStory(Player* player);
    static void MoveToMap(Player& player, CMap& map);

    void SetTargetCity(uint32 cityId); //攻占目标城池，剧本结束
    void _OnStoryEnd(City* pCity);
private:
    StoryMap* CreateMap(uint32 mapId) const;
};