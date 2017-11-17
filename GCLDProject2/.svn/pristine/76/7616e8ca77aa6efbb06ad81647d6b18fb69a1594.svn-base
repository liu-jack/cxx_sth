#include "PlayerMarketLog.h"
#include "Table/MarketTableMgr.h"
#include "datastruct/struct_market.h"
#include "../reward/reward.h"
#include "../object/Player.h"
#include "../Building/BuildingLogMgr.h"
#include "../Map/PlayerMapLogic.h"
#include "../Map/country/country.h"
#include "../Map/Table/MapCity.h"
#include "../Service/ServiceMgr.h"
#include "Market.pb.h"
#include "../CrossLogic/PlayerMarketLogDB.h"
#include "../Trigger/trigger.h"
#include "System.h"
#include "Opcode.pb.h"
#include "Enum.pb.h"
#include "Base.h"
//#include "../AutomaticUpdate/AutomaticUpdate.h"
#include "../Technology/PlayerTechnology.h"
#include "GameServer.h"
#include "../Technology/Table/TechnologyTableMgr.h"
#include "BaseDefineMgr.h"
#include "utility/Utility.h"
#include "item/ItemManager.h"
#include "../CrossLogic/PlayerItemLogic.h"
#include "../Map/country/country.h"
static const int Market_Buy_Content_Refresh_CD = 3600;  //商品多久自动刷新
static const int Market_Black_Max_CD_Sec = 900;// 15 * 60;
static const uint32 MAX_COLLECT_COST = 50;


PlayerMarketLog::PlayerMarketLog()
{
    //memset(this, 0, MMO_OFFSET(PlayerMarketLog, m_SuitsPartCnt));
	m_BlackTime = 0; //过点能交易的时刻
	m_LeftBuyNumAddTime = 0;
	m_BuyContentRefreshTime = 0;
	Market_Buy_Left_Times_CD = GET_BASE_DEF_UINT(pb::BD_PLAYER_REGAIN_MARKET_BUY_NUM_MINUTE) * 60;
	memset(m_BuyContent, 0, sizeof(m_BuyContent));
	memset(m_MysteryShopProgress, 0, sizeof(m_BuyContent));
	memset(m_MysteryShopInvestTime, 0, sizeof(m_BuyContent));
	memset(collect_times_,0,sizeof(collect_times_));
	iron_ticket_.clear();
    for (vector<uint32>::iterator it = sMarketTableMgr.m_SuitsPartIds.begin(); 
         it != sMarketTableMgr.m_SuitsPartIds.end(); ++it)
    {
        m_SuitsPartCnt[*it] = 0;
    }
}
void PlayerMarketLog::SaveTo(pb::GS2C_Market_All_Info& msg)
{
    SaveTo(*msg.mutable_collect());
    SaveTo(*msg.mutable_black());
    SaveTo(*msg.mutable_buy());
	SaveTo(*msg.mutable_mystery());
}
void PlayerMarketLog::SaveTo(pb::GS2C_Market_Collect& msg)
{
	for(int i = 0;i < COLLECT_TYPE_COUNT;++i)
	{
		pb::GxDB_Market_Collect_Info *info = msg.add_info();
		info->set_table_id(i+1);
		info->set_times(collect_times_[i]);
	}
	for(std::map<uint32,iron_collect>::iterator iter = iron_ticket_.begin();iter != iron_ticket_.end();++iter)
	{
		pb::GS2C_Iron_Collect* info = msg.add_iron_collect_info();
		info->set_item_id(iter->first);
		info->set_use_time(iter->second.use_time);
		info->set_left_times(iter->second.left_times);
	}
}
void PlayerMarketLog::SaveTo(pb::GS2C_Market_Buy& msg)
{
	uint64 timeNow = sOS.TimeSeconds();
	m_BuyContentRefreshTime = timeNow - timeNow%3600;	//上一个整点
	//NLOG("@@@@@@@@@@@222 m_BuyContentRefreshTime=%lld",m_BuyContentRefreshTime);
    msg.set_left_num_add_time(m_LeftBuyNumAddTime);
    msg.set_content_refresh_time(m_BuyContentRefreshTime);
    msg.clear_content();
    for (int i = 0; i < 3; ++i) {
        pb::MarketBuyContent* ptr = msg.add_content();
        ptr->set_cost_typ(m_BuyContent[i][0]);
        ptr->set_cost_val(m_BuyContent[i][1]);
        ptr->set_give_typ(m_BuyContent[i][2]);
        ptr->set_give_val(m_BuyContent[i][3]);
        ptr->set_quality(m_BuyContent[i][4]);
    }

	//NLOG("@@@@@@@@@@ m_LeftBuyNumAddTime=%lld,m_BuyContentRefreshTime=%lld",m_LeftBuyNumAddTime,m_BuyContentRefreshTime);
}
void PlayerMarketLog::SaveTo(pb::GS2C_Market_Black& msg)
{
    msg.set_time(m_BlackTime);
}
void PlayerMarketLog::SaveTo(pb::GS2C_Market_Mystery_Shop& msg)
{
    for (int i = 0; i < MysteryShop_GoodsCnt; ++i)
    {
        msg.add_progress(m_MysteryShopProgress[i]);
        msg.add_take_item_time(m_MysteryShopInvestTime[i]);
    }
    for (map<uint32, uint8>::iterator it = m_SuitsPartCnt.begin();
        it != m_SuitsPartCnt.end(); ++it)
    {
        pb::PairValue* pair = msg.add_suits_part();
        pair->set_first(it->first);
        pair->set_second(it->second);
    }
}

void PlayerMarketLog::LoadFrom(const pb::GS2C_Market_All_Info& msg)
{
    const pb::GS2C_Market_Mystery_Shop& mystery = msg.mystery();
    const pb::GS2C_Market_Collect& collect = msg.collect();
    const pb::GS2C_Market_Black& black = msg.black();
    const pb::GS2C_Market_Buy& buy = msg.buy();

    m_BlackTime = black.time();
    m_LeftBuyNumAddTime = buy.left_num_add_time();
	//NLOG("@@@@@@@@@@@ m_LeftBuyNumAddTime=%lld",m_LeftBuyNumAddTime);
	//m_BuyContentRefreshTime = buy.content_refresh_time();
	uint64 timeNow = sOS.TimeSeconds();
	//m_BuyContentRefreshTime = timeNow - timeNow%3600;	//上一个整点
    for (int i = 0; i < (int)buy.content_size(); ++i) {
        const pb::MarketBuyContent& content = buy.content(i);
        m_BuyContent[i][0] = content.cost_typ();
        m_BuyContent[i][1] = content.cost_val();
        m_BuyContent[i][2] = content.give_typ();
        m_BuyContent[i][3] = content.give_val();
        m_BuyContent[i][4] = content.quality();
    }
    for (int i = 0; i < (int)mystery.progress_size(); ++i) {
        m_MysteryShopProgress[i] = mystery.progress(i);
        m_MysteryShopInvestTime[i] = mystery.take_item_time(i);
    }
    for (int i = 0; i < (int)mystery.suits_part_size(); ++i) {
        const pb::PairValue& pair = mystery.suits_part(i);
		m_SuitsPartCnt.insert(std::make_pair(pair.first(), pair.second()));
    }
	for(int i = 0;i< collect.info_size();++i)
	{
		const pb::GxDB_Market_Collect_Info &info = collect.info(i);
		collect_times_[info.table_id()-1] = info.times();
	}
	for(int i = 0;i < collect.iron_collect_info_size();++i)
	{
		const pb::GS2C_Iron_Collect & info = collect.iron_collect_info(i);
		if(info.item_id() == 0) continue;
		iron_ticket_[info.item_id()].use_time = info.use_time();
		iron_ticket_[info.item_id()].left_times = info.left_times();
	}
}



bool PlayerMarketLog::InitBuyContent(Player& player)
{
	uint64 timeNow = sOS.TimeSeconds();
	uint64 lastchecktime = timeNow - timeNow%3600;	//上一个整点

	if(m_BuyContentRefreshTime == 0)
	{
		m_BuyContentRefreshTime = lastchecktime;

		if (m_BuyContent[0][0] == 0)
		{

			if (player.GetCurrency(eMarketBuyNum) == 0)	//如果是0，直接加24点
			{
				m_LeftBuyNumAddTime = timeNow;
				player.AddCurrency(pb::IR_ITEM_USED_COST, eMarketBuyNum, GET_BASE_DEF_UINT(pb::BD_PLAYER_REGAIN_MARKET_BUY_NUM_VALUE));//整点奖励
			}

			RefreshBuyContent(player);

			return true;
		}
	}
	else
	{
		uint64 timeNow = sOS.TimeSeconds();		//当前时间
		if(m_LeftBuyNumAddTime > 0 && timeNow > m_LeftBuyNumAddTime)
		{
			uint64 realupdatetime = 0;
			if(player.HaveTechnology(KUAISUJIAOYI))
				realupdatetime = Market_Buy_Left_Times_CD / 2;
			else
				realupdatetime = Market_Buy_Left_Times_CD;
			uint64 timeaddbuynum = timeNow - m_LeftBuyNumAddTime;
			uint64 timesaddbuynum = timeaddbuynum / realupdatetime;
			for(uint64 i = 0; i < timesaddbuynum; i++)
			{
				if (player.GetCurrency(eMarketBuyNum) < (int)GET_BASE_DEF_UINT(pb::BD_PLAYER_REGAIN_MARKET_BUY_NUM_VALUE))	//购买次数不满
				{
					LLOG("add one time ");
					player.AddCurrency(pb::IR_ITEM_USED_COST, eMarketBuyNum, 1);
				}
			}
			m_LeftBuyNumAddTime = timeNow;
		}
		else
			m_LeftBuyNumAddTime = timeNow;
		
		if(lastchecktime > m_BuyContentRefreshTime)	//如果上个整点大于上个检查点
		{
			RefreshBuyContent(player);
		}
	}
    return true;
}
bool PlayerMarketLog::Buy(Player& player, uint32 idx)
{
    UpdateBuyTimes(&player);

	if (player.GetCurrency(eMarketBuyNum) <= 0)
	{
		player.SendErrorCode(pb::ErrNotBuyTimes);
		return false;
	}
    if (idx < 3)
    {
        IntPairVec vec;
		vec.push_back(std::make_pair(m_BuyContent[idx][0], -(int)m_BuyContent[idx][1]));
        vec.push_back(std::make_pair(m_BuyContent[idx][2], m_BuyContent[idx][3]));
		if (!sReward.Enough(player,m_BuyContent[idx][0],(int)m_BuyContent[idx][1]))
		{
			player.SendErrorCode(pb::ErrNotEnoughCurr);
			return false;
		}
        if (player.TryDeductCurrency(pb::IR_ITEM_USED_COST, eMarketBuyNum, 1) && sReward.Change(player, vec))
        {
			LLOG("BUY SUCCESS");
            RefreshBuyContent(player);
            return true;
        }
    }
    return false;
}
bool PlayerMarketLog::Collect(Player* player, int tableId,uint32 coll_times,pb::GS2C_Market_Collect& msg) //market_collect
{
	const DB_MarketCollect* table = sMarketTableMgr.GetCollect(tableId);
    if (NULL != table)
    {
        bool bGiveReward = false;
        if (table->collect_is_free)
        {
            if (player->TryDeductCurrency(pb::IR_ITEM_USED_COST, eFreeCollectNum, coll_times)) {
                bGiveReward = true;
            }
        }
		if (!bGiveReward)
		{
			int collect_cost = 0; 
			for(uint32 i = 0;i < coll_times;++i)
			{
				collect_times_[tableId-1]++;
				collect_cost += MIN(MAX_COLLECT_COST,table->collect_cost + ( collect_times_[tableId-1] - 1));
			}
			bGiveReward = sReward.Change(*player, Reward::Coin_Gold, -collect_cost);
		}
        if (bGiveReward)
        {
            int multi = sMarketTableMgr.RandCollectMulti(tableId);
			if(tableId == IRON_COLLECT_TABLE_ID)
			{
				player->AddIronCollectTimesAndTen(coll_times);
				uint32 value = GET_BASE_DEF_UINT(pb::BD_OBSIDIAN_COLLECT_EXTRA_REWARD_TIMES);
				if(player->GetIronCollectTimesTen() >= value)
				{
					sReward.Change(*player,Reward::Iron,GET_BASE_DEF_UINT(pb::BD_OBSIDIAN_COLLECT_EXTRA_REWARD_NUM));
					player->IronCollectTimesTenMinusValue(static_cast<int>(value));
				}
			}
			for(uint32 i = 0; i < coll_times;++i)
			{
				uint32 val = 0;
				if(tableId == IRON_COLLECT_TABLE_ID)
				{
					CalcMulti(multi);
				}
				if (table->build_module == 0)
				{
					val = table->collect_value*multi;
				}
				else
				{
					val = player->m_buildingLogMgr->GetModuleIncomeBase(table->build_module) * table->collect_value * multi;
				}
				////zhoulunhao------------------------获得科技，银币征收效果加50%
				if(player->HaveTechnology(QIANGSHOU) && tableId == 1 ){ 
					const TecTable* tectable = sTechnologyTableMgr.GetTable(QIANGSHOU);
					val += (val * tectable->Value1() / 100);
				}
				
				///------------------------------------------------
				pb::PairValue * value = msg.add_value();
				value->set_first(val);
				value->set_second(multi);
				sReward.Change(*player, table->collect_item_id, val);
			}
			msg.set_ret(0);
			SaveTo(msg);
			PlayerMarketLogDB::SendCollectToDb(*player, *this);
			return true;
        }
		else
		{
			msg.set_ret(1);
			return false;
		}
    }
	msg.set_ret(1);
    return false;
}
bool PlayerMarketLog::BlackMarket(Player& player, int tableId, int idx)
{
    if (const DB_MarketBlack* table = sMarketTableMgr.GetBlack(tableId))
    {
        int cd = table->add_cd;
        double multi = 1;
        if (player.m_worldMapLogic->GetWorldMapCountry()->HaveSpecialCity(MapCity::Black_Market))
        {
            cd /= 2;
            multi += 0.1;
			
        }
		//////zhoulunhao   科技  HENG_TONG //黑市单次交易量翻倍
		if(player.HaveTechnology(HENG_TONG)){
			const TecTable* tec = sTechnologyTableMgr.GetTable(HENG_TONG);
			multi += tec->Value1();
		}

        const uint64 timeNow = sOS.TimeSeconds();
        if (timeNow + Market_Black_Max_CD_Sec >= m_BlackTime)
        {
            IntPairVec vec;
            vec.push_back(std::make_pair(table->exchange_type, -(int)table->exchange_num));
            if (idx == 0)
                vec.push_back(std::make_pair(table->get_type1, uint32(table->get_num1 * multi)));
            else
                vec.push_back(std::make_pair(table->get_type2, uint32(table->get_num2 * multi)));
			//判断奖励是否达到上限
			if(vec[1].first == Reward::Coin_Silver)
			{	
				if(player.GetCurrency(eCoin) >= (int)player.GetBuildingIncomeLimit(MODULE_MINJU))
					return false;
			}
			if(vec[1].first == Reward::Food)
			{
				if(player.GetCurrency(eFood) >= (int)player.GetBuildingIncomeLimit(MODULE_LONGCHANG))
					return false;
			}
			if(vec[1].first == Reward::Wood)
			{
				if(player.GetCurrency(eWood) >= (int)player.GetBuildingIncomeLimit(MODULE_MUCHANG))
					return false;
			}
            if (sReward.Change(player, vec))
			{
                m_BlackTime = MAX(timeNow, m_BlackTime) + cd;

				pb::GS2C_Market_Black msg;
				SaveTo(msg);
				pb::StReward* reward = msg.mutable_reward();
				reward->set_type(vec[1].first);
				reward->set_value(vec[1].second);
				player.Send(pb::SMSG_MARKET_BLACK, msg);
				PlayerMarketLogDB::SendBlackToDb(player, *this);
				player.UpdateAllDailyQuestInfo(HEISHI_DUIHUAN,1,0);//每日任务
				return true;
            }
        }
    }
    return false;
}
uint64 PlayerMarketLog::UpdateBuyTimes(Player* player)
{
	if (!player)
	{
		return 0;
	}
    uint64 timeNow = sOS.TimeSeconds();		//当前时间
	uint64 realupdatetime = 0;
	if(player->HaveTechnology(KUAISUJIAOYI))
		realupdatetime = Market_Buy_Left_Times_CD / 2;
	else
		realupdatetime = Market_Buy_Left_Times_CD;

    if (player->GetCurrency(eMarketBuyNum) < (int)GET_BASE_DEF_UINT(pb::BD_PLAYER_REGAIN_MARKET_BUY_NUM_VALUE))	//购买次数不满
    {
		if (timeNow > m_LeftBuyNumAddTime + realupdatetime)	//超过半小时，增加购买次数
        {
            m_LeftBuyNumAddTime = timeNow;
			LLOG("add one time ");
			player->AddCurrency(pb::IR_ITEM_USED_COST, eMarketBuyNum, 1);
        }
    }

	return realupdatetime * 1000;
}

   

void PlayerMarketLog::UpdateMatkets(Player* player)
{
	uint64 timeNow = sOS.TimeSeconds();		//当前时间
	m_BuyContentRefreshTime = timeNow;
	RefreshBuyContent(*player);
}

void PlayerMarketLog::RefreshBuyContent(Player& player)
{
	uint64 timeNow = sOS.TimeSeconds();		//当前时间
	m_BuyContentRefreshTime = timeNow;
    MarketBuyVec vec;
    BuildBuyWeightTable(player, vec);

    for (int i = 0; i < 3; ++i) {
        MarketBuyVec::iterator it = System::RandWeight(vec);
        if (it != vec.end()) {
            const DB_MarketBuy* table = *it;
            m_BuyContent[i][0] = table->cost_type;
            m_BuyContent[i][1] = table->cost_num;
            m_BuyContent[i][2] = table->reward_type;
            m_BuyContent[i][3] = table->reward_num;
            m_BuyContent[i][4] = table->quality;
            vec.erase(it);
        }
    }

	if (Country* country = player.m_worldMapLogic->GetWorldMapCountry())
	{
		if(country->HaveSpecialCity(MapCity::Market))
			DoubleBuyContent();
	}

	pb::GS2C_Market_All_Info msg;
	SaveTo(msg);
	player.Send(pb::SMSG_MARKET_ALL_INFO, msg);

	PlayerMarketLogDB::SendBuyToDb(player, *this);
}
void PlayerMarketLog::DoubleBuyContent()
{
    for (int i = 0; i < 3; ++i) {
		if( m_BuyContent[i][2] != 5000200 && m_BuyContent[i][2]/1000000 != 4 && m_BuyContent[i][2] != 5001100 )
		{
			m_BuyContent[i][1] *= 2;
			m_BuyContent[i][3] *= 2;
		}
    }
}
void PlayerMarketLog::BuildBuyWeightTable(Player& player, MarketBuyVec& vec)
{
	BuyMap& mapTable = sMarketTableMgr.m_BuyTable;
	BuyMap::iterator it = mapTable.begin();
    for (; it != mapTable.end(); ++it)
    {
        if (sTrigger.Check(&player, it->second->trigger))
        {
            vec.push_back(it->second);
        }
    }
}
bool PlayerMarketLog::TicketChange(Player& player, int tableId)
{
    if (const DB_SellProgItems* table = sMarketTableMgr.GetTicket(tableId))
    {
        if (sTrigger.Check(&player, table->require_trigger))
        {
            IntPairVec vec;
            vec.push_back(std::make_pair(table->itemID, table->bulk_count));
            vec.push_back(std::make_pair(table->curr_type_1, -(int)table->curr_value_1));
			if (sReward.Change(player, vec))
			{
				return true;
			}
        }
    }
	return false;
}
bool PlayerMarketLog::MysteryShopInvest(Player& player, int tableId)
{
    if (const DB_MysteryShop* table = sMarketTableMgr.GetMysteryGoods(tableId))
    {
        if (player.GetLevel() < (int32)table->open_lv) return false;

        //进度满了
        if (m_MysteryShopProgress[tableId-1] >= table->cost_need_times) return false;

        //今天投资过了
        if (sOS.IsToday(m_MysteryShopInvestTime[tableId-1])) return false;

        if (sReward.Change(player, table->cost_typ, -(int)table->cost_val))
        {
            ++m_MysteryShopProgress[tableId-1];
            m_MysteryShopInvestTime[tableId-1] = sOS.TimeSeconds();
            PlayerMarketLogDB::SendMysteryToDb(player, *this);

			pb::GS2C_Market_Mystery_Shop msg;
			SaveTo(msg);
			player.Send(pb::SMSG_MARKET_MYSTERY, msg);
            return true;
        }
    }
    return false;
}
bool PlayerMarketLog::MysteryShopTakeItem(Player& player, int tableId)
{
    if (const DB_MysteryShop* table = sMarketTableMgr.GetMysteryGoods(tableId))
    {
        //进度未满
        if (m_MysteryShopProgress[tableId-1] < table->cost_need_times) return false;

        int rewardId = 0, rewardCnt = 0;
        if (table->give_typ == 0) { //特殊随机套装残页
            rewardId = _RandSuitsPartItemId();
            rewardCnt = 1;
        } else {
            rewardId = table->give_typ;
            rewardCnt = table->give_val;
        }
        if (sReward.Change(player, rewardId, rewardCnt))
        {
            if (table->give_typ == 0) _AddSuitsPart(rewardId);
            m_MysteryShopProgress[tableId-1] = 0;
            PlayerMarketLogDB::SendMysteryToDb(player, *this);

            pb::GS2C_Market_Mystery_Shop_Take_Item msg;
            msg.set_table_id(tableId);
            msg.set_reward_id(rewardId);
            msg.set_reward_cnt(rewardCnt);
            player.Send(pb::SMSG_MARKET_MYSTERY_TAKE_ITEM, msg);

            return true;
        }
    }
    return false;
}
int PlayerMarketLog::_RandSuitsPartItemId()
{
	std::vector<uint8> vec;
	for (std::map<uint32, uint8>::iterator it = m_SuitsPartCnt.begin(); 
         it != m_SuitsPartCnt.end(); ++it)
    {
        if (it->second < SuitsCnt_Max)
        {
            vec.push_back(it->first);
        }
    }
    int rand = System::Rand(0, vec.size());
    return vec[rand];
}

void PlayerMarketLog::TakeActivityIronCollectReward(Player* player,uint32 id)
{
	pb::GS2C_Activity_Iron_Collect_Reward_Rsp sendmsg;
	const DB_ActivityObsidianCollect* activity_obsidian_collect = sMarketTableMgr.GetActivityObsidianCollect(id);
	if(activity_obsidian_collect)
	{
		if(player->IsTakeIronCollectReward(id))
		{
			IntPairVec vec;
			Utility::SplitStr2(activity_obsidian_collect->reward_list,vec);
			if(player->GetIronCollectTimes() >= activity_obsidian_collect->need_collect_times)
			{
				if(activity_obsidian_collect->is_auto_use == 1)
				{
					PlayerItemLogic::UseItem(player,vec);
				}
				else
				{
					sReward.Change(*player,vec);
				}
				for(IntPairVec::iterator iter = vec.begin();iter != vec.end();++iter)
				{
					pb::StReward* reward = sendmsg.add_reward();
					reward->set_type(iter->first);
					reward->set_value(iter->second);
				}
				for(std::map<uint32,iron_collect>::iterator iter = iron_ticket_.begin();iter != iron_ticket_.end();++iter)
				{
					pb::GS2C_Iron_Collect * info = sendmsg.add_info();
					info->set_item_id(iter->first);
					info->set_use_time(iter->second.use_time);
					info->set_left_times(iter->second.left_times);
				}
				sendmsg.set_ret(pb::COLLECT_REWARD_SUCCESS);
				PlayerMarketLogDB::SendCollectToDb(*player, *this);
				player->TakeIronCollectReward(id);
			}
		}
		else
		{
			sendmsg.set_ret(pb::COLLECT_REWARD_HAS_TAKE);
		}
	}
	else
	{
		NLOG("Id is invalid!");
		sendmsg.set_ret(pb::COLLECT_REWARD_OTHER_REASON);
	}
	player->Send(pb::SMSG_ACTIVITY_IRON_COLLECT_REWARD_RSP,sendmsg);
}


void PlayerMarketLog::_AddSuitsPart(int itemId)
{
    ++m_SuitsPartCnt[itemId];
}

void PlayerMarketLog::ResetCollectTimes(Player* player)
{
	memset(collect_times_,0,sizeof(collect_times_));
	PlayerMarketLogDB::SendCollectToDb(*player, *this);
	pb::GS2C_Market_All_Info msg;
	SaveTo(msg);
	player->Send(pb::SMSG_MARKET_ALL_INFO, msg);
}

void PlayerMarketLog::CalcMulti(int &multi)
{
	if(!iron_ticket_.empty())
	{
		std::map<uint32,iron_collect>::iterator iter1 = iron_ticket_.find(ItemEnum::IRON_COLLECT_MULTI_TEN);
		if(iter1 != iron_ticket_.end())
		{
			if(iter1->second.left_times > 0)
			{
				GetMultiByItemId(ItemEnum::IRON_COLLECT_MULTI_TEN,iter1,multi);
			}
		}
	}
}

void PlayerMarketLog::GetMultiByItemId(ItemId item_id,std::map<uint32,iron_collect>::iterator & iter,int &multi)
{
	const LogicItemProto*  item_ptr =  sItemMgr.Find(item_id);
	if(item_ptr && item_ptr->IsIronTenPunk())
	{
		multi = item_ptr->Value1();
		iter->second.left_times--;
	}
}
