#include "FubenLog.h"
#include "Fuben.pb.h"
#include "FubenDataMgr.h"
#include "FubenStage.h"
#include "Fuben.h"
#include "OS.h"
#include "../object/Player.h"
#include "../reward/reward.h"
#include "../Combat/CombatGroup.h"
#include "Opcode.pb.h"
#include "../CrossLogic/PlayerFuBenDB.h"
#include "../Npc/NpcMgr.h"
#include "../Npc/Npc.h"
#include "../character/CharacterStorage.h"
#include "Combat.pb.h"
#include "../session/PlayerPool.h"
#include "../Trigger/trigger.h"
#include "utility/Utility.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
//#include "quest/IQuestLogger.h"
#include "quest/QuestLogger.h"
#include "memory_buffer/NetPack.h"
#include "../CrossLogic/PlayerCharacterLogic.h"
#include "PassStageActivity/ActivtyStage.h"
#include "memory_buffer/NetPack.h"
#include "BaoQi/PlayerBaoQiLog.h"
#include "SysEvent/SysEventMgr.h"
#ifdef _MMO_SERVER_
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif
using namespace pb;
#define FIRST_FUBEN_ID 101
FubenLog::FubenLog() 
    : m_curFubenID(0)		//当前副本
    , m_curStageID(0)		//当前关卡
{
	::memset(NR_stars_,0,sizeof(NR_stars_));
	FillStageInBranchAndHide();
}
bool FubenLog::Init()
{
	if (m_curFubenID == 0 || m_curStageID == 0)
	{
		m_curFubenID = sFubenDataMgr.GetFirstFuben().Id();	//101	黑暗之音
		m_curStageID = sFubenDataMgr.GetFirstStage().Id();	//11	泰伦星球
		return true;
	}
	return false;
}


CrusadeLog::CrusadeLog()
:currCrusade(0)
,currCruStage(0)
,isCruDead(0)
,restReset(GET_BASE_DEF_UINT(pb::BD_EXPEDITION_RESET_TIMES))
,addHeroTimes(GET_BASE_DEF_UINT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES))
,award_state(0)
,isCrusadeOver(0)
{
	heroAddTimes.clear();
	heroAddTimes.resize(CHARACTER_COUNT,GET_BASE_DEF_UINT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES_PERTROOP));
	heroAddTimesBackup.clear();
}

void CrusadeLog::Reset()
{
	currCruStage = 0;//fuben Id
	currCrusade = 0;//stage Id
	isCruDead = 0;	//当前远征副本是否打过
	restReset = GET_BASE_DEF_UINT(pb::BD_EXPEDITION_RESET_TIMES);	//剩余重置次数
	//addHeroTimes = GET_BASE_DEF_INT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES);	//填充武将次数
	award_state = 0;
	isCrusadeOver = 0;
	heroAddTimes.assign(heroAddTimes.size(),GET_BASE_DEF_UINT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES_PERTROOP));
	heroAddTimesBackup.clear();
}

void CrusadeLog::AddHeroAddTimes(uint32 times)
{
	addHeroTimes += times;
}

//将关卡信息保存到消息中待发送
void FubenLog::SaveTo(Player* player,pb::GS2C_Fuben_All_Info& msg)
{
    msg.set_cur_fuben_id(m_curFubenID);
    msg.set_cur_stage_id(m_curStageID);
	for(size_t i = 0;i < ARRAY_SIZE(NR_stars_);++i)
	{
		msg.add_nr_stars(NR_stars_[i]);
	}

    for (HideFubenMap::const_iterator it = hideFubenMap_.begin();
        it != hideFubenMap_.end(); ++it)
    {
        pb::HideFubenInfo* hide_info = msg.add_hide_fuben_infos();
		SaveTo(it->first,it->second,*hide_info);
    }
    for (PassStageSet::iterator it = m_passStageLog.begin();
        it != m_passStageLog.end(); ++it)
    {
		pb::Stage_Info *info = msg.add_pass_stage_info();
        info->set_stage_type(it->first);
		info->set_pass_count(it->second.pass_count_);
		info->set_is_take_reward(it->second.is_take_rewrd_);
    }
	pb::GS2C_Combat_Crusade* crusadeInfo = msg.mutable_crusade_info();
	SaveTo(*crusadeInfo);
	PlayerFuBenDB::SendCrusadeToDb(*player, *this);

	for(BranchFubenMap::const_iterator iter = branchFubenMap_.begin();iter != branchFubenMap_.end();++iter)
	{
		pb::BranchFubenInfo * info = msg.add_branch_info();
		SaveTo(iter->first,iter->second,*info);
	}
}

//将远征信息保存到消息中
void FubenLog::SaveTo(pb::GS2C_Combat_Crusade& msg)
{
	msg.set_level_id(m_crusadeLog.currCruStage);
	msg.set_stage_state(m_crusadeLog.isCruDead);
	msg.set_rewards_state(m_crusadeLog.award_state);
	msg.set_reset_lefttimes(m_crusadeLog.restReset);
	msg.set_add_lefttimes(m_crusadeLog.addHeroTimes);
	msg.set_is_crusade_over(m_crusadeLog.isCrusadeOver);
	for(IntPairVec::iterator iter = m_crusadeLog.heroAddTimesBackup.begin();
		iter != m_crusadeLog.heroAddTimesBackup.end();++iter)
	{
		pb::PairValue *value = msg.add_chartimelist();
		value->set_first(iter->first);
		value->set_second(iter->second);
	}
}

void FubenLog::SaveTo(pb::GxDB_Combat_Crusade& msg)
{
	msg.set_level_id(m_crusadeLog.currCruStage);
	msg.set_stage_state(m_crusadeLog.isCruDead);
	msg.set_rewards_state(m_crusadeLog.award_state);
	msg.set_reset_lefttimes(m_crusadeLog.restReset);
	msg.set_add_lefttimes(m_crusadeLog.addHeroTimes);
	msg.set_is_crusade_over(m_crusadeLog.isCrusadeOver);
	VecInt::iterator iter = m_crusadeLog.heroAddTimes.begin();
	for(;iter != m_crusadeLog.heroAddTimes.end();++iter)
	{
		pb::PairValue *value = msg.add_chartimelist();
		value->set_first(GetDistance(iter,m_crusadeLog.heroAddTimes));
		value->set_second(*iter);
	}
}


void FubenLog::SaveTo(const uint32 stageId,const BranchBaseInfo& info,pb::BranchFubenInfo& msg)
{
	msg.set_stage_id(stageId);
	msg.set_is_passed(info.is_passed_);
	msg.set_is_take_award(info.is_take_award_);
	msg.set_branch_group_id(info.branch_group_id_);
}

void FubenLog::SaveTo(const uint32 stageId,const HideFubenBaseInfo& info,pb::HideFubenInfo& msg)
{
	msg.set_hide_stage_id(stageId);
	msg.set_free_times(info.free_time_);
	msg.set_kill_monster(info.kill_monsters_);
	msg.set_count_times(info.today_fight_times_);
}

//加载关卡信息到m_mapResourceLog与m_passStageLog
void FubenLog::LoadFrom(const pb::GS2C_Fuben_All_Info& msg)
{
	NLOG("FubenLog::LoadFrom");
    m_curFubenID = msg.cur_fuben_id();	//当前副本id
    m_curStageID = msg.cur_stage_id();	//当前关卡id
	for(int i = 0;i < msg.nr_stars_size();++i)
	{
		if(msg.nr_stars(i) != 0)
		{
			NR_stars_[i] = msg.nr_stars(i);
		}
	}
	if(msg.has_crusade_info_db())
		LoadFrom(msg.crusade_info_db());	//远征信息

	//隐藏关卡
    for (int i = 0; i < msg.hide_fuben_infos_size(); ++i)
    {
		HideFubenBaseInfo logInfo;
        const pb::HideFubenInfo& info = msg.hide_fuben_infos(i);
        logInfo.free_time_ = info.free_times();
        logInfo.kill_monsters_ = info.kill_monster();
		logInfo.today_fight_times_ = info.count_times();
        hideFubenMap_[info.hide_stage_id()] = logInfo;
    }
	
	//已经打过的特殊关 军团关、资源关
    m_passStageLog.clear();
    for (int i = 0; i < msg.pass_stage_info_size(); ++i)
    {
		const pb::Stage_Info &info = msg.pass_stage_info(i);
        stage_info& ref = m_passStageLog[info.stage_type()];
		ref.is_take_rewrd_ = info.is_take_reward();
		ref.pass_count_ = info.pass_count();
    }
	for (int i = 0; i < msg.branch_info_size(); ++i)
	{
		const pb::BranchFubenInfo &info = msg.branch_info(i);
		BranchBaseInfo& ref = branchFubenMap_[info.stage_id()];
		ref.is_passed_ = info.is_passed();
		ref.is_take_award_ = info.is_take_award();
		ref.branch_group_id_ = info.branch_group_id();
	}
}

void FubenLog::LoadFrom(const pb::GxDB_Combat_Crusade& msg)
{
	NLOG("FubenLog::LoadFrom");
	m_crusadeLog.restReset = msg.reset_lefttimes();
	m_crusadeLog.currCruStage = msg.level_id();
	m_crusadeLog.isCruDead = msg.stage_state();
	m_crusadeLog.addHeroTimes = msg.add_lefttimes();
	m_crusadeLog.award_state = msg.rewards_state();
	m_crusadeLog.isCrusadeOver = msg.is_crusade_over();
	
	for(int i = 0;i < msg.chartimelist_size();++i)
	{
		const pb::PairValue& list = msg.chartimelist(i);
		m_crusadeLog.heroAddTimes[list.first()-1] = list.second();
	}

	const FubenStage* pStageTable = sFubenDataMgr.GetStage(m_crusadeLog.currCruStage);
	if(pStageTable)
		m_crusadeLog.currCrusade = pStageTable->FubenId();
	else
		m_crusadeLog.currCrusade = 0;
}

void FubenLog::AddHeroToVector(std::vector<uint32>& vec)
{
	for(std::vector<uint32>::iterator ite = vec.begin(); ite != vec.end(); ++ite)
	{
		if(*ite == 0) continue;
		bool find = false;
		for(IntPairVec::iterator it = m_crusadeLog.heroAddTimesBackup.begin();it != m_crusadeLog.heroAddTimesBackup.end();++it)
		{
			if(*ite == it->first)
			{
				//it->second = m_crusadeLog.heroAddTimes[*ite-1];
				find = true;
			}
		}
		if(!find)
		{
			m_crusadeLog.heroAddTimesBackup.push_back(std::make_pair(*ite,m_crusadeLog.heroAddTimes[*ite-1]));
		}
	}
}

uint32 FubenLog::GetDistance(const VecInt::iterator& iter,const VecInt& vec)
{
	return iter - vec.begin() + 1;
}
///同步backup里面的数据到 heroAddTimes去
void FubenLog::SyncVecs(Player& player)
{
	for(IntPairVec::iterator iter = m_crusadeLog.heroAddTimesBackup.begin();
		iter != m_crusadeLog.heroAddTimesBackup.end();++iter)
	{
		if(iter->second != m_crusadeLog.heroAddTimes[iter->first-1])
		{
			 m_crusadeLog.heroAddTimes[iter->first-1] = iter->second;
		}
	}
	PlayerFuBenDB::SendCrusadeToDb(player, *this);
}

//进入关卡
bool FubenLog::EnterStage(Player& player,const uint32 stageId, std::vector<uint32>& heroIds)
{
	NLOG("FubenLog::EnterStage stageId=%d",stageId);
	const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId);
	if (pStageTable == NULL) return false;

	if(pStageTable->IsCrusadeStage() || pStageTable->IsCruGiftsStage())
	{
		//远征走这一条逻辑
		return EnterCrusadeStage(player,stageId,heroIds);
	}
	else
	{
		//等级不够
		if (player.GetLevel() < (int32)pStageTable->ReqLv()) return false;
		if(pStageTable->IsHideStage())
		{
			if(NR_stars_[m_curFubenID-101] < pStageTable->HideUnlockStar())
			{
				return false;
			}
			HideFubenMap::iterator iter = hideFubenMap_.find(stageId);
			if(iter != hideFubenMap_.end())
			{
				if(iter->second.free_time_ > 0)
				{
					iter->second.free_time_--;
					pb::HideFubenInfo msg;
					SaveTo(stageId,iter->second,msg);
					player.Send(pb::SMSG_UPDATE_HIDE_FUBEN,msg);
				}
				else
				{
					IntPair Cost;
					pStageTable->GetCost(Cost,iter->second.today_fight_times_);
					bool ret = sReward.Change(player,Cost);
					if(!ret) return false;
				}
			}
		}
		//开战，战斗结束回调OnStageEnd
		FightWithNpc(player, pStageTable, heroIds);
		if(pStageTable->IsCommonStage() || pStageTable->IsFogStage())
		{
			player.UpdateAllDailyQuestInfo(FUBEN,1,0);//每日任务
		}
		return true;
	}
}

bool FubenLog::EnterCrusadeStage(Player& player, uint32 stageId, std::vector<uint32>& heroIds)
{
	NLOG("FubenLog::EnterCrusadeStage stageid=%d",stageId);

	//当前请求的关卡
	const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId);

	//当前请求关卡的第一关
	const FubenStage* firstStage = sFubenDataMgr.GetFirstCrusadeStageByFubenID(pStageTable->FubenId());
	if(pStageTable->IsCrusadeStage())//是远征关
	{
		//如果进入的关卡是当前要打的关卡并且当前关卡可以打
		if(stageId == m_crusadeLog.currCruStage && m_crusadeLog.isCruDead == 0)
		{
			//进入远征关，并没有开战，因为队列没有战斗单位
			CombatInCrusade(player, pStageTable, heroIds);
		}
		else
			NLOG("stage not match,currCruStage=%d,iscrudead=%d",m_crusadeLog.currCruStage,m_crusadeLog.isCruDead);
	}
	else if(pStageTable->IsCruGiftsStage())
	{
		pb::GS2C_Crusade_Take_Reward_Rsp msg;
		TakeCrusadeReward(player,stageId,msg);
	}
 	else
	{
		NLOG("bug!!! not crusade stage");
	}
	return true;
}

void FubenLog::RecoverTimes(Player& player,const pb::C2GS_Combat_Crusade_Recover_Times& msg)
{
	for(IntPairVec::iterator ite = m_crusadeLog.heroAddTimesBackup.begin();
		ite != m_crusadeLog.heroAddTimesBackup.end(); ++ite)
	{
		for(int i = 0;i < msg.values_size();++i)
		{
			const pb::PairValue& char_value = msg.values(i);
			if(char_value.first() == ite->first)
			{
				ite->second += char_value.second();
				m_crusadeLog.addHeroTimes += char_value.second(); 
			}
		}
	}
	SyncVecs(player);
}

void FubenLog::SelectCrusade(Player& player, uint32 crusadeid,pb::GS2C_Select_Ret& msg)
{
	NLOG("FubenLog::SelectCrusade,crusadeid=%d",crusadeid);
	const FubenStage* pStageTable = sFubenDataMgr.GetFirstCrusadeStageByFubenID(crusadeid);
	msg.set_isselectsuc(false);
	if(pStageTable)
	{
		if(pStageTable->IsCrusadeStage())	//是远征
		{
			if(m_crusadeLog.restReset >0)		//有剩余重置次数
			{
				NLOG("reset crusade once");
				m_crusadeLog.restReset--;
				m_crusadeLog.addHeroTimes = GET_BASE_DEF_INT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES);
				m_crusadeLog.heroAddTimes.assign(m_crusadeLog.heroAddTimes.size(),GET_BASE_DEF_UINT(pb::BD_EXPEDITION_TOTAL_DRAFT_TIMES_PERTROOP));
				m_crusadeLog.heroAddTimesBackup.clear();
				m_crusadeLog.currCrusade = crusadeid;
				m_crusadeLog.currCruStage = pStageTable->Id();
				m_crusadeLog.isCruDead = 0;
				m_crusadeLog.award_state = 0;
				msg.set_isselectsuc(true);

				pb::GS2C_Combat_Crusade* crusadeinfo = msg.mutable_crusadeinfo();
				SaveTo(*crusadeinfo);

				PlayerFuBenDB::SendCrusadeToDb(player, *this);
				NLOG("reset crusade over");
			}
			else
				NLOG("no more rest time");
		}
		else
			NLOG("not crusade");
	}
	else
		NLOG("fuben is not find");
}

void FubenLog::GotoNextCrusade(CrusadeLog& log,const uint32 stageId)
{
	const FubenStage* firstStage = sFubenDataMgr.GetFirstCrusadeStageByFubenID(log.currCrusade);
	int stagesNum = sFubenDataMgr.GetCurrentCrusadeStageNum(stageId);	//当前所处在的远征总关卡数
	int stageEndID = stagesNum + firstStage->Id() - 1;
	if((int)stageId >= stageEndID)		//通关最后一关
	{
		log.currCruStage = stageEndID;
		log.isCruDead = 1;
		log.isCrusadeOver = 1;
	}
	else
	{
		log.currCruStage = stageId + 1;
		log.isCruDead = 0;
		log.isCrusadeOver = 0;
	}
}

//战斗结束
void FubenLog::OnStageEnd(Player* player, const uint32 stageId,const uint32 npc_hp, Combat::CombatGroup* pGroup, bool isAttackWin)
{
    const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId);
    if (pStageTable == NULL) return;

    IntPairVec reward;
    if (isAttackWin) 
	{
		//记录打过的特殊关卡
		if (pStageTable->IsResourceStage() || pStageTable->IsGroupStage() || pStageTable->IsFogStage())
		{
			stage_info &ref = m_passStageLog[pStageTable->GetStageType()];
			ref.pass_count_++;
			PlayerFuBenDB::SendPassStageToDb(*player, pStageTable->GetStageType(),&ref);
		}
			//给过关奖励
		pStageTable->GetReward(reward);
		sReward.Change(*player, reward);
		player->m_BaoQiLog->UpdateGemAttr(*player);
		std::set<uint32> unlockChar;
		player->m_PassStageReward->UpdateStagePassByStageLevel(player,stageId);///通关送钻
		if(pStageTable->IsBranchStage())
		{
			UpdateBranchFubenPassed(*player,stageId,isAttackWin);
			GetUnlockCharacter(unlockChar,stageId);//解锁武将
			NR_stars_[m_curFubenID-101]++;
		}
		else if(pStageTable->IsHideStage())
		{
			AddHideFubenFightTimes(*player,stageId);
		}
		else
		{
			GetUnlockCharacter(unlockChar);	////解锁武将
			_GoToNextStage();	//开启下一关
		}
		player->UnlockCharacter(unlockChar);
		PlayerFuBenDB::SendFuBenBaseToDb(*player, *this);
		string battleArrayStr;
		battleArrayStr.reserve(20);
		PlayerCharacterLogic::GenBattleArrayProtoStr(*player,battleArrayStr);
		int32 attackpower = PlayerCharacterLogic::GenBattleArrayConbatPower(*player);
		player->AddEventLogFMT( pb::EVENT_PLAYER_BEHAVIOR_STAGE,
			5,
			"i", player->GetLevel(),
			"i", attackpower,
			"s", battleArrayStr.c_str(),
			"i", stageId,
			"i", pStageTable->GetStageType()
			);
    }

	//struct _Temp_Hero_Func //辅助函数对象
	//{
	//	bool operator()(Character* p)
	//	{
	//		p->CalculateAttr();
	//		return false;
	//	}
	//};
	//_Temp_Hero_Func objFunc;
	//player->m_characterStorage->ForEachBattleCharacterInCombat(objFunc,pGroup->m_unique_id);

    //通知client
	struct stFunc {
        bool                 _isAttackWin;
		Combat::CombatGroup* _pGroup;
        IntPairVec           _reward;
        uint64               _fubenOwnerId;
		uint32*				_kill_moster;
        stFunc(bool isAttackWin, Combat::CombatGroup* pGroup, IntPairVec& reward, uint64 fubenOwnerId,uint32* kill_mo)
            : _isAttackWin(isAttackWin)
            , _pGroup(pGroup)
            , _reward(reward)
            , _fubenOwnerId(fubenOwnerId)
			, _kill_moster(kill_mo)
        {}
		void operator()(uint64 playerId)
		{
            if (Player* player = sPlayerPool.GetByPlayerId(playerId))
			{
				NLOG("################# send awards #############");
                pb::GS2C_Combat_End msg;
                msg.set_is_attack_win(_isAttackWin);
                _pGroup->SaveCombatInfosOfGains(msg, playerId);
				msg.set_group_id(_pGroup->m_unique_id);
                if (playerId == _fubenOwnerId) PackRewardMsg(_reward, msg);
				player->Send(pb::SMSG_COMBAT_COMBAT_END, msg);
				NLOG("on end get award exp = %d, coin = %d, dead = %d, kill = %d",
					msg.statisticexp(), msg.statisticmoney(), msg.statisticdead(), msg.statistickill());
				*_kill_moster = msg.statistickill();
			}
		}
	};
	uint32 kill_mo = 0;
    stFunc func(isAttackWin, pGroup, reward, player->GetGuid(),&kill_mo);
	pGroup->ForEachWatchPlayer(func);
	//pGroup->SaveAllStaticData();

	if(pStageTable->IsHideStage())
	{
		SetHideFubenKillMonsters(*player,stageId,npc_hp,kill_mo);
	}
	
	struct stFunt {
		bool                 _isAttackWin;

		stFunt(bool isAttackWin)
			: _isAttackWin(isAttackWin)
		{}
		void operator()(uint64 playerId)
		{
			if (Player* player = sPlayerPool.GetByPlayerId(playerId))
			{
				if(_isAttackWin)
				{
					NetPack pack(pb::SMSG_UNLOCK_NEW_STAGE);
					player->Send(pack);
				}
			}
		}
	};
	stFunt funt(isAttackWin);
	pGroup->ForMainPlayer(funt);

	if(!player->m_questLogger->GetQuestFinished(GET_BASE_DEF_INT( pb::BD_AUTO_DRAFT_FOR_END_MAIN_QUEST))
		&& (int)player->m_fubenLog->m_curStageID <= GET_BASE_DEF_INT( pb::BD_AUTO_DRAFT_FOR_END_STAGE_LEVEL))
	{
		struct _Hero_Recover_Func //辅助函数对象
		{
			bool operator()(Character* p)
			{
				p->RecoverFullSoldier();
				p->m_player->m_characterStorage->SetModifiedID(p->GetID());
				return false;
			}
		};
		_Hero_Recover_Func recFunc;
		player->m_characterStorage->ForEachBattleCharacter(recFunc);
	}
}

void FubenLog::OnCrusadeEnd(Player* player, uint32 stageId, Combat::CombatGroup* pGroup, bool isAttackWin)
{
	const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId);
	if (pStageTable == NULL) return;

	IntPairVec reward;
	//struct _Temp_Hero_Func //辅助函数对象
	//{
	//	bool operator()(Character* p)
	//	{
	//		p->CalculateAttr();
	//		return false;
	//	}
	//};
	//_Temp_Hero_Func objFunc;
	//player->m_characterStorage->ForEachBattleCharacterInCombat(objFunc,pGroup->m_unique_id);

	//通知client
	struct stFunc {
		bool                 _isAttackWin;
		Combat::CombatGroup* _pGroup;
		IntPairVec           _reward;
		uint64               _fubenOwnerId;

		stFunc(bool isAttackWin, Combat::CombatGroup* pGroup, IntPairVec& reward, uint64 fubenOwnerId)
			: _isAttackWin(isAttackWin)
			, _pGroup(pGroup)
			, _reward(reward)
			, _fubenOwnerId(fubenOwnerId)
		{}
		void operator()(uint64 playerId)
		{
			if (Player* player = sPlayerPool.GetByPlayerId(playerId))
			{
				NLOG("################# send awards #############");
				pb::GS2C_Combat_End msg;
				msg.set_is_attack_win(_isAttackWin);
				_pGroup->SaveCombatInfosOfGains(msg, playerId);
				msg.set_group_id(_pGroup->m_unique_id);
				if (playerId == _fubenOwnerId) PackRewardMsg(_reward, msg);
				player->Send(pb::SMSG_COMBAT_COMBAT_END, msg);
				NLOG("on end get award exp = %d, coin = %d, dead = %d, kill = %d",
					msg.statisticexp(), msg.statisticmoney(), msg.statisticdead(), msg.statistickill());
			}
		}
	};
	stFunc func(isAttackWin, pGroup, reward, player->GetGuid());
	pGroup->ForEachWatchPlayer(func);
	//pGroup->SaveAllStaticData();

	if(stageId != m_crusadeLog.currCruStage)	//当前关卡不是结束的关卡，有可能是过12点重置了
	{
		NLOG("error end ! stage not match");
		return;
	}
	if (isAttackWin) {
		//开启下一关
		const FubenStage* firstStage = sFubenDataMgr.GetFirstCrusadeStageByFubenID(m_crusadeLog.currCrusade);
		int stagesNum = sFubenDataMgr.GetCurrentCrusadeStageNum(stageId);	//当前所处在的远征总关卡数
		int stageEndID = stagesNum + firstStage->Id() - 1;
		if((int)stageId >= stageEndID)		//通关最后一关
		{
			m_crusadeLog.currCruStage = stageEndID;
			m_crusadeLog.isCruDead = 1;
			m_crusadeLog.isCrusadeOver = 1;
		}
		else
		{
			m_crusadeLog.currCruStage = stageId + 1;
			m_crusadeLog.isCruDead = 0;
			m_crusadeLog.isCrusadeOver = 0;
		}
	}
	else
	{
		m_crusadeLog.isCruDead = 0;
		m_crusadeLog.isCrusadeOver = 1;
	}

	pb::GS2C_Combat_Crusade crusadeinfo;
	SaveTo(crusadeinfo);
	NLOG("reset Crusade success restReset=%d,currCrusade=%d,currCruStage=%d,isCruDead=0,award_state=0"
		,m_crusadeLog.restReset,m_crusadeLog.currCrusade,m_crusadeLog.currCruStage);
	player->Send(pb::SMSG_Current_Crusade,crusadeinfo);
	SyncVecs(*player);
}

void FubenLog::OnCrusadeBegin(Player* player,Combat::CombatGroup* pgroup)
{
// 	for(IntPairVec::iterator iter = m_crusadeLog.heroAddTimesBackup.begin();iter != m_crusadeLog.heroAddTimesBackup.end();++iter)
// 	{
// 		int count = m_crusadeLog.heroAddTimes[iter->first - 1] - iter->second;
// 		Character* charac = player->m_characterStorage->MutableCharacter(iter->first);
// 		for(int i = 0;i < count;++i)
// 		{
// 			pgroup->AddObjToAttack(*(charac->CreateShadow()));
// 		}
// 	}
	pb::GS2C_Combat_Crusade crusadeinfo;
	SaveTo(crusadeinfo);
	NLOG("reset Crusade success restReset=%d,currCrusade=%d,currCruStage=%d,isCruDead=0,award_state=0"
		,m_crusadeLog.restReset,m_crusadeLog.currCrusade,m_crusadeLog.currCruStage);
	player->Send(pb::SMSG_Current_Crusade,crusadeinfo);
	SyncVecs(*player);
}


void FubenLog::FillStageInBranchAndHide()
{
	const FubenMap& fuben = sFubenDataMgr.GetFubenMap();
	for(FubenMap::const_iterator iter = fuben.begin();iter != fuben.end();++iter)
	{
		const std::vector<const FubenStage*> *vec = sFubenDataMgr.GetBranchFubenStageList(iter->first);
		if(vec != NULL)
		{
			for(std::vector<const FubenStage*>::const_iterator iter1 = vec->begin();iter1 != vec->end();++iter1)
			{
				branchFubenMap_[(*iter1)->Id()] = BranchBaseInfo((*iter1)->BranchGroupId());
			}
		}
		const std::vector<const FubenStage*> *vec1 = sFubenDataMgr.GetHideFubenStageList(iter->first);
		if(vec1 != NULL)
		{
			for(std::vector<const FubenStage*>::const_iterator iter2 = vec1->begin();iter2 != vec1->end();++iter2)
			{
				hideFubenMap_[(*iter2)->Id()] = HideFubenBaseInfo();
			}
		}
	}
	
}

bool FubenLog::_GoToNextStage()
{
    if (sFubenDataMgr.GetNextStageID(m_curFubenID, m_curStageID))
    {
        const FubenStage* pStage = sFubenDataMgr.GetStage(m_curStageID);
		if (pStage->IsResourceStage() || pStage->IsGroupStage() || pStage->IsHideStage() || pStage->IsBranchStage())
        {
            //资源关无战斗，继续开下一关
            return _GoToNextStage();
        }
		return true;
    }
	else
		return false;
}

void FubenLog::GetUnlockCharacter(std::set<uint32>& unlockChar,const uint32 branch_fuben_id/* = 0*/)
{
	if(branch_fuben_id != 0)
	{
		sFubenDataMgr.GetUnlockCharacter(m_curFubenID, branch_fuben_id,unlockChar);
	}
	else
	{
		sFubenDataMgr.GetUnlockCharacter(m_curFubenID, m_curStageID,unlockChar);
	}
}



void FubenLog::FightWithNpc(Player& player, const FubenStage* pStage, std::vector<uint32>& heroIds)
{
	uint32 npc_hp = 0;
    std::set<Combat::CombatObj*> defence;	//防守npc
    for (IntPairVec::const_iterator it = pStage->m_npcLst.begin(); it != pStage->m_npcLst.end(); ++it)
    {
        Npc* pNpc = sNpcMgr.CreateNpc(it->first, it->second);
        MMO_ASSERT(pNpc);
        if (pNpc) defence.insert(pNpc);
		if(pStage->IsHideStage() && npc_hp == 0)
		{
			npc_hp = pNpc->CurHp();
		}
		
    }
    std::vector<Combat::CombatObj*> attack;	//玩家进攻
    for (std::vector<uint32>::iterator it = heroIds.begin(); it != heroIds.end(); ++it)
    {
        Character* pHero = player.m_characterStorage->MutableCharacter(*it);
        MMO_ASSERT(pHero);
        if (pHero) {
            attack.push_back(pHero);
        }
    }

    if (Combat::CombatGroup* pGroup = Combat::CombatGroup::CreateGroup(attack, defence, Combat::CT_Fuben,pStage->LandForm()))
    {
        //战斗结束回调OnStageEnd
        pGroup->Watch(player.GetGuid());	//玩家标识符
        pb::GS2C_Combat_Pre_Begin msg;
        pGroup->SaveTo(msg);
		msg.set_isautocombat(player.IsAutoCombat());
        player.Send(pb::SMSG_COMBAT_PRE_BATTLE, msg);
        pGroup->AddCallBack_OnEnd(boost::bind(&FubenLog::OnStageEnd, this, &player, pStage->Id(),npc_hp, _1, _2));
    }
}

void FubenLog::CombatInCrusade(Player& player, const FubenStage* pStage, std::vector<uint32>& heroIds)
{
	std::set<Combat::CombatObj*> defence;		//防守npc
	for (IntPairVec::const_iterator it = pStage->m_npcLst.begin(); it != pStage->m_npcLst.end(); ++it)
	{
		Npc* pNpc = sNpcMgr.CreateNpc(it->first, it->second);
		MMO_ASSERT(pNpc);
		if (pNpc) defence.insert(pNpc);
	}
	AddHeroToVector(heroIds);

	if (Combat::CombatGroup* pGroup = Combat::CombatGroup::CreateGroup(defence, Combat::CT_Crusade,pStage->LandForm()))
	{
		//战斗结束回调OnStageEnd
		pGroup->Watch(player.GetGuid());	//玩家标识符
		pb::GS2C_Combat_Pre_Begin msg;
		pGroup->SaveTo(msg);
		msg.set_isautocombat(player.IsAutoCombat());
		player.Send(pb::SMSG_COMBAT_PRE_BATTLE, msg);
		pGroup->AddCallBack_OnEnd(boost::bind(&FubenLog::OnCrusadeEnd, this, &player, pStage->Id(), _1, _2));
		pGroup->AddCallBack_OnBegin(boost::bind(&FubenLog::OnCrusadeBegin,this,&player,pGroup));

		//发送武将剩余次数
		pb::GS2C_Combat_Crusade_Pre_Begin premsg;
		for(IntPairVec::iterator it = m_crusadeLog.heroAddTimesBackup.begin(); it!= m_crusadeLog.heroAddTimesBackup.end(); it++)
		{
			pb::Crusade_HeroInfo* heroinfo = premsg.add_hero_info();
			heroinfo->set_hero_id(it->first);
			heroinfo->set_lefttime(it->second);
		}
		player.Send(pb::SMSG_Crusade_Hero_Times,premsg);
	}
	SyncVecs(player);
}
uint32 FubenLog::IsPassStage(uint32 stageId) const
{
	return m_curStageID > stageId ? 1 : 0;
}



bool FubenLog::IsCursadePassed(const uint32 CrusadeId) const
{
	return m_crusadeLog.currCruStage >= CrusadeId;
}


void FubenLog::InitCrusadeData(Player* player)
{
	NLOG("FubenLog::InitCrusadeData");
	const FubenStage* firstStage = sFubenDataMgr.GetFirstCrusadeStageByFubenID(m_crusadeLog.currCrusade);
	if (firstStage)
	{
		//int stageIndex = m_crusadeLog.currCruStage - firstStage->Id();
		const std::vector<const FubenStage*>* curAllstage = sFubenDataMgr.GetCrusadeFubenStageList(m_crusadeLog.currCrusade);
		if(curAllstage == NULL) 
		{
			NLOG("Can't find Crusade infomation!");
		}
		uint32 CrusadeNum = sFubenDataMgr.GetCurrentCrusadeStageNum(m_crusadeLog.currCruStage);
		std::vector<const FubenStage*>::const_iterator iter = curAllstage->begin();
		string items;
		for(;iter != curAllstage->end();++iter)
		{
			if((*iter)->IsCruGiftsStage() && IsCursadePassed((*iter)->PreStage()))
			{
				int stageIndex = (*iter)->Id() - firstStage->Id() - CrusadeNum;
				if((m_crusadeLog.award_state & (1<<stageIndex)) == 0)	//没有领取
				{
					m_crusadeLog.award_state |= (1<<stageIndex);
					IntPairVec awardlist;
					Utility::SplitStr2((*iter)->GetAwardList(), awardlist);
					sReward.Change(*player, awardlist);
					Utility::PacketItemsToString(items,awardlist);
				}
			}
		}
#ifdef _MMO_SERVER_

		sSysEventMgr.SendJustTipsMail(player->GetGuid(),EVENT_TYPE_CURSADE,items);
#endif
	}
	m_crusadeLog.Reset();
	pb::GS2C_Crusade_Reset msg;
	msg.set_reset(true);
	player->Send(pb::SMSG_CRUSADE_RESET,msg);
	PlayerFuBenDB::SendCrusadeToDb(*player, *this);
}

bool FubenLog::AddShadowInCrusade(Player& player,uint32 heroid, pb::GS2C_Combat_Crusade_Add_Hero& msg)
{
	NLOG("FubenLog::AddShadowInCrusade,heroid=%d",heroid);
	bool ret = false;
	
	if(m_crusadeLog.addHeroTimes <= 0)			//添加次数不足
	{
		return ret;
	}
	
	msg.set_alllefttime(m_crusadeLog.addHeroTimes);
	msg.set_hero_id(heroid);
	msg.set_lefttime(0);

	for(IntPairVec::iterator ite = m_crusadeLog.heroAddTimesBackup.begin();
		 ite != m_crusadeLog.heroAddTimesBackup.end(); ite++)
	{
		NLOG("ite->first=%d, ite->second=%d",ite->first,ite->second);
		if(ite->first == heroid)		//存在着一个武将
		{
			if(ite->second >0)
			{
				ite->second--;
				m_crusadeLog.addHeroTimes--;
				msg.set_alllefttime(m_crusadeLog.addHeroTimes);
				msg.set_lefttime(ite->second);
				ret = true;
				NLOG("alllefttime:%d,hero_id:%d,lefttime:%d",m_crusadeLog.addHeroTimes,heroid,ite->second);
				break;
			}
		}
	}
	msg.set_bsuccess(ret);
	SyncVecs(player);
	return ret;
}

void FubenLog::TakeCrusadeReward(Player& player,const uint32 stageId,pb::GS2C_Crusade_Take_Reward_Rsp& msg)
{
	const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId);
	if(pStageTable == NULL)
	{ 
		msg.set_ret(false); return;
	}
	const FubenStage* firstStage = sFubenDataMgr.GetFirstCrusadeStageByFubenID(pStageTable->FubenId());
	if(firstStage == NULL)
	{
		msg.set_ret(false); return;
	}
	if(pStageTable->IsCruGiftsStage() && IsCursadePassed(pStageTable->PreStage()))
	{
		int stageIndex = pStageTable->Id() - firstStage->Id() - sFubenDataMgr.GetCurrentCrusadeStageNum(stageId);
		if((m_crusadeLog.award_state & (1<<stageIndex)) == 0)	//没有领取
		{
			m_crusadeLog.award_state |= (1<<stageIndex);
			IntPairVec awardlist;
			Utility::SplitStr2(pStageTable->GetAwardList(), awardlist);
			for(IntPairVec::iterator it = awardlist.begin();it != awardlist.end();++it)
			{
				pb::StReward * reward = msg.add_reward();
				reward->set_type(it->first);
				reward->set_value(it->second);
			}
			sReward.Change(player, awardlist);
			msg.set_ret(true);
		}
	}
	msg.set_ret(false);
	player.Send(pb::SMSG_TAKE_CRUSADE_REWARD_RSP,msg);
	pb::GS2C_Combat_Crusade crusadeinfo;
	SaveTo(crusadeinfo);
	NLOG("reset Crusade success restReset=%d,currCrusade=%d,currCruStage=%d,isCruDead=0,award_state=0"
		,m_crusadeLog.restReset,m_crusadeLog.currCrusade,m_crusadeLog.currCruStage);
	player.Send(pb::SMSG_Current_Crusade,crusadeinfo);
	PlayerFuBenDB::SendCrusadeToDb(player, *this);
}

// bool FubenLog::IsBranchFubenPassed(const uint32 stageId)
// {
// 	bool flag = true;
// 	uint32 branch_group_id;
// 	BranchFubenMap::const_iterator iter = branchFubenMap_.find(stageId);
// 	if(iter != branchFubenMap_.end())
// 	{
// 		const BranchBaseInfo& base_Info = iter->second;
// 		branch_group_id = base_Info.branch_group_id_;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 	for(BranchFubenMap::const_iterator iter1 = branchFubenMap_.begin();iter1 != branchFubenMap_.end();++iter1)
// 	{
// 		if(iter1->second.branch_group_id_ == branch_group_id)
// 		{
// 			flag &= iter1->second.is_passed_;
// 		}
// 	}
// 	return flag;
// }

void FubenLog::UpdateBranchFubenPassed(Player& player,const uint32 stageId,const bool passed)
{
	BranchFubenMap::iterator iter = branchFubenMap_.find(stageId);
	if(iter != branchFubenMap_.end())
	{
		BranchBaseInfo& base_Info = iter->second;
		base_Info.is_passed_ = passed;
		pb::BranchFubenInfo msg;
		SaveTo(stageId,base_Info,msg);
		player.Send(pb::SMSG_UPDATE_BRANCH_FUBEN,msg);
		PlayerFuBenDB::SendBranchFubenToDb(player,stageId,base_Info,*this);
	}
}

void FubenLog::UpdateBranchFubenTakendReward(Player& player,const uint32 stageId,const bool taken_star)
{
	BranchFubenMap::iterator iter = branchFubenMap_.find(stageId);
	if(iter != branchFubenMap_.end())
	{
		BranchBaseInfo& base_Info = iter->second;
		base_Info.is_take_award_ = taken_star;
		pb::BranchFubenInfo msg;
		SaveTo(stageId,base_Info,msg);
		player.Send(pb::SMSG_UPDATE_BRANCH_FUBEN,msg);
		PlayerFuBenDB::SendBranchFubenToDb(player,stageId,base_Info,*this);
	}
}

void FubenLog::TakeBranchReward(Player& player,const uint32 stageId,pb::GS2C_take_branch_fuben_star_rsp& msg)
{
	BranchFubenMap::iterator iter = branchFubenMap_.find(stageId);
	if(iter != branchFubenMap_.end())
	{
		BranchBaseInfo& base_Info = iter->second;
		if(base_Info.is_take_award_)
		{
			msg.set_success(false);
			msg.set_branch_stageid(stageId);
			return;
		}
		IntPairVec awardlist;
		if(const FubenStage* pStageTable = sFubenDataMgr.GetStage(stageId))
		{
			Utility::SplitStr2(pStageTable->GetAwardList(), awardlist);
		}
		sReward.Change(player,awardlist);
		for(IntPairVec::const_iterator iter = awardlist.begin();iter != awardlist.end();++iter)
		{
			pb::StReward* reward = msg.add_reward();
			reward->set_type(iter->first);
			reward->set_value(iter->second);
		}
		msg.set_success(true);
		msg.set_branch_stageid(stageId);
		PlayerFuBenDB::SendBranchFubenToDb(player,stageId,base_Info,*this);
		UpdateBranchFubenTakendReward(player,stageId,true);
	}
}

void FubenLog::ResetHideFubenFightTimes(Player& player)
{
	for(HideFubenMap::iterator iter =  hideFubenMap_.begin();iter != hideFubenMap_.end();++iter)
	{
		iter->second.today_fight_times_ = 0;
		PlayerFuBenDB::SendHideFubenToDb(player,iter->first,iter->second);
	}
}

void FubenLog::AddHideFubenFightTimes(Player& player,const uint32 stageId)
{
	HideFubenMap::iterator iter = hideFubenMap_.find(stageId);
	if(iter != hideFubenMap_.end())
	{
		iter->second.today_fight_times_++;
		pb::HideFubenInfo msg;
		SaveTo(stageId,iter->second,msg);
		player.Send(pb::SMSG_UPDATE_HIDE_FUBEN,msg);
		PlayerFuBenDB::SendHideFubenToDb(player,iter->first,iter->second);
	}

}

void FubenLog::SetHideFubenKillMonsters(Player& player,const uint32 stageId,const uint32 npc_hp,const uint32 kill_npc)
{
	if(npc_hp == 0) return;
	HideFubenMap::iterator iter = hideFubenMap_.find(stageId);
	if(iter != hideFubenMap_.end())
	{
		uint32 kill_count =  kill_npc / npc_hp;
		if(kill_count > iter->second.kill_monsters_)
		{
			iter->second.kill_monsters_ = kill_count;
		}
		pb::HideFubenInfo msg;
		SaveTo(stageId,iter->second,msg);
		player.Send(pb::SMSG_UPDATE_HIDE_FUBEN,msg);
		PlayerFuBenDB::SendHideFubenToDb(player,iter->first,iter->second);
	}
}
