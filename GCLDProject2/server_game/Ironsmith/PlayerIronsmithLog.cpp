#include "PlayerIronsmithLog.h"
#include "Table/IronsmithTableMgr.h"
#include "datastruct/struct_smithy.h"
#include "OS.h"
#include "../reward/reward.h"
#include "../object/Player.h"
#include "Smithy.pb.h"
#include "Opcode.pb.h"
#include "../item/ItemManager.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"


PlayerIronsmithLog::PlayerIronsmithLog()
    : m_level(0)
    , m_SilverCoinForBuild(0)
{
    m_subSmithyLst.resize(sSmithyTableMgr.GetMaxFields());
}
void PlayerIronsmithLog::SaveTo(pb::GS2C_Smithy_All_Info& msg)
{
    msg.set_levle(m_level);
    msg.set_silver_coin(m_SilverCoinForBuild);
    for (std::vector<SubSmelter>::iterator it = m_subSmithyLst.begin(); it != m_subSmithyLst.end(); ++it)
    {
        pb::SubSmelter* data = msg.add_sub_smelter();
        data->set_star(it->star);
        data->set_smelt_times(it->smeltTimes);
    }
}
void PlayerIronsmithLog::LoadFrom(const pb::GS2C_Smithy_All_Info& msg)
{
    m_level = msg.levle();
    m_SilverCoinForBuild = msg.silver_coin();
    for (int i = 0; i < msg.sub_smelter_size(); ++i) {
        const pb::SubSmelter& data = msg.sub_smelter(i);
        m_subSmithyLst[i].star = data.star();
		m_subSmithyLst[i].smeltTimes = data.smelt_times();
    }
}
bool PlayerIronsmithLog::DoSmelter(Player& player, uint idx, int itemId /* = 0 */)
{
    if (idx > m_subSmithyLst.size()) return false;

	if (!IsSubSmelterOpen(idx)) return false;

    SubSmelter& info = m_subSmithyLst[idx - 1];

    if (const DB_Smelter* table = sSmithyTableMgr.GetSmelter(idx, info.star))
    {
        if (info.smeltTimes >= table->smelt_times)
        {
            return false;
        }
        int ironNum = 0;

        if (table->need_item > 0 && sReward.Change(player, itemId, -(int)table->need_item_num))
        {
            ironNum = table->get_iron_num;
        }
        else if (const LogicItemProto* proto = sItemMgr.Find(itemId))
        {
            if (proto->IsAcc() && sReward.Change(player, itemId, -(int)table->need_item_num))
            {
                switch (info.star) {
                case 1: ironNum = proto->value01; break;
                case 2: ironNum = proto->value02; break;
                case 3: ironNum = proto->value03; break;
                default: MMO_ASSERT(0); break;
                }
            }
        }
        if (sReward.Change(player, Reward::Iron, ironNum))
        {
            ++info.smeltTimes;
			UpdateSmeltTimesInfoToDb(player);
			player.UpdateAllDailyQuestInfo(TIEJIANGPU,ironNum,0);//每日任务
            return true;
        }
    }
    return false;
}
void PlayerIronsmithLog::SubSmelterLevelUp(Player& player, uint idx)
{
    if (idx > m_subSmithyLst.size()) return;

	if (!IsSubSmelterOpen(idx)) return;

    SubSmelter& info = m_subSmithyLst[idx - 1];

    if (const DB_Smelter* table = sSmithyTableMgr.GetSmelter(idx, info.star))
    {
        if (sReward.Change(player, Reward::Coin_Gold, -(int)table->levelup_need_gold))
        {
            ++info.star;
			UpdateSmeltTimesInfoToDb(player);
        }
    }
}
void PlayerIronsmithLog::LevelUpToOpenSubSmelter(Player& player)
{
    if ((uint32)m_level >= sSmithyTableMgr.GetMaxSmithyLv())
    {
        return;
    }
    if (const DB_Smithy* table = sSmithyTableMgr.GetSmithy(m_level))
    {
        if (player.GetLevel() < (int32)table->need_level)
        {
            return;
        }
        if (sReward.Change(player, table->levelup_need_item, -(int)table->levelup_need_item_num))
        {
            ++m_level;
			UpdateSmeltTimesInfoToDb(player);
        }
    }
}

void PlayerIronsmithLog::DoZeroResetSmeltTimes(Player& player)
{
	for (std::vector<SubSmelter>::iterator it = m_subSmithyLst.begin(); it != m_subSmithyLst.end(); ++it)
	{
		it->smeltTimes = 0;
	}
	UpdateSmeltTimesInfoToDb(player);
}

void PlayerIronsmithLog::UpdateSmeltTimesInfoToDb( Player& player )
{
	pb::GS2C_Smithy_All_Info msg;
	SaveTo(msg);
	sDbProxyClient.SendPlayerPack(player, pb::SG2D_UPDATE_SMITHY, msg);
}

void PlayerIronsmithLog::BuildByToken(Player& player)
{
    if (m_level == 0)
    {
        const DB_Smithy* table = sSmithyTableMgr.GetSmithy(m_level);
        if (sReward.Change(player, table->levelup_need_item, -(int)table->levelup_need_item_num))
        {
            ++m_level;
			UpdateSmeltTimesInfoToDb(player);
        }
    }
}
void PlayerIronsmithLog::BuildBySilver(Player& player, bool isFast)
{
    if (m_level == 0)
    {
        const DB_Smithy* table = sSmithyTableMgr.GetSmithy(m_level);
        int costNum = 0;
        if (isFast)
        {
            costNum = table->levelup_need_candidate_item_num - m_SilverCoinForBuild;
        }
        else
        {
            costNum = table->levelup_need_candidate_item_num / 10;
        }
        if (sReward.Change(player, table->levelup_need_candidate_item, -costNum))
        {
            m_SilverCoinForBuild += costNum;

            if (m_SilverCoinForBuild >= (int)table->levelup_need_candidate_item_num) {
                m_SilverCoinForBuild = 0;
                ++m_level;
            }
			UpdateSmeltTimesInfoToDb(player);
        }
    }
}
bool PlayerIronsmithLog::IsSubSmelterOpen(int idx)
{
	return m_level >= idx;
}