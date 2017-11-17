#include "Tournament.h"
#include "OS.h"
#include "Base.h"
#include "Combat.pb.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "../../character/CharacterStorage.h"
#include "../../Combat/CombatGroup.h"
#include "../../session/PlayerPool.h"
#include "../../Map/country/country.h"
#include "algorithm"
#include "Country.pb.h"
#include "../../Loot/LootList.h"
#include "Map/WorldMap.h"
#include "System.h"  
ActTournament::TaskPlayer::TaskPlayer()
{
    ZeroMemoryThis;
}

ActTournament::ActTournament()
    : m_RankLst(100)
{
    m_isBegined = false;
}
void ActTournament::SaveTo(pb::Struct_Country_Task_Base& msg)
{
    msg.set_is_begin(m_isBegined);

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        msg.add_country_score(m_countryRank[i].amountScore);
    }
}
void ActTournament::SaveTo(pb::GS2C_Country_Task_Player& msg, Player& player)
{
    TaskPlayer* pData = GetPlayerData(player);
    if (pData == NULL) return;

    msg.set_rank(pData->rank);
    msg.set_kill_count(pData->score);
    msg.add_is_take_reward(pData->isTakeReward);
    Country::GetRankReward(player, LOOTTYPE_ACT_TOURNAMENT, pData->rank).SaveTo(*msg.add_rank_reward());
}
bool ActTournament::Begin(int lastSecond)
{
    if (m_isBegined) return false;

    //没领的奖励自动发给玩家
    for (std::map<uint64, TaskPlayer>::iterator it = m_playerLst.begin(); it != m_playerLst.end(); ++it)
    {
        if (!it->second.isTakeReward)
        {
            if (Player* palyer = sPlayerPool.GetByPlayerId(it->first))
            {
                TakeReward(*palyer);
            }
        }
    }
    m_playerLst.clear();
    m_RankLst.Clear();

    m_isBegined = true;
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        m_countryRank[i].amountScore = 0;
    }
    return true;
}

bool ActCountryRankCmp(const ActTournament::TaskCountry& hl,const ActTournament::TaskCountry& hr)
{
	return hl.amountScore > hr.amountScore;
}

void ActTournament::End()
{
    m_isBegined = false;

	std::sort(m_countryRank, m_countryRank + MAX_PLAYER_COUNTRY_ID,ActCountryRankCmp);

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
		if (Country* pCountry = sWorldMap.GetCountry(m_countryRank[i].countryId))
		{
			pCountry->AddCountryExp(Reward_Country_Exp[i]);
		}
    }
}
bool ActTournament::Fighting(Player& player, vector<uint32>& heroIds, uint32 selectIdx)
{
    if (selectIdx >= Select_MaxCnt) return false;

    TaskPlayer* pData = GetPlayerData(player);
    if (pData == NULL) return false;

    std::set<Combat::CombatObj*> defence;
    if (Player* pTemp = sPlayerPool.GetByPlayerId(pData->selectLst[selectIdx]))
    {
        struct _Temp_Hero_Func //辅助函数对象
        {
            std::set<Combat::CombatObj*>& _lst;

            _Temp_Hero_Func(std::set<Combat::CombatObj*>& lst) : _lst(lst) {}
            bool operator()(const Character* p)
            {
                _lst.insert(p->CreateShadow());
                return true;
            }
        };
        _Temp_Hero_Func objFunc(defence);
        pTemp->m_characterStorage->ForEachBattleCharacter(objFunc);
    }
    MMO_ASSERT(defence.size() > 0);

    std::vector<Combat::CombatObj*> attack;
    for (std::vector<uint32>::iterator it = heroIds.begin(); it != heroIds.end(); ++it)
    {
        Character* pHero = player.m_characterStorage->MutableCharacter(*it);
        MMO_ASSERT(pHero);
        if (pHero) attack.push_back(pHero);
    }

    if (Combat::CombatGroup* pGroup = Combat::CombatGroup::CreateGroup(attack, defence, Combat::CT_Activity_Tournament,System::Rand(1,6)))
    {
        //战斗结束回调OnStageEnd
        pGroup->Watch(player.GetGuid());
        pb::GS2C_Combat_Pre_Begin msg;
        pGroup->SaveTo(msg);
		msg.set_isautocombat(player.IsAutoCombat());
        player.Send(pb::SMSG_COMBAT_PRE_BATTLE, msg);
        pGroup->AddCallBack_OnEnd(boost::bind(&ActTournament::OnFightingEnd, this, &player, pData, selectIdx, _1, _2));
        return true;
    }
    return false;
}
void ActTournament::OnFightingEnd(Player* player, TaskPlayer* pData, uint32 selectIdx, Combat::CombatGroup* pGroup, bool isAttackWin)
{
    if (isAttackWin)
    {
        AddScore(*player, Select_Win_Score[selectIdx]);

        if (Player* pTemp = sPlayerPool.GetByPlayerId(pData->selectLst[selectIdx]))
        {
            AddScore(*pTemp, -1);
        }
    }
    else
    {
        AddScore(*player, Select_Fail_Score);
    }

	struct _Temp_Hero_Func //辅助函数对象
	{
		bool operator()(Character* p)
		{
			p->CalculateAttr();
			return false;
		}
	};
	_Temp_Hero_Func objFunc;
	player->m_characterStorage->ForEachBattleCharacterInCombat(objFunc,pGroup->m_unique_id);

}
void ActTournament::AddScore(Player& player, int score)
{
    uint64 playerId = player.GetGuid();
    TaskPlayer& obj = m_playerLst[playerId];
    obj.playerId = playerId;
    obj.score += score;
    m_RankLst.OnValueChange(obj);

    AddCountryScore(player.GetCountryId(), score);
}
ActTournament::TaskPlayer* ActTournament::GetPlayerData(Player& player)
{
    std::map<uint64, TaskPlayer>::iterator it = m_playerLst.find(player.GetGuid());
    if (it == m_playerLst.end()) return NULL;

    return &it->second;
}
uint32 ActTournament::AddCountryScore(int countryId, int num)
{
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        if (countryId == m_countryRank[i].countryId)
        {
            return m_countryRank[i].amountScore += num;
        }
    }
    return 0;
}
bool ActTournament::TakeReward(Player& player)
{
    if (m_isBegined) return false;

    if (TaskPlayer* pData = GetPlayerData(player))
    {
        if (pData->isTakeReward) return false;

        pData->isTakeReward = true;

        Country::TakeRankReward(player, LOOTTYPE_ACT_TOURNAMENT, pData->rank);

        return true;
    }
    return false;
}

void ActTournament::TakeAwardsBeforeStart()
{

}

void ActTournament::RandOpponent(Player& player)
{
    uint64 playerId = player.GetGuid();

    TaskPlayer& playerData = m_playerLst[playerId];

    // 填充三名玩家
    bool succeed = (m_RankLst.GetLastRank() > Select_MaxCnt) ? 
        RandFormRankLst(playerData) : 
        RandFormPool(playerData, playerId);
    if (succeed) {
        //TODO：通知client
    }
}
bool ActTournament::RandFormPool(TaskPlayer& playerData, uint64 exceptPlayerId)
{
    if (sPlayerPool.GetPlayerCount() <= Select_MaxCnt)
    {
        return false;
    }
    std::vector<uint64> vecData;
	FOREACH_PLAYERPTR(PlayerId, playerPtr)
	{
		if (PlayerId == exceptPlayerId) continue;
		vecData.push_back(PlayerId);
	}
    std::random_shuffle(vecData.begin(), vecData.end());

    for (int i = 0; i < Select_MaxCnt; ++i)
    {
        playerData.selectLst[i] = vecData[i];
    }
    return true;
}


// [left, right)
static int g_limit = 0;
#define Length(l, r)        (r - l)
#define Range_Check(n) if (n < 1) n = 1; if (n > g_limit) n = g_limit;
static void _Right_Check(int& l, int& r)
{
    if (Length(l, r) < Select_MaxCnt)
    {
        r += Select_MaxCnt - Length(l, r);
        Range_Check(r);
    }
}
static void _Left_Check(int& l, int& r)
{
    if (Length(l, r) < Select_MaxCnt)
    {
        l -= Select_MaxCnt - Length(l, r);
        Range_Check(l);
    }
}
int ActTournament::_RandFormRankLst(int left, int right)
{
    Range_Check(right);
    Range_Check(left);

    _Right_Check(left, right);
    _Left_Check(left, right);

    return OS::Rand(left, right);
}
#undef Length
#undef Range_Check

static int _Rank_Check(int retRank, int selfRank, int* retRankLst)
{
    const int tmpRetRank = retRank;

    while (retRank < g_limit) {
        if (IS_IN_ARR(retRankLst, Select_MaxCnt, retRank) || retRank == selfRank) {
            ++retRank;
        } else {
            return retRank;
        }
    }
    retRank = tmpRetRank;

    while (retRank >= 1) {
        if (IS_IN_ARR(retRankLst, Select_MaxCnt, retRank) || retRank == selfRank) {
            --retRank;
        } else {
            return retRank;
        }
    }
	return retRank;
}
bool ActTournament::RandFormRankLst(TaskPlayer& playerData)
{
    int lastRank = m_RankLst.GetLastRank();

    if (lastRank <= Select_MaxCnt) return false;

    g_limit = lastRank + 1;

    int retRank = 0;
    int selfRank = playerData.rank;
    int retRankLst[Select_MaxCnt] = { 0 };

    retRank = _RandFormRankLst(1, selfRank - 5);
    retRankLst[2] = _Rank_Check(retRank, selfRank, retRankLst);

    retRank = _RandFormRankLst(selfRank - 5, selfRank + 5);
    retRankLst[1] = _Rank_Check(retRank, selfRank, retRankLst);

    retRank = _RandFormRankLst(selfRank + 5, lastRank + 1);
    retRankLst[0] = _Rank_Check(retRank, selfRank, retRankLst);

    std::sort(retRankLst, retRankLst + Select_MaxCnt);
    for (int i = 0; i < Select_MaxCnt; ++i)
    {
        if (TaskPlayer* ptr = m_RankLst.GetRanker(retRankLst[i]))
        {
            playerData.selectLst[i] = ptr->playerId;
        }
    }
    return false;
}