#include "SuppressMonster.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "../../session/PlayerPool.h"
#include "../Table/ActTableMgr.h"
#include "../../reward/reward.h"
#ifdef _MMO_SERVER_
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "../../Map/country/country.h"
#include "../../Map/country/task/CountryTaskCity.h"
#include "../../Map/Table/MapCountry.h"
#include "../../Npc/NpcMgr.h"
#include "../../Map/city/city.h"
#include "../../Combat/CombatObj.h"
#include "Country.pb.h"
#include "../../Loot/LootList.h"
#include "Map/WorldMap.h"


SuppressMonster::TaskPlayer::TaskPlayer()
{
    ZeroMemoryThis;
}
SuppressMonster::SuppressMonster()
    : m_RankLst(40)
{
    m_isBegined = false;
    m_isWaiting = false;
    m_curStage = 0;
    m_curStageBeginTime = 0;
    ZeroMemoryArray(m_isContinue);
}
void SuppressMonster::SaveTo(pb::Struct_Country_Task_Base& msg)
{
    msg.set_is_begin(m_isBegined);
    msg.set_cur_stage(m_curStage);
    msg.set_cur_stage_begin_time(m_curStageBeginTime);
    msg.set_is_waiting_state(m_isWaiting);

    if (const Table_Suppress* table = sActTableMgr.GetSuppress(m_curStage))
    {
        for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
        {
            msg.add_cur_stage_fighting_city_num(table->m_TargetCity[i].size());
        }
    }
}
void SuppressMonster::SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player)
{
    TaskPlayer* pData = GetPlayerData(player);
    if (pData == NULL) return;

    msg.set_rank(pData->rank);
    msg.set_kill_count(pData->statisticKill);
    msg.add_is_take_reward(pData->isTakeReward);
    Country::GetRankReward(player, LOOTTYPE_COUNTRYQUEST, pData->rank).SaveTo(*msg.add_rank_reward());
}
bool SuppressMonster::Begin(int lastSecond)
{
    if (m_isBegined) return false;

    m_isBegined = true;
    m_isWaiting = true;
    m_curStage = 0;
    m_curStageBeginTime = 0;
    memset(m_isContinue, 1, sizeof(m_isContinue));

    _EnterNextStage();
#ifdef _MMO_SERVER_
    sAutoUpdateGame.AddEvent(boost::bind(&SuppressMonster::End, this), lastSecond);
#endif
	return true;
}
void SuppressMonster::End()
{
    m_isBegined = false;
}

void SuppressMonster::_EnterNextStage()
{
    m_isWaiting = !m_isWaiting;

    // 检查上一轮结果，失败则不继续
    if (const Table_Suppress* table = sActTableMgr.GetSuppress(m_curStage)) {
        for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i) {
            for (std::vector<int>::const_iterator it = table->m_TargetCity[i].begin(); it != table->m_TargetCity[i].end(); ++it)
            {
                if (_IsCityMonsterClear(*it)) {
                    sWorldMap.GetCountry(i+1)->AddCountryExp(table->m_db.reward_country_exp);
                } else {
                    m_isContinue[i] = false;
                }
            }
        }
    }

    if (m_isWaiting) {
#ifdef _MMO_SERVER_
        sAutoUpdateGame.AddEvent(boost::bind(&SuppressMonster::_EnterNextStage, this), 60);
#endif
    } 
    else {
        m_curStage += 1;
        m_curStageBeginTime = OS::TimeSeconds();

        ClearPlayerData();

        if (const Table_Suppress* table = sActTableMgr.GetSuppress(m_curStage)) {
            for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i) {
                if (m_isContinue[i]) {

                    int npcId = 0, npcLv = 0;
                    GetMonsterInfo(i+1, npcId, npcLv);

                    for (std::vector<int>::const_iterator it = table->m_TargetCity[i].begin(); it != table->m_TargetCity[i].end(); ++it)
                    {
                        _AddMonsterInCity(*it, table->m_db.npc_num, npcId, npcLv);
                    }
                }
            }
#ifdef _MMO_SERVER_
            sAutoUpdateGame.AddEvent(boost::bind(&SuppressMonster::_EnterNextStage, this), table->m_db.last_time);
#endif
        }
    }
}
bool SuppressMonster::TakeReward(Player& player, int rank)
{
    if (!m_isWaiting) return false;

    if (TaskPlayer* pData = GetPlayerData(player))
    {
        if (pData->isTakeReward) return false;

        pData->isTakeReward = true;

        Country::TakeRankReward(player, LOOTTYPE_COUNTRYQUEST, pData->rank);

        return true;
    }
    return false;
}

void SuppressMonster::TakeAwardsBeforeStart()
{

}

void SuppressMonster::AddKillCnt(uint64 playerId, uint32 cnt)
{
    if (m_isBegined)
    {
        TaskPlayer& obj = m_playerLst[playerId];
        obj.statisticKill += cnt;
        m_RankLst.OnValueChange(obj);
    }
}

SuppressMonster::TaskPlayer* SuppressMonster::GetPlayerData(Player& player)
{
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
    if (it == m_playerLst.end()) return NULL;

    return &it->second;
}
void SuppressMonster::GetMonsterInfo(int countryId, int& npcId, int& npcLv)
{
    if (Country* country = sWorldMap.GetCountry(countryId))
    {
        if (const MapCountryUp* table = sCountryTableMgr.GetCountryUp(country->GetLevel()))
        {
            npcId = table->m_db.suppress_monster_npc_id;
            npcLv = table->m_db.suppress_monster_npc_lv;
        }
    }
}
void SuppressMonster::_AddMonsterInCity(int cityId, int npcCnt, int npcId, int npcLv)
{
    for (int i = 0; i < npcCnt; ++i)
    {
        sNpcMgr.CreateNpc(npcId, npcLv, MONSTER_COUNTRY_ID, sWorldMap.GetCity(cityId));
    }
}
bool SuppressMonster::_IsCityMonsterClear(int cityId)
{
    if (City* pCity = sWorldMap.GetCity(cityId))
    {
        std::set<Combat::CombatObj*>::iterator it = pCity->m_defender.begin();
        for (; it != pCity->m_defender.end(); ++it)
        {
            if ((*it)->GetCountryId() == MONSTER_COUNTRY_ID)
            {
                return false;
            }
        }
    }
    return true;
}
void SuppressMonster::ClearPlayerData()
{
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin();
    for (; it != m_playerLst.end(); ++it)
    {
        if (Player* palyer = sPlayerPool.GetByPlayerId(it->first)){
            TakeReward(*palyer, it->second.rank);
        }
    }
    m_playerLst.clear();
    m_RankLst.Clear();
}
