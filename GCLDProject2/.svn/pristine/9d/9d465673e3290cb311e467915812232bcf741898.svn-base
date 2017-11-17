#include "PlayerAltarLog.h"
#include "Table/AltarTableMgr.h"
#include "datastruct/struct_altar.h"
#include "OS.h"
#include "../reward/reward.h"
#include "Altar.pb.h"
#include "../object/Player.h"
#include "Opcode.pb.h"
#include "../CrossLogic/PlayerAltarLogDB.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"

PlayerAltarLog::PlayerAltarLog()
    : m_level(0)
    , m_SilverCoinForBuild(0)
{
    m_subAltarLst.resize(sAltarTableMgr.GetMaxAltarLv());
}
void PlayerAltarLog::SaveTo(pb::GS2C_Altar_All_Info& msg)
{
    msg.set_levle(m_level);
    msg.set_silver_coin(m_SilverCoinForBuild);
    for (std::vector<SubAltar>::iterator it = m_subAltarLst.begin(); it != m_subAltarLst.end(); ++it)
    {
        if (!sOS.IsToday(it->studyEndTime))
        {
            it->studyTimes = 0;
        }
        pb::SubAltar* data = msg.add_sub_altar();
        data->set_is_level_up(it->isBuildingUp);
        data->set_have_reward(it->haveReward);
        data->set_study_times(it->studyTimes);
        data->set_last_study_time(it->studyEndTime);
        data->set_take_free_time(it->takeFreeTime);
        data->set_reward_count(it->rewardCnt);
    }
}
void PlayerAltarLog::LoadFrom(const pb::GS2C_Altar_All_Info& msg)
{
    m_level = msg.levle();
    m_SilverCoinForBuild = msg.silver_coin();
    
    SubAltar tmp;
    for (int i = 0; i < msg.sub_altar_size(); ++i) {
        const pb::SubAltar& data = msg.sub_altar(i);
        tmp.isBuildingUp = data.is_level_up();
        tmp.haveReward = data.have_reward();
        tmp.studyTimes = data.study_times();
        tmp.studyEndTime = data.last_study_time();
        tmp.takeFreeTime = data.take_free_time();
        tmp.rewardCnt = data.reward_count();
        m_subAltarLst[i] = tmp;
    }
}

//研究
void PlayerAltarLog::Study(Player& player, uint altarLv)
{
	if (m_level < (int)altarLv) return;

    if (const AltarTable* table = sAltarTableMgr.GetAltar(altarLv))
    {
        uint64 timeNow = sOS.TimeSeconds();
        SubAltar& info = m_subAltarLst[altarLv-1];
        if (!sOS.IsToday(info.studyEndTime))
        {
            info.studyTimes = 0;
        }
        if (info.studyTimes >= table->m_db.study_times)
        {
            return;
        }
        if (timeNow < info.studyEndTime)
        {
            return;
        }

		if(info.haveReward)
			return;

        if (sReward.Change(player, Reward::Wood, -DecLeaveUpCostWood(table->m_db.study_cost)))
        {
            ++info.studyTimes;
            info.haveReward = true;
			uint32 study_cd = table->m_db.study_cd;

			if(player.HaveTechnology(JINGDAXISUAN))//幻影研究时间减少50% //比率50%
			{
				const TecTable* tec = sTechnologyTableMgr.GetTable(JINGDAXISUAN);
				study_cd -= study_cd * tec->Value1()/100;
			}

            info.studyEndTime = timeNow + study_cd;
            info.rewardCnt = table->RandAddSoldierTimes(m_level);
            PlayerAltarLogDB::SendAllInfoToDb(player, *this);

            pb::GS2C_Altar_Study_Reward msgClient;
            msgClient.set_altar_lv(altarLv);
            msgClient.set_reward_num(info.rewardCnt);
            player.Send(pb::SMSG_ALTAR_TAKE_REWARD, msgClient);
        }
    }
}

//领取研究结果
void PlayerAltarLog::TakeReward(Player& player, uint altarLv)
{
    if (sAltarTableMgr.GetAltar(altarLv))
    {
        uint64 timeNow = sOS.TimeSeconds();
        SubAltar& info = m_subAltarLst[altarLv-1];
        if (info.haveReward && timeNow >= info.studyEndTime)
        {
			int maxnum = player.GetAttrInt(pb::PLAYER_FLIELD_TROOPS_ORDER_LIMIT);
			int nownum = player.GetAttrInt(pb::PLAYER_FIELD_TROOPSNUM);
			//int addnum = (int)info.rewardCnt;

			if(maxnum >= nownum)
			{
				info.haveReward = false;
				sReward.Change(player, Reward::AddSoldierTimes, info.rewardCnt);
				player.UpdateAllDailyQuestInfo(ALTAR_GET_ADD_TIMES,info.rewardCnt,0);//每日任务
				info.rewardCnt = 0;
				PlayerAltarLogDB::SendAllInfoToDb(player, *this);
			}
        }
    }
}

bool PlayerAltarLog::isAllAwardsCanTake(Player& player)
{
	int addnum = 0;
	uint64 timeNow = sOS.TimeSeconds();
	uint32 maxLv = sAltarTableMgr.GetMaxAltarLv();

	for (uint32 i = 1; i <= maxLv; ++i)
	{
		SubAltar& info = m_subAltarLst[i-1];
		if (info.haveReward && timeNow >= info.studyEndTime)
		{
			addnum += info.rewardCnt;
		}
	}

	int maxnum = player.GetAttrInt(pb::PLAYER_FLIELD_TROOPS_ORDER_LIMIT);
	int nownum = player.GetAttrInt(pb::PLAYER_FIELD_TROOPSNUM);
	if(maxnum >= nownum + addnum)
		return true;
	else
		return false;

}

//子祭坛升级
void PlayerAltarLog::SubAltarLevelUp(Player& player, uint altarLv)
{
    if (const AltarTable* table = sAltarTableMgr.GetAltar(altarLv))
    {
        SubAltar& info = m_subAltarLst[altarLv - 1];
        if (info.isBuildingUp == false)
        {
            if (sReward.Change(player, Reward::Coin_Gold, -(int)table->m_db.building_up_cost))
            {
                info.isBuildingUp = true;
                PlayerAltarLogDB::SendAllInfoToDb(player, *this);
            }
        }
        else
        {
            if (!sOS.IsToday(info.takeFreeTime))
            {
				int maxnum = player.GetAttrInt(pb::PLAYER_FLIELD_TROOPS_ORDER_LIMIT);
				int nownum = player.GetAttrInt(pb::PLAYER_FIELD_TROOPSNUM);
				//int addnum = (int)table->m_db.free_add_times;

				if(maxnum >= nownum)
				{
					info.takeFreeTime = sOS.TimeSeconds();
					sReward.Change(player, Reward::AddSoldierTimes, table->m_db.free_add_times);
					player.UpdateAllDailyQuestInfo(ALTAR_GET_ADD_TIMES,table->m_db.free_add_times,0);
				}
            }
        }
    }
}

//祭坛升级，开启子祭坛
void PlayerAltarLog::LevelUpToOpenSubAltar(Player& player)
{
    if ((uint32)m_level >= sAltarTableMgr.GetMaxAltarLv())
    {
        return;
    }
    if (const AltarTable* table = sAltarTableMgr.GetAltar(m_level))
    {
        if (player.GetLevel() < (int32)table->m_db.need_level)
        {
            return;
        }
        if (sReward.Change(player, table->m_db.level_cost_typ, -(int)table->m_db.level_cost_num))
        {
            ++m_level;
            PlayerAltarLogDB::SendAllInfoToDb(player, *this);

            IncAddSoldierTimesMax(player);
        }
    }
}

//立即建造	使用券
void PlayerAltarLog::BuildAltar_Token(Player& player)
{
    if (m_level == 0)
    {
        const AltarTable* table = sAltarTableMgr.GetAltar(m_level);
        if (sReward.Change(player, table->m_db.level_cost_typ, -(int)table->m_db.level_cost_num))
        {
            ++m_level;
            PlayerAltarLogDB::SendAllInfoToDb(player, *this);
        }
    }
}

//快速建造，普通建造	使用银币
void PlayerAltarLog::BuildAltar_Silver(Player& player, bool isFast)
{
    if (m_level == 0)
    {
        const AltarTable* table = sAltarTableMgr.GetAltar(m_level);
        int costNum = 0;
        if (isFast)
        {
            costNum = table->m_db.level_cost_num2 - m_SilverCoinForBuild;
        }
        else
        {
            costNum = table->m_db.level_cost_num2 / 10;
        }
        if (sReward.Change(player, table->m_db.level_cost_typ2, -costNum))
        {
            m_SilverCoinForBuild += costNum;

            if (m_SilverCoinForBuild >= (int)table->m_db.level_cost_num2) {
                m_SilverCoinForBuild = 0;
                ++m_level;
            }
            PlayerAltarLogDB::SendAllInfoToDb(player, *this);
        }
    }
}
void PlayerAltarLog::IncAddSoldierTimesMax(Player& player)
{
    const int Special_Level = 3;
    if (m_level == Special_Level)
    {
        sAltarTableMgr.GetAltar(Special_Level)->m_db.special_effect;
        //免费增兵累计上限增加
		player.AddValue(pb::PLAYER_FLIELD_TROOPS_ORDER_LIMIT,sAltarTableMgr.GetAltar(Special_Level)->m_db.special_effect);
    }
}
int PlayerAltarLog::DecLeaveUpCostWood(int costNum)
{
    const int Special_Level = 2;
    if (m_level >= Special_Level)
    {
        return costNum * sAltarTableMgr.GetAltar(Special_Level)->m_db.special_effect / 100;
    }
    return costNum;
}

