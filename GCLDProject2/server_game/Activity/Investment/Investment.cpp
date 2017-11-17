#include "Investment.h"
#include "OS.h"
#include "Base.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "../Table/ActTableMgr.h"
#include "../../reward/reward.h"
#ifdef _MMO_SERVER_
#include "../../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "../../Map/country/country.h"
#include "algorithm"
#include "../../item/ItemArray.h"
#include "../../Loot/LootManager.h"
#include "../../Loot/LootList.h"
#include "../../session/PlayerPool.h"
#include "Country.pb.h"
#include "Map/WorldMap.h"
#include "Activity/Personal/PersonalTask.h"
#include "BaseDefine.pb.h"
#ifdef  _MMO_SERVER_
#include "../../GameServer.h"
#endif
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossDestroy/CrossDestroy.h"
#endif
#include "../ActMgr.h"
#include "../Personal/PersonalTask.h"

static const int Ex_Reward_Id = 9999; //见loot表配置

Investment::TaskPlayer::TaskPlayer()
{
    ZeroMemoryThis;
}

Investment::Investment()
{
    m_isBegined = false;
    //m_haveExReward = false;
    m_curStage = 0;
    m_curStageBeginTime = 0;

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i) {
        m_countryRank[i].countryId = i + 1;
        m_countryRank[i].amountMoney = 0;
    }
	m_playerInvestTotal.clear();
}
void Investment::SaveTo(pb::GS2C_Country_Task_Insvest& msg)
{
	LLOG("[1003]SaveTo GS2C_Country_Task_Insvest m_isBegined=%d, m_curStage=%d, m_curStageBeginTime=%lld",m_isBegined,m_curStage,m_curStageBeginTime);
    msg.set_is_begin(m_isBegined);
    msg.set_cur_stage(m_curStage);
    msg.set_cur_stage_begin_time(m_curStageBeginTime);

    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        msg.add_cur_stage_now_country_invest_money(m_countryRank[i].amountMoney);
    }
}
void Investment::SaveTo(pb::GS2C_Country_Invest_Player& msg, Player& player)
{
	for(int stagenum = m_curStage; stagenum > 0; stagenum--)
	{
		if(isPlayerInList(stagenum,player))
		{
			LLOG("SaveTo GS2C_Country_Invest_Player stagenum=%d, m_curStage=%d",stagenum,m_curStage);
			TaskPlayer& pData = GetPlayerData(stagenum,player);
			uint32& myTotal = m_playerInvestTotal[player.GetGuid()];
			int rank = sActTableMgr.GetInvestTitleId(pData.score);
			msg.set_stageid(stagenum);
			msg.set_donatenum(pData.score);
			msg.set_cd_time(pData.cdTime);
			msg.set_is_take_reward(pData.isTakeReward);
			msg.set_cantakeexreward(pData.canTakeExReward);
			msg.set_total_donate(myTotal);
			Country::GetRankReward(player, LOOTTYPE_ACT_INVESTMENT, rank).SaveTo(*msg.mutable_rank_reward());
			Country::GetWinReward(player, LOOTTYPE_ACT_INVESTMENT).SaveTo(*msg.mutable_win_reward());
			return;
		}
	}

    //int rank = sActTableMgr.GetInvestTitleId(pData.score);
    //msg.set_rankid(rank);
    //msg.set_donatenum(pData.score);
    //msg.set_cd_time(pData.cdTime);
    //msg.add_is_take_reward(pData.isTakeReward);
    //Country::GetRankReward(player, LOOTTYPE_ACT_INVESTMENT, rank).SaveTo(*msg.add_rank_reward());
    //Country::GetWinReward(player, LOOTTYPE_ACT_INVESTMENT).SaveTo(*msg.mutable_win_reward());
}
bool Investment::Begin(int lastSecond)
{
    if (m_isBegined) return false;		//任务还没结束就返回

    m_isBegined = true;		//任务开始
    //m_haveExReward = false;	//是否有额外奖励
    m_curStage = 0;			//当前任务阶段
    m_curStageBeginTime = 0;//当前阶段开始时间
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        m_countryRank[i].amountMoney = 0;		//三个国家分别总投资
    }
	m_playerInvestTotal.clear();
	m_playerLst.clear();
    _EnterNextStage();	//进入第一个阶段
#ifdef _MMO_SERVER_
    sAutoUpdateGame.AddEvent(boost::bind(&Investment::End, this), lastSecond);		//活动结束
#endif

	////发送开始消息
	//pb::GS2C_Country_Task_Insvest sendmsg;
	//sendmsg.set_is_begin(m_isBegined);

	//TaskPlayerList& m_curPlayerLst = m_playerLst[m_curStage];
	//for (TaskPlayerList::iterator it = m_curPlayerLst.begin(); it != m_curPlayerLst.end(); ++it)
	//{
	//	TaskPlayer& data = it->second;
	//	if (Player* palyer = sPlayerPool.GetByPlayerId(data.playerId))
	//	{
	//		palyer->Send(pb::SMSG_COUNTRY_TASK_INVEST,sendmsg);
	//	}
	//}

	return true;
}

bool CountryRankCmp(const Investment::TaskCountry& hl,const Investment::TaskCountry& hr)
{
	return hl.amountMoney > hr.amountMoney;
}

void Investment::End()
{
    m_isBegined = false;

	sPersonTask.End();

#ifdef _MMO_SERVER_
	pb::GS2C_Country_Task_Insvest sendmsg;
	SaveTo(sendmsg);
	sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_INVEST, sendmsg);
#endif

  //  std::sort(m_countryRank, m_countryRank + MAX_PLAYER_COUNTRY_ID,CountryRankCmp);

  //  for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
  //  {
		//if (Country* pCountry = sWorldMap.GetCountry(m_countryRank[i].countryId))
		//{
		//	pCountry->AddCountryExp(Reward_Country_Exp[i]);
		//}
  //  }

    //if (GetCountryMoney(m_countryRank[0].countryId) >= 3*GetCountryMoney(m_countryRank[1].countryId))
    //{
    //    m_haveExReward = true;
    //}
}


//进入下一个阶段 check
void Investment::_EnterNextStage()
{
	LLOG("Investment::_EnterNextStage now stage = %d",m_curStage);
	//确认国家奖励能否领取
	if (const DB_CountryInvest* table = sActTableMgr.GetInvest(m_curStage))
	{
		for (int i = 1; i <= MAX_PLAYER_COUNTRY_ID; ++i)
		{
			if(GetCountryMoney(i) >= table->invest_upper)		//达到投资上限
			{
				TaskPlayerList& m_curPlayerLst = m_playerLst[m_curStage];
				for (TaskPlayerList::iterator it = m_curPlayerLst.begin(); it != m_curPlayerLst.end(); ++it)
				{
					TaskPlayer& data = it->second;
					if(data.countryId == i)
						data.canTakeExReward = true;		//该国家的玩家能够领取额外奖励
				}

				if (Country* country = sWorldMap.GetCountry(i))
				{
					country->AddCountryExp(table->country_exp);		//国家经验增加
				}
			}
		}
	}

    m_curStage += 1;		//阶段数
    m_curStageBeginTime = OS::TimeSeconds();	//阶段开始时间

#ifdef _MMO_SERVER_
    if (const DB_CountryInvest* table = sActTableMgr.GetInvest(m_curStage))		//有下一个阶段则进入下个阶段
    {

#ifdef _MMO_SERVER_
		//发送阶段消息
		pb::GS2C_Country_Task_Insvest sendmsg;
		SaveTo(sendmsg);
		sGameSvr.BroadcastMsgToPlayer(pb::SMSG_COUNTRY_TASK_INVEST, sendmsg);
#endif

        sAutoUpdateGame.AddEvent(boost::bind(&Investment::_EnterNextStage, this), table->stage_last_time);		//阶段结束时间到了自动进入下一个阶段
    }
#endif
}

//捐赠 check
bool Investment::GiveMoney(uint32 stage, Player& player, pb::GS2C_Country_Invest_Donate_Ret& msg)
{
	const int countryId = player.GetCountryId();
	msg.set_issuccess(false);
	
	if (const DB_CountryInvest* table = sActTableMgr.GetInvest(m_curStage))		//当前阶段对应的表
	{
		if (GetCountryMoney(countryId) < table->invest_upper)		//小于投资上限，可以继续投资
		{
			if(isPlayerInList(m_curStage,player))
			{
				TaskPlayer& pData = GetPlayerData(m_curStage,player);
				
				if(stage == m_curStage)	//是当前阶段，允许投资，否则先领取
				{
					const uint64 timeNow = OS::TimeSeconds();
					if (pData.cdTime >= timeNow + GET_BASE_DEF_INT( pb::BD_INVEST_MAX_COOLDOWN_TIME))		//cd还没有好
					{
						msg.set_errret(pb::investInCD);
						return false;
					}

					if(pData.cdTime < timeNow)
						pData.cdTime = timeNow;

					if (sReward.Change(player, Reward::Coin_Silver, -(int)table->invest_cost))		//单次投资消耗银币
					{
						int multi = 1;
						if (player.TakeOutItem(pb::IR_ITEM_USED_COST, table->invest_item, 1)){
							multi = 2;
						}
						pData.cdTime += GET_BASE_DEF_INT( pb::BD_INVEST_ADD_COOLDOWN_TIME);			//增加cd时长
						pData.score += table->invest_cost * multi;	//增加投资花费

						sReward.Change(player, Reward::PlayerExp, (int)table->invest_reward * multi);		//获得经验奖励

						AddCountryMoney(countryId, table->invest_cost * multi);

						uint32& myTotal = m_playerInvestTotal[player.GetGuid()];
						myTotal += table->invest_cost * multi;

						msg.set_issuccess(true);
						msg.set_my_invest_money(pData.score);
						msg.set_country_invest_money(GetCountryMoney(countryId));
						msg.set_cd_time(pData.cdTime);
						msg.set_total_donate(myTotal);
						
						pb::GS2C_LootList* lstloot = msg.mutable_successaward();
						lstloot->set_player_exp(table->invest_reward * multi);
						
						sPersonTask.addProgress(INVEST_TIMES,table->invest_cost * multi,player);
#ifdef _SERVER_CROSS_
						sCrossDestroy.addProgress(INVEST_TIMES,table->invest_cost * multi,player);
#endif
						
						return true;
					}
					else
						msg.set_errret(pb::investNoMoney);
				}
				else
					msg.set_errret(pb::investStageNotMatch);
			}
			else
			{
				if(stage == m_curStage)
				{
					if (sReward.Change(player, Reward::Coin_Silver, -(int)table->invest_cost))		//单次投资消耗银币
					{
					
						TaskPlayer& pData = GetPlayerData(m_curStage,player);
						int multi = 1;
						if (player.TakeOutItem(pb::IR_ITEM_USED_COST, table->invest_item, 1))
							multi = 2;
						pData.cdTime = sOS.TimeSeconds() + GET_BASE_DEF_INT( pb::BD_INVEST_ADD_COOLDOWN_TIME);
						pData.score += table->invest_cost * multi;

						sReward.Change(player, Reward::PlayerExp, (int)table->invest_reward * multi);		//获得经验奖励

						AddCountryMoney(countryId, table->invest_cost * multi);

						uint32& myTotal = m_playerInvestTotal[player.GetGuid()];
						myTotal += table->invest_cost * multi;

						msg.set_issuccess(true);
						msg.set_my_invest_money(pData.score);
						msg.set_country_invest_money(GetCountryMoney(countryId));
						msg.set_cd_time(pData.cdTime);
						msg.set_total_donate(myTotal);
						
						pb::GS2C_LootList* lstloot = msg.mutable_successaward();
						lstloot->set_player_exp(table->invest_reward * multi);

						sPersonTask.addProgress(INVEST_TIMES,table->invest_cost * multi,player);
#ifdef _SERVER_CROSS_
						sCrossDestroy.addProgress(INVEST_TIMES,table->invest_cost * multi,player);
#endif

						return true;
					}
					else
						msg.set_errret(pb::investNoMoney);
				}
				else
					msg.set_errret(pb::investStageNotMatch);
			}
		}
		else
		{
			msg.set_errret(pb::investUpperErr);
		}
	}
	else
	{
		msg.set_errret(pb::TaskErr);
	}

    return false;
}

//领取奖励 check
bool Investment::TakeReward(uint32 stage, Player& player, pb::GS2C_Country_Invest_Take_Reward& msg)
{
    //if (m_isBegined) return false;
	msg.set_issuccess(false);

	if(m_curStage <= stage) 
	{
		msg.set_errret(pb::investStageNotMatch);
		return false;		//申请的阶段必须小于当前阶段
	}

	if(isPlayerInList(stage,player))	//在目标阶段投资过
	{
		TaskPlayer& pData = GetPlayerData(stage,player);
        if (pData.isTakeReward) 
		{
			msg.set_errret(pb::rewardHasTaken);
			return false;	//已经领取
		}

        pData.isTakeReward = true;

        Country::TakeRankReward(player, LOOTTYPE_ACT_INVESTMENT, sActTableMgr.GetInvestTitleId(pData.score));

        if (pData.canTakeExReward)
        {
			pData.canTakeExReward = false;
            Country::TakeWinReward(player, LOOTTYPE_ACT_INVESTMENT);
        }
		msg.set_issuccess(true);
        return true;
    }
	else
	{
		msg.set_errret(pb::stageNotInvested);
	}
    return false;
}

bool Investment::ClearInvestCD(Player& player, pb::GS2C_Country_Invest_Clear_CD_Ret& msg)
{
	msg.set_issuccess(false);

	if (const DB_CountryInvest* table = sActTableMgr.GetInvest(m_curStage))		//当前阶段对应的表
	{
		if(isPlayerInList(m_curStage,player))
		{
			TaskPlayer& pData = GetPlayerData(m_curStage,player);
			if(player.TryDeductCurrency(pb::IR_INVEST_CLEAR_CD_COST,eSysGold,GET_BASE_DEF_UINT(pb::BD_CLEAR_INVEST_TIME_COST_GOLD)))
			{
				pData.cdTime = sOS.TimeSeconds();
				msg.set_issuccess(true);
			}
			else
			{
				msg.set_errret(pb::investNoMoney);
			}
		}
		else
			msg.set_errret(pb::NoRecord);
	}
	else
	{
		msg.set_errret(pb::TaskErr);
	}
	return true;
}

//下一个任务开始前发放奖励
void Investment::TakeAwardsBeforeStart()
{
	//没领的奖励自动发给玩家
	for(std::map<uint32, TaskPlayerList>::iterator ite = m_playerLst.begin(); ite != m_playerLst.end(); ite++)
	{
		TaskPlayerList& m_curPlayerLst = ite->second;
		for (TaskPlayerList::iterator it = m_curPlayerLst.begin(); it != m_curPlayerLst.end(); ++it)
		{
			if (!it->second.isTakeReward)
			{
				if (Player* palyer = sPlayerPool.GetByPlayerId(it->first))
				{
					pb::GS2C_Country_Invest_Take_Reward msg;
					TakeReward(ite->first,*palyer,msg);
				}
			}
		}
	}

	m_playerLst.clear();
	m_playerInvestTotal.clear();
	//m_haveExReward = false;	//是否有额外奖励
	m_curStage = 0;			//当前任务阶段
	m_curStageBeginTime = 0;//当前阶段开始时间
	for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
	{
		m_countryRank[i].amountMoney = 0;		//三个国家分别总投资
	}
}

//获取指定国家的投资金额 check
uint32 Investment::GetCountryMoney(int countryId)
{
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        if (countryId == m_countryRank[i].countryId)
        {
            return m_countryRank[i].amountMoney;
        }
    }
    return 0;
}

//增加投资 check
uint32 Investment::AddCountryMoney(int countryId, int money)
{
    for (int i = 0; i < MAX_PLAYER_COUNTRY_ID; ++i)
    {
        if (countryId == m_countryRank[i].countryId)
        {
            return m_countryRank[i].amountMoney += money;
        }
    }
    return 0;
}

//根据玩家获得任务玩家信息 check
Investment::TaskPlayer& Investment::GetPlayerData(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
    std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
    if (it == playerlststage.end())
	{
		TaskPlayer& playerone = playerlststage[player.GetGuid()];
		playerone.playerId = player.GetGuid();
		playerone.countryId = player.GetCountryId();
		playerone.cdTime = 0;
		playerone.isTakeReward = false;
		playerone.canTakeExReward = false;
		playerone.score = 0;

		sActMgr.AddPlayerInActivity(player.GetGuid());

		return playerone;
	}

    return it->second;
}

//玩家是否在当前阶段的队列中 check
bool Investment::isPlayerInList(uint32 stage, Player& player)
{
	TaskPlayerList& playerlststage = m_playerLst[stage];
	std::map<uint64, TaskPlayer>::iterator it = playerlststage.find(player.GetGuid());
	if (it != playerlststage.end())
	{
		return true;
	}
	return false;
}