#include "PlayerBaoQiLog.h"
#include "Table/BaoQiTableMgr.h"
#include "OS.h"
#include "../reward/reward.h"
#include "BaoQi.pb.h"
#include "../object/Player.h"
#include "Opcode.pb.h"
#include "../CrossLogic/PlayerBaoQiLogDB.h"
#include "../Trigger/trigger.h"
#include "../item/ItemManager.h"
#include "../character/Character.h"
#include "../character/CharacterStorage.h"
#include "Enum.pb.h"
#include "../item/ItemArray.h"
#include "DailyQuest/daily_quest_def.h"
#include "CrossLogic/ActivityOccupyCityDB.h"
#include "Activity/Personal/PersonalTask.h"
#include "Rank.pb.h"
#include "Congratulate/Table/CongratulateTableMgr.h"
#include "CrossLogic/PlayerCongratulateDB.h"
#include "session/PlayerPool.h"
#include "Congratulate/CongratulateRewardLog.h"
#include "WannaBeStronger/WannaBeStrongerLog.h"
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossDestroy/CrossDestroy.h"
#endif


PlayerBaoQiLog::PlayerBaoQiLog()
{
	memset(m_BaoQiLst,0,sizeof(m_BaoQiLst));
	ItemUsed_.clear();
	unlock_gem_id_ = 0;
}
void PlayerBaoQiLog::SaveTo(pb::GS2C_BaoQi_All_Info& msg)
{
    for (int i = 0; i < BaoQi_MaxCnt; ++i)
    {
        pb::BaoQiPro* data = msg.add_property();
        data->set_exp(m_BaoQiLst[i].exp);
        data->set_level(m_BaoQiLst[i].level);
        data->set_star(m_BaoQiLst[i].star);
        data->set_gem(m_BaoQiLst[i].gem);
        data->set_have_hole(m_BaoQiLst[i].have_hole);
    }
	for(std::map<uint64,item_use>::iterator iter = ItemUsed_.begin();
		iter != ItemUsed_.end();)
	{
		if(iter->second.count_ == 0){
			ItemUsed_.erase(iter++);
		}
		else{
			pb::UsedItem* pair_time = msg.add_item_used();
			pair_time->set_itemid(iter->second.item_id_);
			pair_time->set_count(iter->second.count_);
			pair_time->set_used_time(iter->first);
			iter++;
		}
	}
}
void PlayerBaoQiLog::LoadFrom(const pb::GS2C_BaoQi_All_Info& msg)
{
    for (int i = 0; i < msg.property_size(); ++i)
    {
        m_BaoQiLst[i].exp = msg.property(i).exp();
        m_BaoQiLst[i].level = msg.property(i).level();
        m_BaoQiLst[i].star = msg.property(i).star();
        m_BaoQiLst[i].gem = msg.property(i).gem();
        m_BaoQiLst[i].have_hole = msg.property(i).have_hole();
    }
	if(msg.property_size() != 0)
		unlock_gem_id_ = msg.property_size() - 1;
	for(int i = 0;i < msg.item_used_size();++i)
	{
		const pb::UsedItem& pair_time = msg.item_used(i);
		if(pair_time.count() == 0) continue;
		ItemUsed_[pair_time.used_time()] = item_use(pair_time.itemid(),pair_time.count());
	}
}

void PlayerBaoQiLog::SaveToMsg(pb::ShenQiMsg& msg)
{
	for(int i = 0;i < BaoQi_MaxCnt;++i)
	{
		if(m_BaoQiLst[i].level == 0) continue;
		pb::ShenQiInfo * shen_qi_info = msg.add_shen_qi_info();
		shen_qi_info->set_shenqi_lv(m_BaoQiLst[i].level);
		shen_qi_info->set_shenqi_id(i);
	}
}

void PlayerBaoQiLog::FreshAllChara(Player& player)
{
	struct _Temp_Hero_Func //辅助函数对象
	{
		bool operator()(Character* p)
		{
			p->ReFreshCombatData();
			return true;
		}
	};
	_Temp_Hero_Func objFunc;
	player.m_characterStorage->ForEachBattleCharacter(objFunc);
}

const uint32 PlayerBaoQiLog::GetCountOfBaoQi(const uint32 level) const
{
	uint32 count = 0;
	for (int i = 0; i < BaoQi_MaxCnt; ++i)
	{
		if(m_BaoQiLst[i].level >= level)
			count++;
	}
	return count;
}

const uint32 PlayerBaoQiLog::FindMinBaoQiLevel() const
{
	uint32 min_level = sBaoQiTableMgr.GetBaoQiMaxLv();
	for (int i = 0; i < BaoQi_MaxCnt; ++i)
	{
		if(min_level > m_BaoQiLst[i].level)
			min_level = m_BaoQiLst[i].level;
	}
	return min_level;
}

////this function is abandoned
bool PlayerBaoQiLog::Create(Player& player, uint id, pb::GS2C_BaoQi_Create& msg)
{
	bool ret = false;
    if (const DB_BaoQi_Unlock* table = sBaoQiTableMgr.GetBaoQiUnlock(id))
    {
        if (sTrigger.Check(&player, table->unlock_trigger))
        {
            IntPairVec vec;
			vec.push_back(std::make_pair(table->creat_need_item1, -(int)table->item1_num));
			vec.push_back(std::make_pair(table->creat_need_item2, -(int)table->item2_num));
			vec.push_back(std::make_pair(table->creat_need_item3, -(int)table->item3_num));
            if (sReward.Enough(player,vec) && sReward.Change(player, vec))
            {
                m_BaoQiLst[id].level = 1;
                PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
				FreshAllChara(player);
				ret = true;
            }
			else
			{
				msg.set_reterr(pb::Err_BaoQi_Resource);
			}
        }
		else
			msg.set_reterr(pb::Err_BaoQi_PassStage);
    }
	return ret;
}

void PlayerBaoQiLog::UpdateGemAttr(Player& player)
{
	if (const DB_BaoQi_Unlock* table = sBaoQiTableMgr.GetBaoQiUnlock(unlock_gem_id_))
	{
		if (sTrigger.Check(&player, table->unlock_trigger))
		{
			if(m_BaoQiLst[unlock_gem_id_].level == 0)
			{
				m_BaoQiLst[unlock_gem_id_].level = 1;
				++unlock_gem_id_;
				PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
				FreshAllChara(player);
			}
		}
	}
}

void PlayerBaoQiLog::GM_SetShenQiLevel(Player& player,uint id,uint level)
{
	if (id >= BaoQi_MaxCnt) return;
	BaoQiPro& pro = m_BaoQiLst[id];
	//uint32 oldLevel = pro.level;
	if (level >= (uint32)player.GetLevel() || level >= sBaoQiTableMgr.GetBaoQiMaxLv())
	{
		return;
	}
	pro.level = level;
	PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
	SendCongratulateMsg(player);
	player.UpdateAllGrowUpQuestInfo(pb::BAOQI_LEVEL_UP,pro.level);
	FreshAllChara(player);
}

////this function is abandoned
bool PlayerBaoQiLog::BuyDrawing(Player& player, uint id)
{
    if (const DB_BaoQi_Unlock* table = sBaoQiTableMgr.GetBaoQiUnlock(id))
    {
        IntPairVec vec;
		vec.push_back(std::make_pair(Reward::Coin_Gold, -(int)table->buy_item1_cost));
		vec.push_back(std::make_pair(table->creat_need_item1, 1));
		if (!sReward.Enough(player,Reward::Coin_Gold, (int)table->buy_item1_cost))
		{
			player.SendErrorCode(pb::ErrNotEnoughGold);
			return false;
		}
		else if (!player.CanAddItemToBag(table->creat_need_item1))
		{
			player.SendErrorCode(pb::ErrRechBagCapacity);
			return false;
		}
		else if(sReward.Change(player, vec))
		{
			return true;
		}
    }
    return false;
}
int PlayerBaoQiLog::AddExp(Player& player, uint id)
{
    if (id >= BaoQi_MaxCnt) return 0;

    BaoQiPro& pro = m_BaoQiLst[id];

	if (pro.level >= (uint32)player.GetLevel() || pro.level >= sBaoQiTableMgr.GetBaoQiMaxLv())
	{
		return 0;
	}

    if (const DB_BaoQi::Property* table = sBaoQiTableMgr.GetBaoQi(id, pro.level))
    {
        if (sReward.Change(player, Reward::Iron, -(int)table->up_cost))
        {
			int multi = CalcMulti(id,pro.level);
			if(multi == 0)
			{
				multi = sBaoQiTableMgr.RandCriticaMulti(id, pro.level);
			}
            int addExp = table->click_value * multi;

			sPersonTask.addProgress(CASTING_TIMES,1,player);
#ifdef _SERVER_CROSS_
			sCrossDestroy.addProgress(CASTING_TIMES,1,player);
#endif

            if (pro.exp >= 100) {
                ++pro.level;
                pro.exp = 0;
				SendCongratulateMsg(player);
				//升级刷新武将属性
				FreshAllChara(player);

				return 0;
            }
			else
			{
				if (pro.exp + addExp >= 100) {
					++pro.level;
					SendCongratulateMsg(player);
					int diffExp = 100 - pro.exp;
					pro.exp = 0;
					return diffExp;
				}
				else
				{
					pro.exp += addExp;	
				}
			}
			PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
            return addExp;
        }
    }
    return 0;
}
void PlayerBaoQiLog::LevelUp(Player& player, uint id)
{
    if (id >= BaoQi_MaxCnt) return;

    BaoQiPro& pro = m_BaoQiLst[id];

    uint32 oldLevel = pro.level;

    if (oldLevel >= (uint32)player.GetLevel() || oldLevel >= sBaoQiTableMgr.GetBaoQiMaxLv())
    {
        return;
    }

    pb::GS2C_BaoQi_Exp_Inc_History msg;
    msg.set_id(id);
	uint32 count = 0;
    while (oldLevel == pro.level)
    {
        int addExp = AddExp(player, id);

        if (addExp <= 0) break;

        msg.add_inc_val_lst(addExp);
		count++;
    }
    PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);

    player.Send(pb::SMSG_BAO_QI_EXP_INC_HISTORY, msg);
	player.UpdateAllDailyQuestInfo(BAOQI_ZHUZAO,count,0);//每日任务
	player.UpdateAllGrowUpQuestInfo(pb::BAOQI_LEVEL_UP,pro.level);

//	sPersonTask.addProgress(CASTING_TIMES,1,player);
//#ifdef _SERVER_CROSS_
//	sCrossDestroy.addProgress(CASTING_TIMES,1,player);
//#endif

	FreshAllChara(player);
}
void PlayerBaoQiLog::StarUp(Player& player, uint id)
{
    if (id >= BaoQi_MaxCnt) return;

    BaoQiPro& pro = m_BaoQiLst[id];

    if (const DB_BaoQi_UpStar* table = sBaoQiTableMgr.GetBaoQiUpStar(pro.star + 1))
    {
        if (pro.level >= table->star_need_level)
        {
            ++pro.star;

            PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
			FreshAllChara(player);
        }
    }
}
bool PlayerBaoQiLog::Punch(Player& player, uint id)
{
    if (id >= BaoQi_MaxCnt) return false;

    BaoQiPro& pro = m_BaoQiLst[id];

    if (const DB_BaoQi_Unlock* table = sBaoQiTableMgr.GetBaoQiUnlock(id))
    {
        if (pro.have_hole == 0 && sReward.Change(player, Reward::Iron, -(int)table->punching_cost_iron))
        {
            pro.have_hole = 1;

            PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);

            return true;
        }
    }
    return false;
}
bool PlayerBaoQiLog::KnitGem(Player& player, uint id, uint gemItemId)
{
    if (id >= BaoQi_MaxCnt) return false;

    const LogicItemProto* proto = sItemMgr.Find(gemItemId);
    if (proto && proto->IsJewel())
    {
        BaoQiPro& pro = m_BaoQiLst[id];

        if (pro.have_hole == 1 && pro.gem == 0 && sReward.Change(player, gemItemId, -1))
        {
            pro.gem = gemItemId;

            PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
			FreshAllChara(player);
            return true;
        }
    }
    return false;
}
bool PlayerBaoQiLog::UnKnitGem(Player& player, uint id)
{
    if (id >= BaoQi_MaxCnt) return false;

    BaoQiPro& pro = m_BaoQiLst[id];

    //宝石拆卸：平时要5金币，特殊活动期间免费
    IntPairVec vec;
	vec.push_back(std::make_pair(Reward::Coin_Gold, -5));
	vec.push_back(std::make_pair(pro.gem, 1));
	if (!sReward.Enough(player,Reward::Coin_Gold, 5))
	{
		player.SendErrorCode(pb::ErrNotEnoughGold);
		return false;
	}
    if (sReward.Change(player, vec))
    {
        pro.gem = 0;

        PlayerBaoQiLogDB::SendAllInfoToDb(player, *this);
		FreshAllChara(player);
        return true;
    }
    return false;
}
uint32 PlayerBaoQiLog::GetMaxLevel() const
{
    uint32 ret = 0;
    for (int i = 0; i < BaoQi_MaxCnt; ++i) {
        if (ret < m_BaoQiLst[i].level)
            ret = m_BaoQiLst[i].level;
    }
    return ret;
}

std::map<uint64,item_use>::iterator PlayerBaoQiLog::FindItemInItemUsed(const uint32 ItemId)
{
	std::map<uint64,item_use>::iterator iter = ItemUsed_.begin();
	for(;iter != ItemUsed_.end();++iter)
	{
		if(iter->second.item_id_ == ItemId && iter->second.count_ > 0)
		{
			return iter;
		}
	}
	return iter;
}

void PlayerBaoQiLog::GetMultiByItemId(ItemId item_id,std::map<uint64,item_use>::iterator & iter,int &multi)
{
	const LogicItemProto*  item_ptr =  sItemMgr.Find(item_id);
	if(item_ptr && item_ptr->IsShenQiPunk())
	{
		multi = item_ptr->Value1();
		iter->second.count_--;
	}
}

uint32 PlayerBaoQiLog::CalcMulti(uint32 id,uint32 level)
{
	int multi = 0;
	if(!ItemUsed_.empty())
	{
		std::map<uint64,item_use>::iterator iter1 = FindItemInItemUsed(ItemEnum::SHENQI_CRITICAL_FOUR);
		std::map<uint64,item_use>::iterator iter2 = FindItemInItemUsed(ItemEnum::SHENQI_CRITICAL_TEN);
		std::map<uint64,item_use>::iterator iter = FindItemInItemUsed(ItemEnum::SHENQI_CRITICAL_BUFF);
		if(iter != ItemUsed_.end())
		{
			CalcShenQiBuff(id,level,multi);
			return multi;
		}
		if(iter1 != ItemUsed_.end() && iter2 != ItemUsed_.end())
		{
			if(iter1->first > iter2->first)
			{
				GetMultiByItemId(ItemEnum::SHENQI_CRITICAL_TEN,iter2,multi);
			}
			else
			{
				GetMultiByItemId(ItemEnum::SHENQI_CRITICAL_FOUR,iter1,multi);
			}
		}
		else if(iter1 == ItemUsed_.end() && iter2 == ItemUsed_.end())
		{
			multi = sBaoQiTableMgr.RandCriticaMulti(id, level);
		}
		else
		{
			if(iter1 != ItemUsed_.end())
			{
				GetMultiByItemId(ItemEnum::SHENQI_CRITICAL_FOUR,iter1,multi);
			}
			if(iter2 != ItemUsed_.end())
			{
				GetMultiByItemId(ItemEnum::SHENQI_CRITICAL_TEN,iter2,multi);
			}
		}
	}
	else
	{
		multi = sBaoQiTableMgr.RandCriticaMulti(id, level);
	}
	return multi;
}

void PlayerBaoQiLog::CalcShenQiBuff(int id,int level,int &multi)
{
	uint64 time_now = sOS.TimeSeconds();
	const LogicItemProto*  item_ptr =  sItemMgr.Find(ItemEnum::SHENQI_CRITICAL_BUFF);
	if(item_ptr && item_ptr->IsShenQiPunk())
	{
		//BOOST_STATIC_ASSERT(item_ptr->IsShenQiPunk(),"Item is not matched!");
		uint32 buff_last_time = item_ptr->Value1() * ONE_HOUR_SECOND;
		if(ShenQiBuffLastTime(time_now,buff_last_time) > 0)
		{
			int value  = item_ptr->Value2();
			multi = sBaoQiTableMgr.RandCriticaMulti(id, level,value);
		}
		else
		{
			multi = sBaoQiTableMgr.RandCriticaMulti(id, level);
		}
	}
}
////返回神器暴击buff的持续时间
uint64 PlayerBaoQiLog::ShenQiBuffLastTime(uint64 time_now,uint32 buff_last_time)
{
	uint64 last_long = 0;
	bool has_one_shen_qi_buff_is_activie = false;
	for(std::map<uint64,item_use>::iterator iter = ItemUsed_.begin();
		iter != ItemUsed_.end();++iter)
	{
		if(iter->second.item_id_ == ItemEnum::SHENQI_CRITICAL_BUFF && iter->second.count_ > 0)
		{
			buff_last_time *= iter->second.count_;
			if(time_now <= iter->first + buff_last_time)
			{
				if(has_one_shen_qi_buff_is_activie){
					last_long += buff_last_time;
				}
				else{
					has_one_shen_qi_buff_is_activie = true;
					last_long += buff_last_time - (time_now - iter->first);
				}
			}
			else{
				iter->second.count_--;
			}
		}
	}
	return last_long;
}

void PlayerBaoQiLog::SendCongratulateMsg(Player& player)
{
	//player.UpdateWannaBeStrongerInfo();
	const uint32 min_level = FindMinBaoQiLevel();
	uint32 cat1 = 0;
	if(min_level >= CONGRATULATE_SHEN_QI_LEVEL_150)
	{
		if(!player.TestFlag(pb::PLAYER_CONGRATULATE_SHENQI_150))
		{
			player.SetFlag(pb::PLAYER_CONGRATULATE_SHENQI_150);
			cat1 = CONGRATULATE_SHEN_QI_LEVEL_150;
		}
	}
	else if(min_level >= CONGRATULATE_SHEN_QI_LEVEL_120)
	{
		if(!player.TestFlag(pb::PLAYER_CONGRATULATE_SHENQI_120))
		{
			player.SetFlag(pb::PLAYER_CONGRATULATE_SHENQI_120);
			cat1 = CONGRATULATE_SHEN_QI_LEVEL_120;
		}
	}
	else if(min_level >= CONGRATULATE_SHEN_QI_LEVEL_100)
	{
		if(!player.TestFlag(pb::PLAYER_CONGRATULATE_SHENQI_100))
		{
			player.SetFlag(pb::PLAYER_CONGRATULATE_SHENQI_100);
			cat1 = CONGRATULATE_SHEN_QI_LEVEL_100;
		}
	}
	else if(min_level >= CONGRATULATE_SHEN_QI_LEVEL_80)
	{
		if(!player.TestFlag(pb::PLAYER_CONGRATULATE_SHENQI_80))
		{
			player.SetFlag(pb::PLAYER_CONGRATULATE_SHENQI_80);
			cat1 = CONGRATULATE_SHEN_QI_LEVEL_80;
		}
	}
	else
	{
		return;
	}
	uint32 before_rank = 0;
	if(cat1 == 0) return;
	uint32 table_id = sCongratulateMgr.GetTableId(CONGRATULATE_TYPE_SHENQI,cat1,before_rank);
	if(table_id == 0) return;
	size_t index = (cat1-CONGRATULATE_SHEN_QI_LEVEL_BASE) / 20;
	if(sCongratulateMgr.congratulate_shen_qi_[index] < before_rank)
	{
		sCongratulateMgr.congratulate_shen_qi_[index]++;
		pb::GS2C_Achieve_Congratulate msg;
		pb::C2GS_Congratulate_Accept *info = msg.mutable_info();
		info->set_rank(sCongratulateMgr.congratulate_shen_qi_[index]);
		info->set_congratulate_type(CONGRATULATE_TYPE_SHENQI);
		info->set_table_id(table_id);
		msg.set_to_player_name(player.CharName());
		info->set_to_player_id(player.GetGuid());
		info->set_operate_type(pb::DATA_UPDATE);
		const uint64 time_Now = sOS.TimeSeconds();
		info->set_active_time(time_Now);
		player.m_CongratulateLog->SetAchieve(time_Now,table_id);
		CongratulateLogDB::SendInfoToSelfCongratulateDb(player,*(player.m_CongratulateLog),time_Now,pb::DATA_UPDATE);
		ActivityOccupyCityDB::SendUpdateInfoToDb(cat1 << CONGRATULATE_TYPE_SHENQI,sCongratulateMgr.congratulate_shen_qi_[index]);
		player.Send(pb::SMSG_ACHIEVE_CONGRATULATE,msg);
		player.Set24HoursPastedFunc(time_Now);
		sPlayerPool.BroadcastMsgToAllPlayerByCountry(pb::SMSG_ACHIEVE_CONGRATULATE,msg,player.GetCountryId(),player.GetGuid(),time_Now);
	}
}
