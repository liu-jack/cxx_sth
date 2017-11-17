#include "Player.h"
#include "Base.h"
#include "memory_buffer/NetPack.h"
#include "Logger.h"
#include "OS.h"
#include "boost/bind.hpp"
#include "boost/foreach.hpp"
#include "../server_client/DbProxyServerClient.h"
#include "../server_client/EventLogServerClient.h"
#include "../character/CharacterStorage.h"
#include "../item/ItemArray.h"
#include "../item/LogicItemProto.h"
#include "../item/ItemManager.h"
#include "../CrossLogic/PlayerItemLogic.h"
#include "../CrossLogic/PlayerCharacterLogic.h"
#include "../CrossLogic/ItemCharacterLogic.h"
#include "../CrossLogic/PlayerOperationLogic.h"
//#include "../quest/IQuestLogger.h"
#include "../quest/QuestLogger.h"
#include "../quest/behavior/PlayerBehaviorCounter.h"
#include "PlayerStringGroup.h"
#include "utility/Utility.h"
#include "utility/STLUtility.h"
#include "utility/TimePeriod.h"
#include "Opcode.pb.h"
#include "GameServer.pb.h"
#include "CommomDef.pb.h"
#include "Enum.pb.h"
#include "ServerOpcode.pb.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Congratulate/CongratulateRewardLog.h"
//#include "../Social/SocialHandler.h"
#include "../Building/BuildingLogMgr.h"
#include "../TimerEvent/TimerEventGroup.h"
#include "../Map/PlayerMapLogic.h"
#include "../BaoQi/PlayerBaoQiLog.h"
#include "../item/ItemEquip.h"
#include "../session/PlayerPool.h"
#include "../Palace/PalaceMgr.h"
#include "../buff/Buff.h"
#include "../Service/ServiceMgr.h"
#include "../Technology/PlayerTechnology.h"
#include "Exploit/ExploitMgr.h"
#include "../DailyQuest/Table/DailyQuestTableMgr.h"
#include "DailyQuest/DailyQuest.h"
#include "CrossLogic/DailyQuestDB.h"
#include "GrowUpQuest/Table/GrowUpQuestTableMgr.h"
#include "GrowUpQuest/GrowUpQuest.h"
#include "CrossLogic/PlayerGrowUpQuestDB.h"
#include "CrossLogic/PlayerPassStageReward.h"
#include "CrossLogic/ActivityOccupyCityDB.h"
#include "SignUp/SignUp.h"
#include "SignUp/Table/SignUpTableMgr.h"
#include "PassStageActivity/ActivtyStage.h"
#include "PassStageActivity/Table/ActivtyStageRewardTableMgr.h"
#include "ActivityStarBox/ActivityStarBox.h"
#include "ActivityStarBox/Table/ActivityStarBoxTableMgr.h"
#include "ActivityXiLian/Table/ActivityXiLianTableMgr.h"
#include "ActivityCelebrity/Table/ActivityCelebrityTableMgr.h"
#include "ActivityMoneyToGold/Table/ActivityMoneyToGoldTableMgr.h"
#include "WorldFightAchievement/Table/WorldFightAchievementMgr.h"
#include "ContinueOccupyCity/Table/ContinueOccupyCityMgr.h"
#include "ActivityRank/table/ActivityRankTableMgr.h"
#include "CrossLogic/PlayerCongratulateDB.h"
#include "CrossWar.pb.h"
#include "Rank.pb.h"
#include "../reward/reward.h"
#include "PlayerDataManager.h"
#include "../chat/ChatSpeaker.h"
#include "Congratulate/Table/CongratulateTableMgr.h"
#include "WannaBeStronger/WannaBeStrongerLog.h"
#ifdef _SERVER_CROSS_
#include "InterServer.pb.h"
#include "../../../net_framework/Session.h"
#endif
#include "Technology/Table/TechnologyTableMgr.h"
#include "ActivityControl/ActivityMgr.h"
//#include "../Market/PlayerMarketLog.h"
//#include "../fuben/FubenLog.h"

#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
#ifdef _MMO_SERVER_
#include "../session/PlayerSession.h"
#include "../GameServer.h"
#include "../stage/StageLogger.h"
#include "../fuben/FubenLog.h"
#include "../CrossLogic/PlayerSellLogic.h"
#include "../sell/SellLog.h"
#include "../Loot/LootRoundRecordGroup.h"
#include "../Lottery/LotteryLog.h"
#include "../Mail/MailBox.h"
#include "../Market/PlayerMarketLog.h"
#include "../Altar/PlayerAltarLog.h"
#include "../Dungeon/PlayerDungeonLog.h"
#include "../Ironsmith/PlayerIronsmithLog.h"
#include "../Story/PlayerStoryLog.h"
#include "../CrossLogic/PlayerFuBenDB.h"
#include "PlayerDataManager.h"
#include "Map/WorldMap.h"
#include "Map/country/country.h"
#include "../AutomaticUpdate/AutoUpdateGame.h"
#include "../GovernmentAffairs/GovAffairsLog.h"
#include "../Palace/PalaceAchievementLog.h"
#include "../ActivityHeroReward/ActivityHeroRewardLog.h"
#include "../WorldFightAchievement/WorldFightAchievement.h"
#include "data/PayItemTable.h"
#include "../ContinueOccupyCity/ContinueOccupyCityLog.h"
#endif
#include "seige_force/SeigeForceLog.h"
#include "Web.pb.h"
#include "SysEvent/SysEventMgr.h"
#include "../Map/Table/MapTableMgr.h"
#include "../Market/Table/MarketTableMgr.h"
#include "def/EventLogDefine.h"
#include "Activity/ActMgr.h"
using namespace pb;

/*
整点奖励   每小时  零点开始
清空功勋榜	 每天   零点开始
宴会奖励    每4小时   零点开始 

*/
#define MONTH_CARD_NORMAL	"com.yxjq.m30"
#define MONTH_CARD_ADVANCE	"com.yxjq.m50"
uint32 activity_occupy_city_worm = 0;
uint32 activity_occupy_city_human = 0;
uint32 activity_occupy_city_god = 0;
uint32 _Service_Player_Buff(void* p){
    Player* ptr = (Player*)p;
    ptr->RefreshBuff();
    return 500;
}
#ifdef _MMO_SERVER_
uint32 _Service_Player_UpdateBuyTime(void* p){
	Player* ptr = (Player*)p;
	return (uint32)ptr->m_marketLog->UpdateBuyTimes(ptr);
}
uint32 _Service_GovnermentAffairsIncTimes(void *p)
{
	Player* ptr = (Player*)p;
	return (uint32)ptr->m_GovAffairsLog->UpdateZhengWuTimes(ptr);
}
#endif




Player::Player( void )
:m_pSession( NULL )
,m_pCountry(NULL)
,box_queue_(sExploitMgr.box_queue_)
,trick_queue_(sExploitMgr.trick_queue_)
,occupy_queue_(sExploitMgr.occupy_queue_)
,occupy_queue_size_(occupy_queue_.size())
,trick_queue_size_(trick_queue_.size())
,cross_kill_troops(false)
,cross_kill_troops_endtime(0)
,rush_cd_endtime(0)
,cross_hammer_num(0)
,m_maxUpdatet(0)
,m_maxMinuupdatet(0)
{
#ifdef _MMO_SERVER_
	m_fubenLog.reset(new FubenLog());
	m_marketLog.reset(new PlayerMarketLog());
	m_AltarLog.reset(new PlayerAltarLog());
	m_SmithyLog.reset(new PlayerIronsmithLog());
	 m_DungeonLog.reset(new PlayerDungeonLog());
	 m_equipShop.reset(new ItemArray());
	 m_mailBox.reset(new MailBox());
	 m_StoryLog.reset(new PlayerStoryLog());
	 m_GovAffairsLog.reset(new GovAffairsLog());
	m_PalaceAchievementLog.reset(new PalaceAchievementLog());
	m_ActivityStarBoxLog.reset(new ActivityStarBoxLog());
	m_ActivityHeroRewardLog.reset(new ActivityHeroRewardLog());
	m_WorldFightAchievementLog.reset(new WorldFightAchievementLog());
	m_ContinueOccupyCityLog.reset(new ContinueOccupyCityLog());
#endif
	m_SeigeForceLog.reset(new SeigeForceLog());
	m_chatSpeaker.reset(new ChatSpeaker());
    m_characterStorage.reset( new CharacterStorage());
    m_BaoQiLog.reset(new PlayerBaoQiLog());
    m_bag.reset(new ItemArray());
    m_questLogger.reset(new QuestLogger());
    m_behaviorCounter.reset(new PlayerBehaviorCounter( m_questLogger.get()));
    m_playStringGroup.reset(new PlayerStringGroup());
	m_buildingLogMgr.reset(new BuildingLogMgr());
	m_timerEventGroup.reset(new TimerEventGroup());
    m_worldMapLogic.reset(new PlayerMapLogic());
    m_Technology.reset(new PlayerTechnology());
	m_DailyQuest.reset(new DailyQuestLog());
	m_GrowUpQuest.reset(new GrowUpQuestLog());
	m_SignUpLog.reset(new SignUpLog());
	m_PassStageReward.reset(new PassStageReward());
	m_AutoCountryWarLog.reset(new PlayerAutoCountryWarLogic());
	m_CongratulateLog.reset(new CongratulateLog());
	m_WannaBeStrongerLog.reset(new WannaBeStrongerLog());

    m_Buff.reset(new CBuff());
	RegistRecordProvideFunction(); 
	SetDataIsLost( false ) ;
	
//#ifdef _MMO_SERVER_
	//ServiceMgr::Register(SERVICE_BUY_RESET_TIMER, this);
	//ServiceMgr::Register(SERVICE_GOV_UPDATE_TIME,this);
//#endif

	m_watchSecond.SetMax(1000);
	m_BuyTimeUpdate.SetMax(GET_BASE_DEF_UINT(pb::BD_PLAYER_REGAIN_MARKET_BUY_NUM_MINUTE) * 60 * 1000);

}

Player::~Player( void )
{
	m_timerEventGroup->Detach();
//#ifdef _MMO_SERVER_
//	ServiceMgr::UnRegister(SERVICE_BUY_RESET_TIMER, this);
//#endif
	DeleteAllPtrInLFQueue<NetPack*>( m_packetTable ) ;
}

void Player::Update( uint64 nowTimeMS, int64 diffMS )
{
	//LLOG("player update");
	//int64 curS = nowTimeMS / 1000;
	//uint64 begintime =sOS.TimeMS();
#ifdef _MMO_SERVER_
	UpdatePlayerValue(); 
    PlayerItemLogic::SendBagModify( this);
	//uint64 up1 =sOS.DiffTimeMS(begintime);
	//uint64 temp1 = sOS.TimeMS();
#endif
    PlayerCharacterLogic::SendCharacterStorageModify( this);
	if(m_AutoCountryWarLog)
		m_AutoCountryWarLog->Update(this,nowTimeMS);
	UpHandlePacket();
//#endif
#ifdef _MMO_SERVER_

	/*uint64 temp2 = sOS.TimeMS();
	uint64  up2 = sOS.DiffTimeMS(temp1);*/

	//temp1 = sOS.TimeMS();
	//uint64  up3 =sOS.DiffTimeMS(temp2);

	m_watchSecond.Update(diffMS);
	if(m_watchSecond.Done())
	{
		DoUpdatePer_Second_Zero(nowTimeMS);
		m_watchSecond.Reset();
	}
	//temp2 = sOS.TimeMS();
	//uint64 up4 =sOS.DiffTimeMS(temp1);
	m_BuyTimeUpdate.Update(diffMS);
	if(m_BuyTimeUpdate.Done())
	{
		uint64 maxtime = m_marketLog->UpdateBuyTimes(this);
		m_BuyTimeUpdate.SetMax(maxtime);
		m_BuyTimeUpdate.Reset();
		//m_maxUpdatet = 0;
		//m_maxMinuupdatet = 0;
	}
	//temp1 = sOS.TimeMS();
	//uint64 up5 = sOS.DiffTimeMS(temp2);
	m_GovAffairUpdate.Update(diffMS);
	if(m_GovAffairUpdate.Done())
	{
		uint64 maxtime = m_GovAffairsLog->UpdateZhengWuTimes(this);
		m_GovAffairUpdate.SetMax(maxtime);
		m_GovAffairUpdate.Reset();
		//m_maxUpdatet = 0;
		//m_maxMinuupdatet = 0;
	}
	//temp1 = sOS.TimeMS();
	//uint64 up6 = sOS.DiffTimeMS(temp2);
	m_ActivityUpdate.Update(diffMS);
	if(m_ActivityUpdate.Done())
	{
		DoUpdatePer_Minute_Zero();
		m_ActivityUpdate.SetMax(60 * 1000);
		m_ActivityUpdate.Reset();
		//m_maxUpdatet = 0;
		//m_maxMinuupdatet = 0;
	}
	//uint64 up7 =sOS.DiffTimeMS(temp1);
	//uint64 total =sOS.DiffTimeMS(begintime);

	//if (total > m_maxUpdatet)
	//{
	//	m_maxUpdatet = total;
	//	NLOG( "--------Player up1:%llu,up2:%llu,up3:%llu,up4:%llu,up5:%llu,up6:%llu,up7:%llu,total:%llu",up1,up2,up3,up4,up5,up6,up7,total) ;
	//}
#endif
}

void Player::UpHandlePacket()
{
	NetPack *pPacket         =NULL ;
	while(GetOnePacket( pPacket ) )
	{
		if( !HandlePack( *pPacket ) )
		{
			ELOG ("%s, player id:%llu receive not handle packet from %s, %s(%d) ! "
				, CharName().c_str(),GetGuid(), pPacket->GetPacketTypeName(),pPacket->GetOpcodeName(),pPacket->GetOpcode() );
		}

		SAFE_DEL(pPacket);
	}
}

bool Player::IsOpenChatChannel(uint32 chatType)
{

	return true;
}


void Player::SaveTo(pb::PlayerShowData &data)
{
    data.set_player_id(GetGuid());
    data.set_level(GetLevel());
    data.set_name(CharName());
	data.set_player_head_id(GetHeadIconId());
	data.set_vip_lv(GetVipLevel());
	data.set_military_power(getMilitaryPower());
}
void Player::SaveTo(uint64 playerId, pb::PlayerShowData &data)
{
    if (Player* player = sPlayerPool.GetByPlayerId(playerId))
    {
        player->SaveTo(data);
    }
}
void Player::WritePlayerData(pb::PlayerAllData &data )
{
	SaveValue( *data.mutable_full_value() ) ;
    m_bag->SaveTo( *data.mutable_bag_info());    
    m_characterStorage->SaveTo( *data.mutable_character_info());
    m_questLogger->Save(this, *data.mutable_quest_log());
    m_behaviorCounter->SaveTo( *data.mutable_behavior_count());
}

void Player::WritePlayerCrossWarData(pb::PlayerAllData& data)
{
	SaveValue( *data.mutable_full_value() ) ;
	m_bag->WriteAllItemTo( *data.mutable_bag_info());    
	m_characterStorage->SaveTo( *data.mutable_character_info());
	m_questLogger->Save(this, *data.mutable_quest_log());
	m_behaviorCounter->SaveTo( *data.mutable_behavior_count());
	m_worldMapLogic->SaveTo(*data.mutable_world_map_logic());
	m_BaoQiLog->SaveTo(*data.mutable_bao_qi_info());
	m_Technology->SaveTo(*data.mutable_technology_info());
	m_DailyQuest->SaveTo(*data.mutable_dailyquest_info());
	m_SignUpLog->SaveToDB(*data.mutable_sign_up_info());
	m_PassStageReward->SaveTo(*data.mutable_pass_stage_reward_info());
	m_GrowUpQuest->SaveTo(*data.mutable_growupquest_info());
	m_GrowUpQuest->SaveTo(*data.mutable_growupquestgroup_info());
	data.mutable_other_info()->set_channel_id(GetChannelId());
	data.mutable_other_info()->set_platform_id(GetPlatformId());
}

void Player::LoadPlayerDynamicData(pb::PlayerAllData& data)
{
#ifdef _MMO_SERVER_
	if (data.has_mails())
	{
		if (m_mailBox)
		{
			m_mailBox->LoadFrom(this,*data.mutable_mails());
		}
	}
	
#endif
}
// void Player::IntergalPointAward(uint32 TypeAward,uint32 num)
// {
// 	sReward.Change(*this,TypeAward,num);
// }

void Player::LoadCrossPlayerData(const pb::PlayerAllData& data)
{
	LoadPlayerData(data);
}
void Player::LoadPlayerData(const pb::PlayerAllData& data)
{
#ifdef _MMO_SERVER_
    if (data.has_fuben_info())
    {
        m_fubenLog->LoadFrom(data.fuben_info());
    }
    if (data.has_market_info())
    {
        m_marketLog->LoadFrom(data.market_info());
	}
    if (data.has_altar_info())
    {
        m_AltarLog->LoadFrom(data.altar_info());
    }
    if (data.has_smithy_info())
    {
        m_SmithyLog->LoadFrom(data.smithy_info());
    }
	if(data.has_gov_affairs_info())
	{
		m_GovAffairsLog->loadFrom(data.gov_affairs_info());
	}
	if(data.has_achievement_info())
	{
		m_PalaceAchievementLog->LoadFrom(data.achievement_info());
	}
	if(data.has_continue_info())
	{
		m_ContinueOccupyCityLog->LoadFrom(data.continue_info());
	}
#endif
	if (data.has_technology_info())//科技信息先加载
	{
		m_Technology->LoadFrom(data.technology_info());
	}
	if(data.has_dailyquest_info())
	{
		m_DailyQuest->LoadFrom(data.dailyquest_info());
	}
	if(data.has_growupquest_info())
	{
		m_GrowUpQuest->LoadFrom(data.growupquest_info());
		m_GrowUpQuest->LoadFrom(data.growupquestgroup_info());
	}
	if(data.has_sign_up_info())
	{
		m_SignUpLog->LoadFromDB(data.sign_up_info());
	}
	if(data.has_pass_stage_reward_info())
	{
		m_PassStageReward->LoadFrom(data.pass_stage_reward_info());
	}
	if (data.has_bao_qi_info())
    {
        m_BaoQiLog->LoadFrom(data.bao_qi_info());
    }
    if (data.has_buff_info())
    {
        m_Buff->LoadFrom(data.buff_info());
    }
	if (data.has_full_value())
	{
		LoadValue(data.full_value());
	}        
    if (data.has_bag_info())
    {
        m_bag->LoadFrom( data.bag_info());
    }                                 
    if (data.has_character_info())
    {
        m_characterStorage->LoadFrom(this, data.character_info());
    }

	if (data.has_world_map_logic())		//世界地图系统
	{
		m_worldMapLogic->LoadFrom(this, data.world_map_logic());
	}
#ifdef _MMO_SERVER_
    if ( data.has_string_data())
    {
        m_playStringGroup->LoadFrom(data.string_data());
    }
	if (data.has_dungeon_info())
	{
		m_DungeonLog->LoadFrom(data.dungeon_info());
	}
	if(data.has_activity_star_box_info())
	{
		m_ActivityStarBoxLog->LoadFrom(data.activity_star_box_info());
	}
	if(data.has_world_fight_achieve_info())
	{
		m_WorldFightAchievementLog->LoadFrom(data.world_fight_achieve_info());
	}
	if(data.has_hero_reward())
	{
		m_ActivityHeroRewardLog->LoadFrom(this,data.hero_reward());
	}
#endif
	if(data.has_seige_force_info())
	{
		m_SeigeForceLog->LoadFrom(data.seige_force_info());
	}

	if( data.has_other_info() )
	{
		SetChannelId( data.other_info().channel_id() ) ;
		SetPlatformId( data.other_info().platform_id() ) ;
	}     
#ifdef _MMO_SERVER_
	if (data.has_quest_log())
	{
		m_questLogger->Load(this,data.quest_log());
	}
#endif
	if (data.has_behavior_count())
	{
		m_behaviorCounter->LoadFrom(data.behavior_count());
	}
	if(data.has_continue_info() && data.congratulate_info().info_size() > 0)
	{
		m_CongratulateLog->LoadFrom(data.congratulate_info());
		const uint64 begin_time = m_CongratulateLog->GetCongratulateStructMapBeginTime();
		if(begin_time != 0)////恭贺功能
		{
#ifdef _MMO_SERVER_
			const uint64 time_now = sOS.TimeSeconds();
			if(ONE_DAY_SECOND >= (time_now - begin_time))
			{
				sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursPasted, this,begin_time),(U32)(ONE_DAY_SECOND - (time_now - begin_time)));
			}
			else
			{
				When24HoursPasted(begin_time);
			}
#endif
		}
	}
	if(data.has_con_been_congratulated() && data.con_been_congratulated().info_size() > 0)
	{
		m_CongratulateLog->LoadFrom(data.con_been_congratulated());
		const uint64 begin_time = m_CongratulateLog->GetCongratulateBeenStructMapBeginTime();
		if( begin_time != 0)////恭贺功能
		{
#ifdef _MMO_SERVER_
			const uint64 time_now = sOS.TimeSeconds();
			if(ONE_DAY_SECOND >= (time_now - begin_time))
			{
				sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursToPlayerPasted, this,begin_time),(U32)(ONE_DAY_SECOND - (time_now - begin_time)));
			}
			else
			{
				When24HoursToPlayerPasted(begin_time);
			}
#endif
		}
	}
	if(data.has_wanna_be_info())
	{
		m_WannaBeStrongerLog->LoadFrom(data.wanna_be_info());
	}
	SetInCrossWar(false);	//玩家登陆，则置玩家不在跨服中
	m_characterStorage->BattleCharAttrResize(m_characterStorage->GetBattleMemberCount(this));
	ItemCharacterLogic::ReCalculateBattleAttr(this);
	while(!box_queue_.empty() && GetAttrUint(pb::PLAYER_FIELD_TAKE_EXPLOIT_BOX_STEP) >= box_queue_.front().id)
	{
		box_queue_.pop_front();//防止服务器宕机，而多发宝箱
	}
}

void Player::UpdateCrossPlayerData(const pb::PlayerAllData& data)
{
	if (data.has_technology_info())//科技信息先加载
	{
		m_Technology->LoadFrom(data.technology_info());
	}
	//if(data.has_dailyquest_info())
	//{
	//	m_DailyQuest->LoadFrom(data.dailyquest_info());
	//}
	//if(data.has_growupquest_info())
	//{
	//	m_GrowUpQuest->LoadFrom(data.growupquest_info());
	//	m_GrowUpQuest->LoadFrom(data.growupquestgroup_info());
	//}
	//if(data.has_sign_up_info())
	//{
	//	m_SignUpLog->LoadFromDB(data.sign_up_info());
	//}
	//if(data.has_pass_stage_reward_info())
	//{
	//	m_PassStageReward->LoadFrom(data.pass_stage_reward_info());
	//}
	//if(data.has_activity_star_box_info())
	//{
	//	m_ActivityStarBoxLog->LoadFrom(data.activity_star_box_info());
	//}
	if (data.has_bao_qi_info())
	{
		m_BaoQiLog->LoadFrom(data.bao_qi_info());
	}
	//if (data.has_buff_info())
	//{
	//	m_Buff->LoadFrom(data.buff_info());
	//}
	if (data.has_full_value())
	{
		LoadValue(data.full_value());
	}        
	if (data.has_bag_info())
	{
		m_bag->DestroyAll();
		m_bag->LoadFrom( data.bag_info());
	}                                 
	if (data.has_character_info())
	{
		m_characterStorage->Synchronization(this, data.character_info());
	}

	//if (data.has_world_map_logic())		//世界地图系统
	//{
	//	m_worldMapLogic->LoadFrom(data.world_map_logic());
	//}
	//m_characterStorage->BattleCharAttrResize(m_characterStorage->GetBattleMemberCount(this));
	ItemCharacterLogic::ReCalculateBattleAttr(this);
	//while(!box_queue_.empty() && GetAttrUint(pb::PLAYER_FIELD_TAKE_EXPLOIT_BOX_STEP) >= box_queue_.front().id)
	//{
	//	box_queue_.pop_front();//防止服务器宕机，而多发宝箱
	//}
	//if( data.has_other_info() )
	//{
	//	SetChannelId( data.other_info().channel_id() ) ;
	//	SetPlatformId( data.other_info().platform_id() ) ;
	//}   
}

void Player::AddValue(uint32 index,int value){
//如果value是负值，new_value 需要做<=0的判断
	int32 old_value = GetAttrInt(index);
	int32 new_value = old_value + value;
	if(index == pb::PLAYER_FIELD_DINNER_NUMBER){//宴会次数上限
		if(new_value >= static_cast<int32>(GetAttrUint(pb::PLAYER_FIELD_SUM_DINNER_NUMBER)))
			new_value = GetAttrUint(pb::PLAYER_FIELD_SUM_DINNER_NUMBER);
	}
	if(index == pb::PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX){//普通宝箱上限
		if(new_value >= static_cast<int32>(GetAttrUint(pb::PLAYER_FIELD_NORMAL_EXPLOIT_BOX_LIMIT)))
			new_value = GetAttrUint(pb::PLAYER_FIELD_NORMAL_EXPLOIT_BOX_LIMIT);
	}
	if(index == pb::PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX){//高级宝箱上限
		if(new_value >= static_cast<int32>(GetAttrUint(pb::PLAYER_FIELD_ADVANCE_EXPLOT_BOX_LIMIT)))
			new_value = GetAttrUint(pb::PLAYER_FIELD_ADVANCE_EXPLOT_BOX_LIMIT);
	}
	if(new_value <= 0)
	{
		new_value = 0;
	}
	SetValueInt(index,new_value);
}

void Player::AddValue64(uint32 index,uint64 value)
{
	uint64 old_value = GetAttrUint64(index);
	uint64 new_value = old_value + value;
	SetValueUint64(index,new_value);
}

bool Player::IsOnline() const
{
	if (m_pSession)
	{
		return !m_pSession->GetIsHadClosed();
	}
	return false;
}

void Player::SetSession(Session *pSession)
{
	m_pSession =pSession ;
}

void Player::CloseSession()
{
	if (m_pSession)
	{
		m_pSession->Kick(pb::RELOGIN);
		m_pSession->SetIsNeedClose(true);
	}
}

void Player::Send( NetPack &pack ) const
{
	if( m_pSession !=NULL )
	{
#ifdef _MMO_SERVER_
		m_pSession->Send( pack ) ;
#elif _SERVER_CROSS_
		m_pSession->Send( pack,GetGuid(),PACKET_TYPE_CROSS_CLIENT ) ;
#endif
	}
}

void Player::SendToServer( NetPack &pack,char clientType) const
{
	if( m_pSession !=NULL )
	{
#ifdef _MMO_SERVER_
		m_pSession->Send( pack ) ;
#elif _SERVER_CROSS_
		m_pSession->Send( pack,GetGuid(),clientType ) ;
#endif
	}
}

void Player::Send( int opCode, const ::google::protobuf::Message &msg )
{
	if(  m_pSession !=NULL )
	{
		//PLOG("send have session");
#ifdef _MMO_SERVER_
		if(IsInCrossWar())
		{
			if(!sPlayerDataMgr.CheckGameClientBlackList(static_cast< pb::Opcode >(opCode)))
			{
				LLOG("one message not send, code=%d",opCode);
				return;
			}
		}
		m_pSession->Send( opCode, msg ) ;
#elif _SERVER_CROSS_
		m_pSession->Send( opCode, GetGuid(),PACKET_TYPE_CROSS_CLIENT,msg ) ;
#endif
	}
}

void Player::Send(int opCode)
{
	NetPack pack(opCode);
	Send(pack);
}

void Player::SendToServer( int opCode, const ::google::protobuf::Message &msg,char clientType ) const
{
	if(  m_pSession !=NULL )
	{
		//PLOG("send have session");
#ifdef _MMO_SERVER_
		m_pSession->Send( opCode, msg ) ;
#elif _SERVER_CROSS_
		m_pSession->Send( opCode, GetGuid(),clientType,msg ) ;
#endif
	}
}

void Player::SendGError(GErrorCode errCode)
{
	NetPack pack(pb::SMSG_GERROR);
	pack << (uint32)NotInGuild;
	Send(pack);
}

void Player::SendErrorCode(pb::CxGS_ERROR_CODE errCode)
{
	NetPack pack(pb::SMSG_GERROR_CODE);
	pack << (uint32)errCode;
	Send(pack);
}

bool Player::CanAddItemToBag(int entry)
{
	return m_bag->CanAddItemToBag(entry,GetBagCapacity());
}

bool Player::IsRechBagCapcity()
{
	if (m_bag->CurBagItemCount() >= GetBagCapacity())
	{
		return true;
	}
	return false;
}

int Player::CreateItem( pb::ITEM_REASON reason, int entry, int count /*= 1*/, std::list< Item *>* outList/* = NULL*/,bool is_auto_give_reward /*= false*/)
{
    const LogicItemProto* proto = sItemMgr.Find(entry);
    if (proto == NULL) {
        ELOG("No item with entry %d", entry);
        return pb::ErrInvalidProto;
    }

    int retCode = pb::ErrCommonFail;
    do 
    {
        if (count == 0)
        {
            retCode = pb::ErrInvalidParam;
            break;
        }

        if (proto->IsCard())
        {
            for (int i = 0; i < count; ++i)
            {
                PlayerCharacterLogic::CreateCharacterForPlayer(this, proto->ObjectValue());
            }

        }
        else if (proto->IsCurrency())
        {
			if (proto->IsPlayerExp())
			{
				AddXP(count);
				retCode = pb::ErrCommonSuccess;
				break;
			}
			else if (proto->IsVipExp())
			{
				AddVipXp(count);
				retCode = pb::ErrCommonSuccess;
				break;
			}
			else
			{
				AddCurrency(reason, proto->category_3, count);
				retCode = pb::ErrCommonSuccess;
				break;
			}
        }
        else if (proto->IsBoxImm())
        {
            for (int i = 0; i < count; ++i)
            {
                PlayerItemLogic::PlayerLootImmBox(this, *proto);
            }
        }
        else if (proto->CanBeCreateToBag())
        {
			if(!is_auto_give_reward)
			{
				if(!CanAddItemToBag(entry))
				{
					retCode = pb::ErrRechBagCapacity;
					break;
				}
			}
			std::list<Item*> sell_list;
			if (!m_bag->CreateItem(proto,GetBagCapacity(), count,outList,&sell_list))
			{
				retCode = pb::ErrCommonFail;
				break;
			}
			else
			{
				//成长任务
				uint32 equip_count = m_bag->GetQualityEquipOfSuitCount(proto->Quality());
				if(proto->Quality() == ItemEnum::QUALITY_3) //红
				{
					UpdateAllGrowUpQuestInfo(pb::RED_EQUIP,equip_count);
				}
				if(proto->Quality() == ItemEnum::QUALITY_4)//紫
				{
					UpdateAllGrowUpQuestInfo(pb::PURPLE_EQUIP,equip_count);
				}
				if(proto->Quality() == ItemEnum::QUALITY_5)//橙
				{
					UpdateAllGrowUpQuestInfo(pb::ORANGE_EQUIP,equip_count);
				}
				//UpdateWannaBeStrongerInfo();
				if(!sell_list.empty())
				{
					for(std::list<Item*>::iterator iter = sell_list.begin();iter != sell_list.end();++iter)
					{
						if(is_auto_give_reward)
						{
							SellSlotItem(pb::IR_BAG_RECH_CAPACITY_GET,(*iter)->BagSlot(),true);
						}
						else
							SellSlotItem(pb::IR_BAG_RECH_CAPACITY_GET,(*iter)->BagSlot());
					}
				}
			}
        }
		else
        {
            retCode = pb::ErrCommonFail;
            break;
        }

        AddEventLog(EVENT_PALYER_ITEM_SYS,
            4,
            GetLevel(),
            entry,
            count,
            reason);

        retCode = pb::ErrCommonSuccess;
        break;

    } while (0);
#ifdef _MMO_SERVER_
    //物品添加成功后的处理
    if (retCode == pb::ErrCommonSuccess)
    {
        if (proto->IsSuitScrap())
        {
            m_marketLog->_AddSuitsPart(entry);
        }
    }
#endif
    return retCode;
}

int Player::CreateSysMailItem(pb::ITEM_REASON source ,  const ItemID2Count& itemMap)
{
	int lastErr = pb::ErrCommonSuccess;
	ItemPtrList sellList;
	for ( ItemID2Count::const_iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if (it->second != 0)
		{
			int err_code = CreateItem( source, it->first, it->second,NULL,true);
			if ( err_code != pb::ErrCommonSuccess)
			{
// 				if (err_code == pb::ErrRechBagCapacity)
// 				{
// 					if (!m_bag->CreateItem(it->first, it->second, &sellList))
// 					{
// 						err_code = pb::ErrCommonFail;
// 					}
// 				}
				lastErr = err_code;
			}
		}
	}

// 	for (ItemPtrList::iterator iterSell = sellList.begin(); iterSell != sellList.end(); ++iterSell)
// 	{
// 		Item* curItem = (*iterSell);
// 		SellSlotItem(source,curItem->BagSlot(),true);
// 	}
	return lastErr;

}

int Player::CreateMailItem(pb::ITEM_REASON source ,  const IntV3Vec& v3Vec)
{
	int lastErr = pb::ErrCommonSuccess;
	ItemPtrList sellList,createList;
	for ( IntV3Vec::const_iterator it = v3Vec.begin(); it != v3Vec.end(); ++it)
	{
		const IntV3& curItem = (*it);
		int err_code = CreateItem( source, curItem.first, curItem.second,&createList,true);
		if ( err_code != pb::ErrCommonSuccess)
		{
// 			if (err_code == pb::ErrRechBagCapacity)
// 			{
// 				if (!m_bag->CreateItem(curItem.first, curItem.second, &sellList))
// 				{
// 					err_code = pb::ErrCommonFail;
// 				}
// 			}
			lastErr = err_code;
		}
		else if (err_code == pb::ErrCommonSuccess)
		{
			if (curItem.third.length() > 1)
			{
				for (ItemPtrList::iterator iterCreat = createList.begin(); iterCreat != createList.end(); ++iterCreat)
				{
					Item* pItem = (*iterCreat);
					if (pItem->IsEquip())
					{
						ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
						if ( item_equip)
						{
							IntPairVec retVec;
							Utility::SplitAttrLv(curItem.third,retVec);
							item_equip->UpdateAttrLv(retVec);
						}
					}
				}
			}
			createList.clear();
		}
	}

// 	for (ItemPtrList::iterator iterSell = sellList.begin(); iterSell != sellList.end(); ++iterSell)
// 	{
// 		Item* curItem = (*iterSell);
// 		SellSlotItem(source,curItem->BagSlot(),true);
// 	}
	return lastErr;

}

int Player::CreateGMItemWithAttr(ITEM_REASON reason, int itemId, std::vector<int> attr)
{
	ItemPtrList itemlst;
	int lastErr = pb::ErrCommonSuccess;
	int err_code = CreateItem( reason, itemId,1,&itemlst);
	if(err_code == pb::ErrCommonSuccess)
	{
		Item* pItem = itemlst.front();
		if(pItem && pItem->IsEquip())
		{
			ItemEquip* item_equip = dynamic_cast<ItemEquip*>( pItem);
			
			int nFreshCount = sItemMgr.GetItemMaxStarCount(item_equip->Proto()->id) - 1;	//前几个星
			int nMaxXiLianLevel = sItemMgr.GetItemMaxXiLianLevel(item_equip->Proto()->id);
			IntPairVec retvec;
			int count = 0;
			for(std::vector<int>::iterator it = attr.begin(); it != attr.end() && count < nFreshCount; ++it,count++)
			{
				retvec.push_back(std::make_pair(*it,nMaxXiLianLevel));
			}
			item_equip->UpdateAttrLv(retvec);
		}
	}

	return lastErr;
}

int Player::CreateItem(  pb::ITEM_REASON source , const ItemID2Count& itemid2count)
{
    int lastErr = pb::ErrCommonSuccess;
    for ( ItemID2Count::const_iterator it = itemid2count.begin(); it != itemid2count.end(); ++it)
    {
        if (it->second != 0)
        {
            int err_code = CreateItem( source, it->first, it->second);
            if ( err_code != pb::ErrCommonSuccess)
            {
                lastErr = err_code;
            }
        }   
    }
    return lastErr;
}

void Player::GMPayOrder(ProductId proId)
{
#ifdef _MMO_SERVER_
	PayItem*  pItem = sGameSvr.m_payItemsInfo.GetPayItem(proId);
	if (pItem)
	{
		pb::GXDB_PayOrder payOder;
		payOder.set_is_new(true);
		payOder.set_crystal_final(pItem->iVal.reward_diamond);
		payOder.set_vip_xp(pItem->iVal.reward_diamond);
		payOder.set_product_id(proId);
		TakePayOrderResult(payOder);
		int rand = sOS.Rand(0, 5);
		uint32 realNeedMoney = (uint32)(pItem->iVal.need_rmb*100.f);
		sEventLogClient.SendEventInfo( pb::EVENT_PLAYER_PAY_OK, GetGuid(),0, 5, "i", rand, "s", "GMTestPayOrder", "u", realNeedMoney,  "u", payOder.crystal_final(),"u", GetPlatformId() ) ;
	}
#endif
}

void Player::TakePayOrderResult( const pb::GXDB_PayOrder &info )
{
#ifdef _MMO_SERVER_
    if ( info.is_new())
    {
		NLOG( "TakePayOrderResult:crystal:%u,vix_xp:%u", info.crystal_final(),info.vip_xp()) ;
        AddVipXp( info.vip_xp() * GET_BASE_DEF_UINT(BD_VIP_GOLD_TO_EXP));
		if(std::strcmp(info.product_id().c_str(),MONTH_CARD_NORMAL) == 0)
		{
			SetBuyNormalMonthCardTime(sOS.TimeSeconds());
			AddCurrency( pb::IR_IAP_GET, eCashGold, info.crystal_final() ) ;
		}
		else if(std::strcmp(info.product_id().c_str(),MONTH_CARD_ADVANCE) == 0)
		{
			SetBuyAdvanceMonthCardTime(sOS.TimeSeconds());
			AddCurrency( pb::IR_IAP_GET, eCashGold, info.crystal_final() ) ;
		}
		else
		{
			if(TestFlag(pb::PLAYER_FLAG_HAD_FIRST_PAY))
			{
				AddCurrency( pb::IR_IAP_GET, eCashGold, info.crystal_final() ) ;
			}
			else
			{
				SetFlag( pb::PLAYER_FLAG_HAD_FIRST_PAY);
				AddCurrency( pb::IR_IAP_GET, eCashGold, info.crystal_final()*2) ;
			}
		}
		
		if(IsActivityStillActive(pb::ACTIVITY_MONEY_TO_GOLD))
		{
			AddValue(pb::PLAYER_FIELD_ACTIVITY_GOLD_NUM,info.crystal_final());
		}
    }
    else
    {
        MASSERT(false, "Pay Order Not New!")
    }
#endif
}


void Player::UpdatePlayerValue()
{
    if ( PlayerValueOwner::HasUpdateInfo())
    {
        pb::ObjectValueUpdate msg;
        PlayerValueOwner::WriteUpdateInfoTo( msg);

        Send( pb::SMSG_PLAYER_VALUE_UPDATE, msg );
        sDbProxyClient.SendPlayerPack(*this, pb::SG2D_UPDATE_PLAYER_VALUE, msg);


        if ( msg.has_update_int32())
        {
            const pb::Int32ListUpdate& up = msg.update_int32();
            uint32 mask = up.bit_masks(0);
            bool b1 = Utility::TestBit( mask, PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_COUNT);

            if ( b1)
            {
                DLOG( "###########################################");
                DLOG( "####### send 7 day count");
                DLOG( "###########################################");
            }
            bool b2 = Utility::TestBit( mask, PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_COUNT);
            if ( b2 )
            {
                DLOG( "###########################################");
                DLOG( "####### send monthly day count");
                DLOG( "###########################################");
            }
        }


        PlayerValueOwner::SetUnmodified();
    }
}

void Player::RegistRecordProvideFunction()
{
#ifdef _MMO_SERVER_
	// 任务系统的取值回调

    RegistDefault( boost::bind(&PlayerBehaviorCounter::GetCount, m_behaviorCounter.get(), _1, _2 ));

    // 按 condition 取值
    RegistGetterWithParam( 
        BehaviorType( QC1_CHAR, QC2_CHAR_LEVEL_REACH, QC3_UNDEFINED).m_counterType,
        boost::bind(&CharacterStorage::GetCountBAMatchLevel, (this->m_characterStorage).get(), _1) );
    RegistGetterWithParam( 
        BehaviorType( QC1_CHAR, QC2_CHAR_QUALITY_REACH, QC3_UNDEFINED).m_counterType,
        boost::bind(&CharacterStorage::GetCountBAMatchQuality, (this->m_characterStorage).get(), _1) );
    RegistGetterWithParam( 
        BehaviorType( QC1_CHAR, QC2_CHAR_REINFORCE_REACH, QC3_UNDEFINED).m_counterType,
        boost::bind(&CharacterStorage::GetCountBAMatchReinforce, (this->m_characterStorage).get(), _1) );
    RegistGetterWithParam( 
        BehaviorType( QC1_CHAR, QC2_CHAR_SKILL_LEVEL_REACH, QC3_UNDEFINED).m_counterType,
        boost::bind(&CharacterStorage::GetCountBAMatchSkillLv, (this->m_characterStorage).get(), _1) );
    RegistGetterWithParam(
        BehaviorType(QC1_PVE, QC2_FUBEN_STAGE, QC3_FUBEN_PASS).m_counterType,
        boost::bind(&FubenLog::IsPassStage, (this->m_fubenLog).get(), _1));
// 	RegistGetterWithParam(
// 		BehaviorType(QC1_PVE, QC2_FUBEN_SOURECE, QC3_FUBEN_PASS).m_counterType,
// 		boost::bind(&FubenLog::TakeResouceCount, (this->m_fubenLog).get(), _1));
    // 装备
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_ITEM_ID, QC3_EQUIP_BUY).m_counterType, //购买特定装备
        boost::bind(&ItemArray::GetCountItem, (this->m_bag).get(), _1));
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_ITEM_ID, QC3_EQUIP_WEAR).m_counterType, //穿戴特定装备
        boost::bind(&CharacterStorage::GetWearEquip, (this->m_characterStorage).get(), _1));
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY, QC3_EQUIP_GET).m_counterType, //获得：特定品质及以上
		boost::bind(&ItemArray::GetQualityAndStarCount, (this->m_bag).get(), _1,0));
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY, QC3_EQUIP_WEAR).m_counterType, //穿戴：特定品质及以上
        boost::bind(&CharacterStorage::WearQualityAndStarCount, (this->m_characterStorage).get(),this, _1,0));
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY_STAR, QC3_EQUIP_GET).m_counterType, //获得：特定品质及以上、1星及以上装备
        boost::bind(&ItemArray::GetQualityAndStarCount, (this->m_bag).get(), _1, 1));
    RegistGetterWithParam(
        BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY_STAR, QC3_EQUIP_WEAR).m_counterType, //穿戴：特定品质及以上、1星及以上装备
        boost::bind(&CharacterStorage::WearQualityAndStarCount, (this->m_characterStorage).get(),this, _1, 1));
	RegistGetterWithParam(
		BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY_AND_STARS, QC3_EQUIP_WEAR).m_counterType, //穿戴：特定品质、任意星级装备
		boost::bind(&CharacterStorage::WearQualityAndAnyStar, (this->m_characterStorage).get(),this, _1, 0));
	RegistGetterWithParam(
		BehaviorType(QC1_EQUIP, QC2_EQUIP_QUALITY_AND_STARS, QC3_EQUIP_GET).m_counterType, //获得：特定品质、任意星级装备
		boost::bind(&ItemArray::GetQualityAndAnyStar, (this->m_bag).get(),_1, 0));

	RegistGetterWithParam(
		BehaviorType(QC1_EQUIP, QC2_EQUIP_CLASS, QC3_EQUIP_GET).m_counterType, //获得：特定类型
		boost::bind(&Player::CountEquipClass, this, _1));
	RegistGetterWithParam(
		BehaviorType(QC1_EQUIP, QC2_EQUIP_CLASS, QC3_EQUIP_WEAR).m_counterType, //穿戴：特定类型
		boost::bind(&Player::WearEquipClass, this, _1));
	RegistGetterWithParam(
		BehaviorType(QC1_EQUIP, QC2_EQUIP_CLASS, QC3_EQUIP_HAVE).m_counterType, // 拥有：特定类型
		boost::bind(&Player::HaveEquipClass, this, _1));
    // 建筑
    RegistGetterWithParam(
        BehaviorType(QC1_BUILDING, QC2_BUILDING_TYPE, QC3_BUILDING_LEVEL).m_counterType, //已解锁建筑等级达到
        boost::bind(&BuildingLogMgr::GetUnlockBuildingCurlv, (this->m_buildingLogMgr).get(), _1));
    RegistGetterWithParam(
        BehaviorType(QC1_BUILDING, QC2_BUILDING_TYPE, QC3_BUILDING_OUTPUT).m_counterType, //产量达到
        boost::bind(&Player::GetBuildingIncome, this, _1,true));
	RegistGetterWithParam(
		BehaviorType(QC1_BUILDING, QC2_BUILDING_ID, QC3_BUILDING_LEVEL).m_counterType, //特定建筑等级达到
		boost::bind(&BuildingLogMgr::GetBuildCurLv, (this->m_buildingLogMgr).get(), _1));
    // 宝器
    RegistRecordGetter(
        BehaviorType(QC1_BAO_QI, QC2_UNDEFINED, QC3_BAO_QI_LEVEL).m_counterType, //升级
        boost::bind(&PlayerBaoQiLog::GetMaxLevel, (this->m_BaoQiLog).get()));
// 	RegistRecordGetter(
// 		BehaviorType(QC1_OPERATE, QC2_CHANGE_NAME, QC3_UNDEFINED).m_counterType, //取名
// 		boost::bind(&Player::ChangeName,this,_1));
    // 城池
        //攻占任意城池(客户端行为)
    // 武将
        //任意武将移动(客户端行为)
        //特定武将移动(客户端行为)
    RegistGetterWithParam(
        BehaviorType(QC1_GET_EXPLOIT, QC2_MAIN_QUEST, QC3_UNDEFINED).m_counterType, //获得功勋
        boost::bind(&Player::GetMainQuestExploitValue,this));
	RegistGetterWithParam(
		BehaviorType(QC1_GET_EXPLOIT, QC2_BRANCH_QUEST, QC3_UNDEFINED).m_counterType, //获得功勋
		boost::bind(&Player::getBranchQuestExploitValue,this));
    RegistGetterWithParam(
        BehaviorType(QC1_HERO, QC2_HERO_TABLE_ID, QC3_HERO_UNLOCK).m_counterType, //特定武将解锁
        boost::bind(&CharacterStorage::HasCharacterProto, (this->m_characterStorage).get(), _1));
	RegistGetterWithParam(
		BehaviorType(QC1_HERO, QC2_HERO_TABLE_ID, QC3_HERO_ZHAOMU).m_counterType, //特定武将招募
		boost::bind(&CharacterStorage::CharacterHasZhaoMu, (this->m_characterStorage).get(), _1));
    // 操作(客户端行为)
// 	RegistGetterWithParam(
// 		BehaviorType(QC1_OPERATE, QC2_OPERATE_NAMED, QC3_UNDEFINED).m_counterType, ////取名
// 		boost::bind(&PlayerMarketLog::Collect, (this->m_marketLog).get(),this, _1));


    // 科技
	RegistGetterWithParam(
        BehaviorType(QC1_TECHNOLOGY, QC2_TECHNOLOGY_TABLE_ID, QC3_TECHNOLOGY_STUDY).m_counterType, //研究
        boost::bind(&PlayerTechnology::StudyTec,(this->m_Technology).get(),this, _1));
    RegistGetterWithParam(
        BehaviorType(QC1_TECHNOLOGY, QC2_TECHNOLOGY_TABLE_ID, QC3_TECHNOLOGY_COMPLETE).m_counterType, //研究完成
        boost::bind(&PlayerTechnology::HaveTec, (this->m_Technology).get(), _1));
	RegistGetterWithParam(
		BehaviorType(QC1_TECHNOLOGY, QC2_TECHNOLOGY_TABLE_ID, QC3_TECHNOLOGY_UPSTAR).m_counterType, //注资完成
		boost::bind(&PlayerTechnology::HasFinishUpStar, (this->m_Technology).get(), _1));
    // 世界宝箱
    RegistRecordGetter(
        BehaviorType(QC1_WORLD_BOX, QC2_UNDEFINED, QC3_WORLD_BOX_GET).m_counterType, //获取
        boost::bind(&PlayerMapLogic::GetBoxCnt, (this->m_worldMapLogic).get()));
    // 世界矿场

//     RegistGetterWithParam(
//         BehaviorType(QC1_WORLD_MINES, QC2_UNDEFINED, QC3_WORLD_MINES_OCCUPY).m_counterType, //占领
//         boost::bind(&, (this->m_).get(), _1);

	RegistRecordGetter( BehaviorType( QC1_CITY, QC2_UNDEFINED,QC3_CITY_GET).m_counterType, boost::bind(&Player::OccupyCityCount, this)); // 攻占城池
//战斗
	RegistRecordGetter( BehaviorType( QC1_FIGHT, QC2_KILL,QC3_UNDEFINED).m_counterType, boost::bind(&Player::KillEnemy, this)); // 杀敌数
	RegistRecordGetter( BehaviorType( QC1_FIGHT, QC2_TRICK,QC3_UNDEFINED).m_counterType, boost::bind(&Player::TrickNum, this)); // 诱敌数
	RegistRecordGetter( BehaviorType( QC1_FIGHT, QC2_ADD_SOLIDER,QC3_UNDEFINED).m_counterType, boost::bind(&Player::AddSolider, this)); // 借兵


    RegistRecordGetter( BehaviorType( QC1_PLAYER, QC2_PLAYER_LEVEL_REACH).GetUint64(), boost::bind(&PlayerValueOwner::GetLevel, this));
    RegistRecordGetter( BehaviorType( QC1_PLAYER, QC2_PLAYER_VIP_LEVEL_REACH).GetUint64(), boost::bind(&PlayerValueOwner::GetVipLevel, this));
    RegistRecordGetter( BehaviorType( QC1_PLAYER, QC2_PLAYER_COMBAT_POWER).GetUint64(), boost::bind(&PlayerValueOwner::GetCombatPower, this));
    RegistRecordGetter( BehaviorType( QC1_PLAYER, QC2_PLAYER_CONTINUE_LOGIN_DAY).GetUint64(), boost::bind(&PlayerValueOwner::GetContinLoginDay, this));
#endif
}

void Player::_SetLevel( int level )
{
    int previousLv = GetLevel();
    PlayerValueOwner::_SetLevel(level);
    m_questLogger->UpdateQuest(this,QC1_PLAYER, QC2_PLAYER_LEVEL_REACH);

    if ( previousLv != level)
    {
        AddEventLog( EVENT_PLAYER_BEHAVIOR_LEVELUP,
            2, level, previousLv );
    }
    
}

//void Player::_SetIconID(int headid)
//{
//	PlayerValueOwner::_SetIconID(headid);
//}

void Player::_SetVipLv( int vipLv )
{
    int previousVipLv = GetVipLevel();
    PlayerValueOwner::_SetVipLv( vipLv);
    m_questLogger->UpdateQuest(this,QC1_PLAYER, QC2_PLAYER_VIP_LEVEL_REACH);

    if ( previousVipLv != vipLv)
    {
        AddEventLog( EVENT_PLAYER_BEHAVIOR_VIPLEVELUP,
            2, vipLv, previousVipLv );
    }
}

void Player::_SetCombatPower( int power )
{
    PlayerValueOwner::_SetCombatPower( power);
    m_questLogger->UpdateQuest(this,QC1_PLAYER, QC2_PLAYER_COMBAT_POWER);
}


void Player::InitNewPlayerData()
{
    _SetVipLv( 0);
	SetValueInt(pb::PLAYER_FIELD_SUM_DINNER_NUMBER,GET_BASE_DEF_UINT(BD_FEAST_LIMIT_TIME));//宴会上限
	SetValueInt(pb::PLAYER_FIELD_DINNER_NUMBER,GET_BASE_DEF_UINT(BD_FEAST_LIMIT_TIME));//宴会次数6次
	SetValueInt(pb::PLAYER_FIELD_DRAFT_ORDER_LIMIT,GET_BASE_DEF_UINT(BD_DRAFT_RODER_LIMIT));// 征兵令上限
	SetValueInt(pb::PLAYER_FIELD_NORMAL_EXPLOIT_BOX_LIMIT,GET_BASE_DEF_UINT(BD_NORMAL_EXPLOIT_BOX_LIMIT));
	SetValueInt(pb::PLAYER_FIELD_ADVANCE_EXPLOT_BOX_LIMIT,GET_BASE_DEF_UINT(BD_ADVANCE_EXPLOT_BOX_LIMIT));
	SetValueInt(pb::PLAYER_FLIELD_TROOPS_ORDER_LIMIT,GET_BASE_DEF_UINT(BD_INCREASE_TROOPS_TIME_LIMIT));//增兵次数上限
	SetValueInt(pb::PLAYER_FIELD_CARD_SLOT_COUNT,2);//初始可以招募武将2个
	m_characterStorage->BattleCharAttrResize(m_characterStorage->GetBattleMemberCount(this));
	if(GetAttrInt(pb::PLAYER_FIELD_EXPLOIT_RANK_BOX_LEVEL) == 0)
	{
		SetValueInt(pb::PLAYER_FIELD_EXPLOIT_RANK_BOX_LEVEL,1);//初始化军工宝箱等级
	}
	SetValueInt(pb::PLAYER_FIELD_ACTIVITY_STAR_BOX_LEVEL,1);

	SetRegistTime(sOS.GetRealTime());//设置注册时间
#ifdef _MMO_SERVER_
	uint32 char_proId = GetFirstAutoUnlockCharacterId();
	if (char_proId > 0)
	{
		Character * character = PlayerCharacterLogic::CreateCharacterForPlayer(this, char_proId);
		if( character != NULL )
		{
			CharacterIds ids;
			ids.push_back(character->GetID());
			ItemCharacterLogic::SetBattleArray( this, ids);
		}
	}

    // 商店
    //PlayerSellLogic::InitVictimRDProgram(*this);


    // 乐透免费记录
    //m_lotteryLogger->SendFreeRecordToDB( this);
	PlayerCharacterLogic::InitCreatPlayerData(this);
	InitPlayerBaseData();
#endif
}

uint32 Player::GetDbBaseDefineValueUint(pb::BASE_DEFINE defineType)
{
	return	GET_BASE_DEF_UINT(defineType);
}

int Player::GetDbBaseDefineValueInt(pb::BASE_DEFINE defineType)
{
	return	GET_BASE_DEF_INT(defineType);
}

float Player::GetDbBaseDefineValueFloat(pb::BASE_DEFINE defineType)
{
  return GET_BASE_DEF_FLOAT(defineType);
}

void Player::InitPlayerBaseData()
{
#ifdef _MMO_SERVER_
	/*AddCurrency(IR_GM_GET,eCoin,10000000);
	AddCurrency(IR_GM_GET,eSysGold,10000000);
	AddCurrency(IR_GM_GET,eIron,10000000);
	AddCurrency(IR_GM_GET,eFood,1000000);
	AddCurrency(IR_GM_GET,eWood,1000000);
	AddCurrency(IR_GM_GET,eHornor,1000000);
	AddCurrency(IR_GM_GET,eFreePurify,12);
	AddCurrency(IR_GM_GET,eExtrmePurify,5);
	AddCurrency(IR_GM_GET,eGoldHammer,20);
	SetAutoHammerCount(20);*/

	
	SetEquipShopBuyCoolDownTime(sOS.GetRealTime());
	SetBagCapacity(GET_BASE_DEF_UINT(BD_PLAYER_BAG_BEGIN_CAPACITY));
	SetCurRebuildId(GET_BASE_DEF_UINT(BD_REBUILD_BEGIN_BUILD_ID));
	SetAutoIncreaseSolders(true);
#endif
}

uint64 Player::SessionId()
{
	if (m_pSession)
	{
	  return m_pSession->GetSessionId();
	}
	return 0;
}

void Player::SendItemError( ItemOperateError errorType, int32 addVal /*=0 */ )
{
	pb::GS2C_ItemOperateError opError;
	opError.set_error_type( errorType ) ;
	opError.set_property( addVal ) ;

	Send( pb::SMSG_ITEM_OPERATE_ERROR, opError ) ;
}



uint32 Player::GetLeaderProtoId()
{
	if (m_characterStorage.get())
	{
		return m_characterStorage->GetLeaderProtoId();
	}
	return 0;
}

void Player::UpDatePlayerData()
{
	IsShutUp();
	if (m_bag)
	{
		std::vector<int> todelIds;
		if(m_bag->RemoveExpireItem(todelIds))
		{
			for (std::vector<int>::iterator itemIter = todelIds.begin(); itemIter != todelIds.end(); ++itemIter)
			{
				OnDestoryItem(IR_ITEM_EXPIRY_COST,(int)(*itemIter),1);
			}
		}
	}
#ifdef _MMO_SERVER_
	if (m_marketLog)
	{
		m_marketLog->InitBuyContent(*this);
	}
	if (m_fubenLog)
	{
		if (m_fubenLog->Init())
		{
			PlayerFuBenDB::SendFuBenBaseToDb(*this, *m_fubenLog.get());
		}
	}
#endif
}
void Player::OnPlayerLogin()
{
    time_t periodLast = sPeriodSystem.GetTimePeriod( GetLastLogin(), SECONDS_PER_DAY);
    time_t periodNow = sPeriodSystem.GetTimePeriod( sOS.GetRealTime(), SECONDS_PER_DAY);
    time_t diffDay = periodNow - periodLast;

    if ( diffDay >= 1) // 今天第一次登录
    {
		SetLoginDaysNumber(GetLoginDaysNumber() + 1);
		m_SeigeForceLog->GivePlayerFirstSeigeArmy(this,GetLoginDaysNumber());

        if ( diffDay == 1 || GetLastLogin() == 0) // 连续登录
        {
            SetContinLoginDay( GetContinLoginDay() + 1);
        }
        else
        {
            SetContinLoginDay( 1);
        }
        m_questLogger->UpdateQuest(this,QC1_PLAYER, QC2_PLAYER_CONTINUE_LOGIN_DAY );
        m_behaviorCounter->TriggerBehavior(this, QC1_PLAYER, QC2_PLAYER_LOGIN_DAY, QC3_UNDEFINED);
    }
    SetLastLoginToNow();
}
void Player::ReEnterCurCityOnRestart()
{
    struct _Temp_Hero_Func //辅助函数对象
    {
        bool operator()(Character* p)
        {
            p->_ReEnterCurCity();
            return false;
        }
    };
    _Temp_Hero_Func objFunc;
    m_characterStorage->ForEachBattleCharacter(objFunc);
}

void Player::SetPlayerPrivilege(uint32 pri, uint32 value)
{
	switch(pri)
	{
	case pb::PLAYER_GET_AUTO_BUILD:
	case pb::PLAYER_GET_TROOPS_ODER_EVERYDAY:
		{
			uint32 nReward  = value;
			if (nReward > 0)
			{
				AddCurrency(IR_VIP_REWARD_GET,eTroopsOder,nReward);
			}
		}
	case pb::PLAYER_OPEN_ADVANCED_RES_FUBEN:
	case pb::PLAYER_UP_KILL_ENEMY_HORNOR:
	case pb::PLAYER_UP_RES_CAPCITY:
	case pb::PLAYER_UP_WORD_MAP_MOVE_SPEED:
	case pb::PLAYER_UP_COUNTRY_WAR_TIME:
	case pb::PLAYER_CAN_FAR_TRIGE_PERSONAL_EVENT:
	case pb::PLAYER_NO_SHOP_COOL_DOWN_TIME:
	case pb::PLAYER_CRUSADE_ADD_HERO_TIMES:
		{
#ifdef _MMO_SERVER_
			m_fubenLog->m_crusadeLog.AddHeroAddTimes(value);
#endif
		}
		SetFlag(pri);
		break;
	}
}

void Player::OnModifyInt( int valIdx, int val,int old_value,bool isNew /* = true */ )
{
	uint32 country_id = m_worldMapLogic->GetCountryId();
	uint64 player_id = GetGuid();
	const string &name = CharName();
	uint32 lv = GetLevel();
	uint32 vip_level = GetVipLevel();
	uint32 head_icon_id = GetHeadIconId();
	switch(valIdx)
	{
	case pb::PLAYER_FIELD_LEVEL:
		{
			/*_SetLevel(val);*/
			m_buildingLogMgr->CheckBuildingIsUnlock(this,val);
			
			//玩家升级的时候给奖励

			IntPairVec vec;
			if(sPlayerDataMgr.GetRewardByLevel(val,&vec)){
				sReward.Change(*this,vec);
				pb::PlayLvUpReward msg;
				for(IntPairVec::iterator iter = vec.begin();iter != vec.end();++iter)
				{
					pb::StReward* reward = msg.add_rewards();
					reward->set_type(iter->first);
					reward->set_value(iter->second);
				}
				msg.set_playerlv(val);
				Send(pb::SMSG_PLAYER_LEVEL_UP_REWARD,msg);
			}
			//成长任务
			UpdateAllGrowUpQuestInfo(pb::LEVLE_UP,val);
			{
				UpdatePlayerLvToMap(sExploitMgr.exploit_rank_map_,val,vip_level,player_id,country_id,GetExploitValue(),old_value,name,head_icon_id);
				UpdatePlayerLvToMap(sExploitMgr.trick_rank_map_,val,vip_level,player_id,country_id,GetTrickValue(),old_value,name,head_icon_id);
				UpdatePlayerLvToMap(sExploitMgr.occupy_rank_map_,val,vip_level,player_id,country_id,GetOccupyValue(),old_value,name,head_icon_id);
				UpdatePlayerLvToMap(sActivityRankTableMgr.occupation_rank_map_,val,vip_level,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM),old_value,name,head_icon_id);
				UpdatePlayerLvToMap(sActivityRankTableMgr.add_solider_rank_map_,val,vip_level,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER),old_value,name,head_icon_id);
				if(val > GET_BASE_DEF_INT(pb::BD_BIGGER_TO_ENTRY_FIGHT_RANK))
					UpdatePlayerLvToMap(sExploitMgr.military_power_rank_map_,val,vip_level,player_id,country_id,getMilitaryPower(),old_value,name,head_icon_id);
			}
			break;
		}
	case pb::PLAYER_FIELD_VIP_LEVEL:
		{
#ifdef _MMO_SERVER_
			std::map<int,int> retMap;
			if (sPlayerDataMgr.GetPrivilege(val,retMap))
			{
				std::map<int,int>::iterator iterPri = retMap.begin();
				for (; iterPri != retMap.end(); ++iterPri)
				{
					SetPlayerPrivilege((uint32)iterPri->first, (uint32)iterPri->second);
				}
			}
#endif
			UpdatePlayerVipLvToMap(sExploitMgr.exploit_rank_map_,lv,val,player_id,country_id,GetExploitValue(),old_value,name,head_icon_id);
			UpdatePlayerVipLvToMap(sExploitMgr.trick_rank_map_,lv,val,player_id,country_id,GetTrickValue(),old_value,name,head_icon_id);
			UpdatePlayerVipLvToMap(sExploitMgr.occupy_rank_map_,lv,val,player_id,country_id,GetOccupyValue(),old_value,name,head_icon_id);
			UpdatePlayerVipLvToMap(sActivityRankTableMgr.occupation_rank_map_,lv,val,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM),old_value,name,head_icon_id);
			UpdatePlayerVipLvToMap(sActivityRankTableMgr.add_solider_rank_map_,lv,val,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER),old_value,name,head_icon_id);
			if(lv > GET_BASE_DEF_UINT(pb::BD_BIGGER_TO_ENTRY_FIGHT_RANK))
				UpdatePlayerVipLvToMap(sExploitMgr.military_power_rank_map_,lv,val,player_id,country_id,getMilitaryPower(),old_value,name,head_icon_id);
			break;
		}
	case pb::PLAYER_FIELD_GUIDE_PROCESS:
		{
			break;
		}
	case pb::PLAYER_FIELD_HEAD_ICON_ID:
		{
			if(val == 0) break;
			UpdatePlayerHeadIconIdToMap(sExploitMgr.exploit_rank_map_,lv,vip_level,player_id,country_id,GetExploitValue(),val,old_value,name);
			UpdatePlayerHeadIconIdToMap(sExploitMgr.trick_rank_map_,lv,vip_level,player_id,country_id,GetTrickValue(),val,old_value,name);
			UpdatePlayerHeadIconIdToMap(sExploitMgr.occupy_rank_map_,lv,vip_level,player_id,country_id,GetOccupyValue(),val,old_value,name);
			UpdatePlayerHeadIconIdToMap(sActivityRankTableMgr.occupation_rank_map_,lv,vip_level,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM),val,old_value,name);
			UpdatePlayerHeadIconIdToMap(sActivityRankTableMgr.add_solider_rank_map_,lv,vip_level,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER),val,old_value,name);
			if(lv > GET_BASE_DEF_UINT(pb::BD_BIGGER_TO_ENTRY_FIGHT_RANK))
				UpdatePlayerHeadIconIdToMap(sExploitMgr.military_power_rank_map_,lv,vip_level,player_id,country_id,getMilitaryPower(),val,old_value,name);
			break;
		}
	case pb::PLAYER_FIELD_IS_IN_CROSS_SERVER:
		{
			/*if(Values().GetInt32(valIdx) != val)
				sSocialHandler.OnPlayerAttrChange(pb::PLAYER_VALUE_TYPE_32_BIT_FIELD,GetGuid(),val,valIdx);*/
			
		}
		break;
	case pb::PLAYER_FIELD_GROW_UP_QUEST_EXPLOIT_VALUE:
		{
			UpdateAllGrowUpQuestInfo(pb::GET_EXPLOIT,val);
		}
		break;
	case pb::PLAYER_FIELD_XILIAN_COUNT:
		{
			UpdateAllGrowUpQuestInfo(pb::XILIAN_EQUIP,val);
		}
		break;
	case pb::PLAYER_FIELD_EXPLOIT_RANK_VALUE:
		{
			if(val == 0) break;
			UpdateRankSortToMap(sExploitMgr.exploit_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
//////-------------------------------------------------------------------------------------------------------------
			GiveExploitBox();
///////////////////////////////////////----------------------------------------------------------------------------
		}
		break;
	case pb::PLAYER_FIELD_TRICK_RANK_VALUE:
		{
			if(val == 0) break;
			UpdateRankSortToMap(sExploitMgr.trick_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
		}
		break;
	case pb::PLAYER_FIELD_OCCUPY_RANK_VALUE:
		{
			if(val == 0) break;
			UpdateRankSortToMap(sExploitMgr.occupy_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
			UpdateAllGrowUpQuestInfo(pb::ATTACK_CITY,val);	
		}
		break;
	case pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM:
		{	
			if(val == 0) break;
			UpdateRankSortToMap(sActivityRankTableMgr.occupation_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
		}
		break;
	case pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER:
		{
			if(val == 0) break;
			UpdateRankSortToMap(sActivityRankTableMgr.add_solider_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
		}
		break;
	case pb::PLAYER_FIELD_MILITARY_POWER_VALUE:
		if(val == 0) break;
		if(lv > GET_BASE_DEF_UINT(pb::BD_BIGGER_TO_ENTRY_FIGHT_RANK))
			UpdateRankSortToMap(sExploitMgr.military_power_rank_map_,lv,vip_level,player_id,country_id,val,name,old_value,head_icon_id);
		break;
	default:
		break;
	}

	if(isNew)
	{
		//如果有玩家数据值改变，则通知游戏服务器/跨服服务器
		//cross->game
	#ifdef _SERVER_CROSS_
		//LLOG("[1002] cross to game Update Player Value");
		pb::GS2R_Update_Player_Value msg;
		msg.set_index(valIdx);
		msg.set_val(val);
		SendToServer(pb::SG2R_UPDATE_PLAYER_VALUE,msg,PACKET_TYPE_SERVER_CROSS);
	#endif
		//game->cross
	#ifdef _MMO_SERVER_
		pb::GS2R_Update_Player_Value msg;
		msg.set_index(valIdx);
		msg.set_val(val);
		//LLOG("[1002] game to cross Update Player Value");
		SendCrossServerMsg(pb::SG2R_UPDATE_PLAYER_VALUE,msg,PACKET_TYPE_SERVER_GAME,GetGuid());
	#endif
	}
}

void Player::OnModifyfloat( int valIdx, float val )
{

}

void Player::OnModifyString( int valIdx, const string& val)
{
	uint32 country_id = m_worldMapLogic->GetCountryId();
	uint64 player_id = GetGuid();
	uint32 lv = GetLevel();
	uint32 vip_lv = GetVipLevel();
	uint32 head_icon_id = GetHeadIconId();
	switch(valIdx)
	{
	case pb::PLAYER_FIELD_NAME:
		{
			UpdatePlayerNameToMap(sExploitMgr.exploit_rank_map_,lv,vip_lv,player_id,country_id,GetExploitValue(),val,head_icon_id);
			UpdatePlayerNameToMap(sExploitMgr.trick_rank_map_,lv,vip_lv,player_id,country_id,GetTrickValue(),val,head_icon_id);
			UpdatePlayerNameToMap(sExploitMgr.occupy_rank_map_,lv,vip_lv,player_id,country_id,GetOccupyValue(),val,head_icon_id);
			UpdatePlayerNameToMap(sActivityRankTableMgr.occupation_rank_map_,lv,vip_lv,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM),val,head_icon_id);
			UpdatePlayerNameToMap(sActivityRankTableMgr.add_solider_rank_map_,lv,vip_lv,player_id,country_id,GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_ADD_SOLDIER),val,head_icon_id);
			if(lv > GET_BASE_DEF_UINT(pb::BD_BIGGER_TO_ENTRY_FIGHT_RANK))
				UpdatePlayerNameToMap(sExploitMgr.military_power_rank_map_,lv,vip_lv,player_id,country_id,getMilitaryPower(),val,head_icon_id);
		}
		break;
	case pb::PALYER_FIELD_GUILD_NAME:
		{
			/*if(val != Values().GetString(valIdx) )
				sSocialHandler.OnPlayerAttrChange(pb::PLAYER_VALUE_TYPE_STR_FIELD,GetGuid(),0,valIdx,val);*/

		}
		break;
	default:
		break;
	}
}

void Player::OnModifyUint64( int valIdx, uint64 val )
{
	switch(valIdx)
	{
	//case pb::PLAYER_FIELD_THREE_RESET_TIME:
		//{
		//	
		//}
		///break;
	case pb::PLAYER_FIELD_LAST_LOGIN_TIME:
	case pb::PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_TIME:
	case pb::PLAYER_FIELD_NEXT_RECOVER_MARKET_BUY_NUM_TIME:
	case pb::PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_TIME:
	case pb::PLAYER_FIELD_PLAYER_ID:
	case pb::PLAYER_FIELD_GUILD_ID:

	/*case pb::PLAYER_FIELD_FOUR_RESET_TIME:
		{
			if(val != Values().GetUint64(valIdx) )
			{
				//m_playStringGroup->SetString((uint32)pb::PLAYER_FIELD_AP_SEND_IDS,"");
				SetValueInt(pb::PLAYER_FIELD_AP_RECEIVE,0);
				sSocialHandler.ResetFriendAp(GetGuid());*/
			//}
		//}
		//break;
		//

	case pb::PLAYER_FIELD_PALACE_TAKE_SALARY_TIME:
		{
// 			if(Values().GetUint64(valIdx)!= val){
// 				SetValueUint64(valIdx,val);
// 			}

		}
		break;
	default:
		break;
	}
	

}

void Player::AddEventLog( pb::EVENT_ID eventID, int paramCount, ... )
{
    EventLogInfo evtMsg;
	evtMsg.event_id = eventID;
	evtMsg.account_id =  AccountId();
	evtMsg.player_id = GetGuid();

    va_list vlist;
    va_start(vlist, paramCount);

    sEventLogClient.SendEvent( evtMsg, paramCount, vlist, false);

    va_end(vlist);
}

void Player::AddEventLogFMT( pb::EVENT_ID eventID, int paramCount, ... )
{
	EventLogInfo evtMsg;
	evtMsg.event_id = eventID;
	evtMsg.account_id =  AccountId();
	evtMsg.player_id = GetGuid();
    va_list vlist;
    va_start(vlist, paramCount);
    
    sEventLogClient.SendEvent( evtMsg, paramCount, vlist, true);

    va_end(vlist);
}

bool Player::DestroyItem( pb::ITEM_REASON reason, int entry, int count )
{
    bool result = m_bag->DestroyItem( entry, count);
    if ( result)
    {
        OnDestoryItem( reason, entry, count);
    }
    return true;

}

bool Player::DestroyItem( pb::ITEM_REASON reason, const ItemID2Count& itemid2count )
{
    bool result = m_bag->DestroyItem( itemid2count);
    if ( result)
    {
        for ( ItemID2Count::const_iterator it = itemid2count.begin();
            it != itemid2count.end(); ++it)
        {
            OnDestoryItem( reason, it->first, it->second);
        }
    }
    return result;
}

bool Player::DestroySlotItem( pb::ITEM_REASON reason, int slot )
{
    int entry = 0;
    int count = 0;
    bool result = m_bag->DestroySlotItem( slot, entry, count);
    if ( result )
    {
        OnDestoryItem( reason, entry, count);
    }
    return result;
}

bool Player::SellSlotItem(pb::ITEM_REASON reason, int slot,bool forceSell/* = false*/)
{
	int entry = 0;
    int count = 0;
    bool result = m_bag->SellSlotItem( slot, entry, count,forceSell);
    if ( result )
    {
		const LogicItemProto* proto = sItemMgr.Find(entry);
		if (proto)
		{
			uint32 allSellPrice =	proto->sell_price * count;
			AddCurrency(reason,eCoin,allSellPrice);
		}
    }
	AddEventLog( EVENT_PALYER_ITEM_SYS, 
		4,
		GetLevel(),
		entry,
		-count,
		reason);
    return result;
}

uint32 Player::BuyBackSlotItem(pb::ITEM_REASON reason, int slot)
{
	bool result = false;
	Item* curItem = m_bag->MutableItem(slot);
	if (curItem)
	{
		uint32 allSellPrice = curItem->Proto()->sell_price * 2 * curItem->StackCount();
		if (EnoughCurrency(eCoin,allSellPrice))
		{
			int entry = 0;
			int count = 0;
			result = m_bag->BuyBackSlotItem( slot, entry, count);
			if ( result )
			{
				TryDeductCurrency(reason,eCoin,allSellPrice);
				return pb::ErrCommonSuccess;
			}
			else
			{
				return pb::ErrCommonFail;
			}
		}
		else
		{
			return pb::ErrNotEnoughCoin;
		}
	}
	return pb::ErrCommonFail;
}

bool Player::TakeOutSlotItem( pb::ITEM_REASON reason, int slot, int count )
{
	int entry = 0;
	
	if (m_bag->IsEquipForBagSolt(slot))
	{
		ItemEquip* curEquip = m_bag->MutableItemEquip(slot);
		if (curEquip && !curEquip->CanBeDestroyed())
		{
			CharacterStorage* charStorage = m_characterStorage.get();
			Character* character = charStorage->MutableCharacter( curEquip->EquiperId());
			if ( character == NULL)
				return pb::ErrCommonFail;
			if ( character)
			{
				int curEquipSlot = character->GetEquipSlot(curEquip->BagSlot());
				if (curEquipSlot >= 0 )
				{
					ItemCharacterLogic::_RemoveEquipFromCharacter( character, m_bag.get(), curEquip, curEquipSlot);
				}
				else
				{
					curEquip->DettachFromCharacter( curEquip->EquiperId());
					m_bag->SetModifiedID( curEquip->BagSlot());
				}
			}
		}
	}

	bool result = m_bag->TakeOutSlotItem( slot, count, entry);
	if ( result)
	{
		OnDestoryItem( reason, entry, count);
	}
	return result;
}

bool Player::TakeOutItem( pb::ITEM_REASON reason, int entry, int count )
{
	if(m_bag->CountItem(entry) < count)
		return false;

	if (!m_bag->EnoughItem(entry,count))
	{
		ItemEquip* curEquip = m_bag->MutableItemEquipByProId(entry);
		if (curEquip && !curEquip->CanBeDestroyed())
		{
			//Todo
			CharacterStorage* charStorage = m_characterStorage.get();
			Character* character = charStorage->MutableCharacter( curEquip->EquiperId());
			if ( character == NULL)
				return pb::ErrCommonFail;
			if ( character)
			{
				int curEquipSlot = character->GetEquipSlot(curEquip->BagSlot());
				if (curEquipSlot >= 0 )
				{
					ItemCharacterLogic::_RemoveEquipFromCharacter( character, m_bag.get(), curEquip, curEquipSlot);
				}
				else
				{
					curEquip->DettachFromCharacter( curEquip->EquiperId());
					m_bag->SetModifiedID( curEquip->BagSlot());
				}
			}
		}
	}

	bool result = m_bag->DestroyItem( entry, count);
	if ( result)
	{
		OnDestoryItem( reason, entry, count);
	}
	return result;
}

void Player::OnDestoryItem( pb::ITEM_REASON reason, int entry, int count )
{
    AddEventLog( EVENT_PALYER_ITEM_SYS, 
        4,
        GetLevel(),
        entry,
        -count,
        reason);
}

void Player::AddCurrency(uint32 type, uint32 number )
{
	//int previous = GetCurrency( type);
	PlayerValueOwner::AddCurrency( type, number);
}

void Player::AddCurrency( pb::ITEM_REASON reason, uint32 type, uint32 number )
{
    int previous = GetCurrency( type);
	if(!IsActivityStillActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		if(type == eRenown || type == eStarCrystal || type == eBoard || type == eGeneCode)
			return;
	}
	if(!IsActivityStillActive(pb::ACTIVITY_HERO_REWARD))
	{
		if(type == eBeef || eWine== type)
			return;
	}
    PlayerValueOwner::AddCurrency( type, number);
    AddEventLog( pb::EVENT_PLAYER_MONEY_SYS,
        6,
        GetLevel(),
        reason,
        type,
        number,
        GetCurrency(type),
        previous
        );
}

bool Player::TryDeductCurrency( pb::ITEM_REASON reason, uint32 type, uint32 number )
{
    int previous = GetCurrency( type);
    bool result = PlayerValueOwner::TryDeductCurrency( type, number);
    if ( result)
    {
        AddEventLog( pb::EVENT_PLAYER_MONEY_SYS,
            6,
            GetLevel(),
            reason,
            type,
            -((int)number),
            GetCurrency(type),
            previous
            );
    }
    return result;
}

bool Player::TryDeductCurrency( pb::ITEM_REASON reason, const CurrType2Count& curr2count )
{
    CurrType2Count previous;
    for ( CurrType2Count::const_iterator it = curr2count.begin();
        it != curr2count.end(); ++it)
    {
        previous[it->first] = GetCurrency( it->first);
    }
    bool result = PlayerValueOwner::TryDeductCurrency( curr2count); 
    if ( result)
    {
        for ( CurrType2Count::const_iterator itp = previous.begin(), itc = curr2count.begin();
            itp != previous.end(); ++itp, ++itc)
        {
            AddEventLog( pb::EVENT_PLAYER_MONEY_SYS,
                6,
                GetLevel(),
                reason,
                itc->first,
                - (int)(itc->second),
                GetCurrency(itc->first),
                itp->second
                );
        }
    }


    return result;
}


time_t Player::AddTimer(uint32 type,uint32 id, boost::function<void()> f, U32 start, int repeat, U32 interval)
{
	return m_timerEventGroup->Add(type,id, f, start, repeat, interval);
}

void Player::AddCallBackTimer(uint32 type, boost::function<void()> f, U32 start, int repeat, U32 interval)
{
	m_timerEventGroup->AddTimer(type,f, start, repeat, interval);
}

bool Player::HasTypeTimer(uint32 type)
{
	return m_timerEventGroup->HasTypeTimer(type);
}

void Player::RemoveTimer(uint32 type,uint32 id)
{
	m_timerEventGroup->RemoveEvent(type,id);
}

void Player::LoadPlayerAppendData(const pb::PlayerAppendData &data)
{
	//NLOG("TEST Player::LoadPlayerAppendData");
	m_buildingLogMgr->Init();
	m_buildingLogMgr->LoadModuleFrom(data.module());
	m_buildingLogMgr->LoadBuildingFrom(data.building());
	m_buildingLogMgr->CheckBuildingIsUnlock(this,GetLevel());//set isUnlock = true;
	PlayerOperationLogic::AddIncomeTimer(this);
}

void Player::DoUpdatePer_Second_Zero(uint64 timeNowMS)
{
#ifdef _MMO_SERVER_
	if(m_worldMapLogic)
	{
		if(IsOnline())
			m_worldMapLogic->_UpdateCityEvent(*this,timeNowMS);
	}
#endif
}
void Player::DoUpdatePer_Minute_Zero()
{
	//uint64 begintime =sOS.TimeMS();
	AutoGiveDailySignUpReward();
	//uint64 temp1 = sOS.TimeMS();
	//uint64 up1 =sOS.DiffTimeMS(begintime);
	AutoGivePassStageReward();
	//uint64 temp2 = sOS.TimeMS();
	//uint64  up2 = sOS.DiffTimeMS(temp1);
	AutoGiveXilianReward();
	//temp1 = sOS.TimeMS();
	//uint64  up3 =sOS.DiffTimeMS(temp2);
	AutoGiveOpenFogReward();
	//temp2 = sOS.TimeMS();
	//uint64 up4 =sOS.DiffTimeMS(temp1);
	AutoGiveGrowUpQuestReward();
	//temp1 = sOS.TimeMS();
	//uint64 up5 = sOS.DiffTimeMS(temp2);
	AutoActivityStarBoxReward();
	//temp2 = sOS.TimeMS();
	//uint64 up6 =sOS.DiffTimeMS(temp1);
	AutoGiveIronCollectReward();
	//temp1 = sOS.TimeMS();
	//uint64 up7 =sOS.DiffTimeMS(temp2);
	ResetBuyLog();
	//temp2 = sOS.TimeMS();
	//uint64 up8 =sOS.DiffTimeMS(temp1);
	AutoGiveCelebrityReward();
	//temp1 = sOS.TimeMS();
	//uint64 up9 =sOS.DiffTimeMS(temp2);
	resetGeneralRewardData();
	//temp2 = sOS.TimeMS();
	//uint64 up10 =sOS.DiffTimeMS(temp1);
	AutoGiveRechargeRebateAward();
	//uint64 up11 =sOS.DiffTimeMS(temp1);
	//uint64 total =sOS.DiffTimeMS(begintime);
	//if (total != m_maxMinuupdatet)
	//{
	//	m_maxMinuupdatet = total;
	//	NLOG( "--------PlyaerPerMin up1:%llu,up2:%llu,up3:%llu,up4:%llu,up5:%llu,up6:%llu,up7:%llu,up8:%llu,up9:%llu,up10:%llu,up11:%llu,total:%llu",up1,up2,up3,up4,up5,up6,up7,up8,up9,up10,up11,total) ;
	//}
}

void Player::DoUpdatePer_Hour_Zero()
{
	//每小时恢复一点免费洗练次数
	if(GetCurrency(eFreePurify) < (int32)GET_BASE_DEF_UINT(BD_PLAYER_REGAIN_FREE_PURIFY_NUM_VALUE))
	{
		//LLOG("ItemManager::_recoverPurifyTimes recover once, num=%d",GetCurrency(eFreePurify));
		AddCurrency(eFreePurify,1);
	}

	if(IsOnline())
	{
		if(m_worldMapLogic)
			m_worldMapLogic->updateArmyResourcePerHour(*this);
	}

#ifdef _MMO_SERVER_
	if(IsOnline())
	{
		if(m_marketLog)
			m_marketLog->UpdateMatkets(this);
	}
#endif
}


void Player::DoUpdatePer_Day_Zero()
{
		//TODO:
	/*
	PLAYER_FIELD_NEXT_RECOVER_HORNOR_TIME             =4;
	PLAYER_FIELD_NEXT_RECOVER_MARKET_BUY_NUM_TIME     =5;
	PLAYER_FIELD_NEXT_RECOVER_FREE_PURIFY_TIME        =6;
	PLAYER_FIELD_NEXT_RECOVER_DAILY_QUEST_SCORE_TIME  =7;
	PLAYER_FIELD_NEXT_RECOVER_SMITHY_BUY_NUM_TIME     =8;
	PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_TIME         =9;
	PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_TIME        =10;
	*/

	SetValueInt(PLAYER_FIELD_DAILY_QUEST_SCORE,0);
	SetValueInt(PLAYER_FIELD_BUY_LABOUR_TIMES,0);
	SetValueInt(PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG,0);//领奖励的标记位清零
	resetTodayAddXP();//将每天科技31增加的经验置0
	resetDailyQuest();//每日任务的数据库清零
	UpdateDateTime();//每日签到
	m_CongratulateLog->ResetCongratulateData(*this);
	m_worldMapLogic->ClearMonsterCityInvadeCount(GetCountryId());
	if(TestFlag(pb::PLAYER_LOTTERY_DAILY_DONE)){
		RemoveFlag(pb::PLAYER_LOTTERY_DAILY_DONE);
	}

#ifdef _MMO_SERVER_
	if(TestFlag(pb::PLAYER_GET_TROOPS_ODER_EVERYDAY))
	{
		int nReward = sPlayerDataMgr.GetPrivilegeValue(pb::PLAYER_GET_TROOPS_ODER_EVERYDAY);
		if (nReward > 0)
		{
			AddCurrency(IR_VIP_REWARD_GET,eTroopsOder,nReward);
			int now_currency = GetCurrency(eTroopsOder);
			if(now_currency >= GetAttrInt(pb::PLAYER_FIELD_DRAFT_ORDER_LIMIT)){//征兵令上限
				now_currency = GetAttrInt(pb::PLAYER_FIELD_DRAFT_ORDER_LIMIT);
			}
			SetValueInt(pb::PLAYER_FIELD_TROOPSODER,now_currency);
		}
	}

	m_SmithyLog->DoZeroResetSmeltTimes(*this);
	m_marketLog->ResetCollectTimes(this);//每日征收的次数
	m_ContinueOccupyCityLog->ResetLogMap(*this);
#endif
	m_SeigeForceLog->ResetSiegeTimes(this);//reset siege army times.
//远征副本重置
#ifdef _MMO_SERVER_
	m_fubenLog->InitCrusadeData(this);
	m_fubenLog->ResetHideFubenFightTimes(*this);
#endif

}


void Player::TryDailyResetTime()
{
//#ifdef _MMO_SERVER_
//	ServerValue *pValue =sGameSvr.GetServerValue( pb::VALUE_INDEX_PLAYER_DAILY_RESET_TIME ) ;
//	if( pValue == NULL )
//	{
//		return  ;
//	}
//#endif
}

void Player::Attach( TimingWheel* pTimingWheel )
{
	m_timerEventGroup->Attach(pTimingWheel);
}

void Player::RegistQuestLogger()
{
	m_questLogger->AfterLoadLogger(this);
}

void Player::CalculatePlayerData()
{
	m_characterStorage->InitCharactersCombatData();
}

void Player::FullCharHp()
{
	m_characterStorage->FullCharHP();
}


bool Player::_SetCountryId(uint32 id)
{
	return m_worldMapLogic->_SetCountryId(*this, id);
}
uint32 Player::GetCountryId() const
{
    return m_worldMapLogic->GetCountryId();
}

void Player::SetCrossUniqueID()
{
#ifdef _MMO_SERVER_
	LLOG("Set One Player Cross Unique ID");
	if(m_pCountry == NULL)
	{
		m_pCountry = sWorldMap.GetCountry(GetCountryId());
		m_worldMapLogic->SetCrossUniqueID(m_pCountry->cross_uniqueID);
		LLOG("Set Success, countryid = %d, crossid = %d",GetCountryId(),m_pCountry->cross_uniqueID);
	}
	else
	{
		m_worldMapLogic->SetCrossUniqueID(m_pCountry->cross_uniqueID);
		LLOG("Set Success, countryid = %d, crossid = %d",m_pCountry->id,m_pCountry->cross_uniqueID);
	}
#endif
}

uint32 Player::GetCrossUniqueId()
{
	return m_worldMapLogic->GetCrossUniqueID();
}

int Player::GetPlaceOfficeId() const
{
    int ret = sPalaceMgr(GetCountryId())->GetOfficeId(GetGuid());
    if (ret <= 0) ret = GetPlaceLowOfficeId();
    return ret;
}
int Player::GetPlaceAideOfficeId() const
{
    std::map<uint64, uint8>& mapData = sPalaceMgr(GetCountryId())->m_aideInfoMap;
    std::map<uint64, uint8>::iterator it = mapData.find(GetGuid());
    if (it == mapData.end()) return 0;
    return it->second;
}

void Player::UpdateEquipShop(pb::GS2C_LootShopEquipResult& msg)
{
#ifdef _MMO_SERVER_
	if (msg.err_code() == ErrCommonSuccess)
	{
		int equipCout = msg.loot_list().items_size();
		ASSERT(equipCout == 6);
		std::list< Item *> outItemList;
		m_equipShop->DestroyAll();
		for (int i = 0; i < equipCout; i++ )
		{
			pb::GS2C_SlotItem* curInfo = msg.mutable_loot_list()->mutable_items(i);
			uint32 curProid = curInfo->item().proto_id();
			const LogicItemProto* proto = sItemMgr.Find(curProid);
			ASSERT(proto != NULL);
			if (proto && proto->CanBeCreateToBag())
			{
				outItemList.clear();
				if (m_equipShop->CreateItem(proto,GetBagCapacity(),1,&outItemList))
				{
					Item * curItem = outItemList.front();
					ItemEquip* item_equip = dynamic_cast<ItemEquip*>( curItem);
					if ( item_equip && !item_equip->IsEquiped())
					{
						uint32 curSolt = item_equip->BagSlot();
						item_equip->LoadFrom(*curInfo);
						item_equip->SetSlot(curSolt);
						curInfo->set_slot(curSolt);
					}
				}
				else
				{
				   ASSERT(false);
				}
			}
		}
	}
#endif
}

void Player::RefreshBuff()
{
    m_Buff->Refresh(OS::TimeMS());

    if (m_Buff->Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_PLAYER, this);
    }
}
void Player::AddBuff(uint32 buffId, uint32 lastMsec /* = 0 */)
{
    if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(buffId))
    {
        switch (table->GetTarget()) {
        case PlayerBuffTable::Char:
        case PlayerBuffTable::All:
            break;
        default:
            return;
        }

        if (m_Buff->Size() == 0)
        {
            ServiceMgr::Register(SERVICE_BUFF_PLAYER, this);
        }
        m_Buff->AddBuff(table, lastMsec);
    }
}
void Player::DelBuff(uint32 buffId)
{
    m_Buff->DelBuff(buffId);

    if (m_Buff->Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_PLAYER, this);
    }
}
bool Player::HaveBuff(uint32 buffId) const
{
    return m_Buff->HaveBuff(buffId);
}
bool Player::HaveBuff(CBuff::Type typ) const
{
    return m_Buff->HaveBuff(typ);
}
void Player::DelOneTypeBuff(CBuff::Type typ)
{
    m_Buff->DelOneTypeBuff(typ);

    if (m_Buff->Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_PLAYER, this);
    }
}
uint32 Player::CountEquip(uint32 quality, uint32 star)
{
    // TODO：拥有相应装备数量
    uint32 ret = 0;
	ret = m_bag->GetQualityAndStarCount(quality,star);
    return ret;
}
uint32 Player::count_equip(uint32 quality){
	return CountEquip(quality,0);
}
uint32 Player::wear_equip(uint32 quality){
	return WearEquip(quality,0);
}
uint32 Player::WearEquip(uint32 quality, uint32 star)
{
    // TODO：穿戴相应装备数量
    uint32 ret = 0;
	ret = m_characterStorage->WearQualityAndStarCount(this,quality,star);
    return ret;
}
bool Player::HaveTechnology(uint32 id) const
{
    return m_Technology->Have(id);
}

uint32 Player::CountEquipClass(uint type){
	uint32 ret = 0;
	ret = m_bag->CountEquipClass(type);
	return ret;
}
uint32 Player::WearEquipClass(uint type){	
	uint32 ret  = 0;
	ret = m_characterStorage->WearEquipClass(this,type);
	return ret;
}
uint32 Player::HaveEquipClass(uint type){
	uint32 ret  = 0;
	ret = m_bag->HaveEquipClass(type);
	return ret;
}

uint64 Player::GetGuid() const
{
 return PlayerValueOwner::GetGuid();
}

uint64 Player::AccountId() const
{
 return PlayerValueOwner::AccountId();
}

uint32 Player::GetChannelId() const
{
 return PlayerValueOwner::GetChannelId();
}

uint32 Player::GetPlatformId() const
{
	return PlayerValueOwner::GetPlatformId();
}

void Player::SyncPlayerData(pb::PlayerAllData& allData)
{
#ifdef _MMO_SERVER_
	if (m_marketLog)
	{
		allData.clear_market_info();
		m_marketLog->SaveTo(*allData.mutable_market_info());
	}
	if (m_fubenLog)
	{
		allData.clear_fuben_info();
		m_fubenLog->SaveTo(this,*allData.mutable_fuben_info());
	}
	//if(m_worldMapLogic)
	//{
	//	allData.clear_world_map_logic();
	//	m_worldMapLogic->SaveTo(*allData.mutable_world_map_logic());
	//}
#endif
}

void Player::UpdateCurDateTo( pb::PlayerAllData& allData )
{
#ifdef _MMO_SERVER_

	allData.clear_behavior_count();
	allData.clear_quest_log();
	allData.clear_character_info();
	allData.clear_full_value();
	//allData.clear_string_data();
	allData.clear_bag_info();
	allData.clear_world_map_logic();

	if (m_questLogger)
	{
		m_questLogger->Save(this, *allData.mutable_quest_log());
	}

	if (m_characterStorage)
	{
		m_characterStorage->SaveTo( *allData.mutable_character_info());
		m_characterStorage->SetUnmodified();
	}

	if (m_bag)
	{
		m_bag->WriteAllItemTo(*allData.mutable_bag_info());
	}

	if (m_worldMapLogic)
	{
		m_worldMapLogic->SaveTo(*allData.mutable_world_map_logic());
	}
	if (m_behaviorCounter)
	{
		m_behaviorCounter->SaveTo(*allData.mutable_behavior_count());
	}
	SaveValue(*allData.mutable_full_value());
#endif
}

uint32 Player::OccupyCityCount() const
{
	return GetAttrUint(pb::PLAYER_FIELD_OCCUPY_RANK_VALUE);
}


uint32 Player::KillEnemy() const
{
	return static_cast<uint32>(GetAttrUint64(pb::PLAYER_FIELD_KILL_NUM));
}

uint32 Player::TrickNum() const
{
	return GetAttrUint(pb::PLAYER_FIELD_TRICK_RANK_VALUE);
}

uint32 Player::AddSolider() const
{
	return GetCreateShadowNum();
}

void Player::SetCreateShadowNum( uint32 num )
{
	SetValueInt(pb::PLAYER_FIELD_CREATESHADOW_NUM,num);
}

uint32 Player::GetCreateShadowNum() const
{	
	return GetAttrUint(pb::PLAYER_FIELD_CREATESHADOW_NUM);
}

uint32 Player::GetMaxGemLevel()
{
	const TecTable* tec = NULL;
	if(HaveTechnology(BAOSHIHECHENG5))
	{
		tec = sTechnologyTableMgr.GetTable(BAOSHIHECHENG5);
		return tec->Value1();
	}
	
	if(HaveTechnology(BAOSHIHECHENG4))
	{
		tec = sTechnologyTableMgr.GetTable(BAOSHIHECHENG4);
		return tec->Value1();
	}

	if(HaveTechnology(BAOSHIHECHENG3))
	{
		tec = sTechnologyTableMgr.GetTable(BAOSHIHECHENG3);
		return tec->Value1();
	}

	if(HaveTechnology(BAOSHIHECHENG2))
	{
		tec = sTechnologyTableMgr.GetTable(BAOSHIHECHENG2);
		return tec->Value1();
	}

	if(HaveTechnology(BAOSHIHECHENG1))
	{
		tec = sTechnologyTableMgr.GetTable(BAOSHIHECHENG1);
		return tec->Value1();
	}

	return 0;
}

float Player::GetHeroSpeedPercent()
{
	float speed = 1.0;
	if(TestFlag(pb::PLAYER_UP_WORD_MAP_MOVE_SPEED))
		speed = (float)(speed - 0.1);

	//TODO:改造主城提升速度

	return speed;
}

//不判断上限
void Player::AddDinnerNum( int value )
{
	uint32 old_value = GetAttrUint(pb::PLAYER_FIELD_DINNER_NUMBER);
	int new_value = (int)old_value + value;
	SetValueInt(pb::PLAYER_FIELD_DINNER_NUMBER,new_value);
}

uint32 Player::HurtToExploit(uint32 type,uint32 hurt)
{
	uint64 old_kill_num = GetAttrUint64(pb::PLAYER_FIELD_KILL_NUM);
	AddValue64(pb::PLAYER_FIELD_KILL_NUM,hurt);
	if(GetKillFromLevel() == 0) return 0;
	int n = (int)(GetAttrUint64(pb::PLAYER_FIELD_KILL_NUM) / GetKillFromLevel() - old_kill_num / GetKillFromLevel() );
#ifdef _MMO_SERVER_
	if(type == ENUM_TYPE_KILL)//杀敌功勋翻倍
	{
		if(TestFlag(pb::PLAYER_UP_KILL_ENEMY_HORNOR))
		{
			n <<= 1;
		}
	}
#endif
	AddExploitValue(n);
	return n;
}

void Player::DoUpdatePer_FourHour_Zero()
{
	//宴会次数加一
	AddValue(pb::PLAYER_FIELD_DINNER_NUMBER,1);
}

uint32 Player::GetKillFromLevel() const
{
	uint32 level = GetAttrUint(pb::PLAYER_FIELD_EXPLOIT_RANK_BOX_LEVEL);
	return sExploitMgr.__GetKillFromLevel(level);
}

void Player::DescreaseDinnerNum(int num)
{
	uint32 old_value = GetAttrUint(pb::PLAYER_FIELD_DINNER_NUMBER);
	int new_value = (int)old_value + num;
	SetValueInt(pb::PLAYER_FIELD_DINNER_NUMBER,new_value);
}
//返回true,没有领过
bool Player::IsTakeDailyQuestReward(uint32 id)
{
	uint32 cur_score = GetDailyQuestScore();
	const DailyQuestReward* daily_quest_reward = sDailyQuestTableMgr.GetDailyQuestReward(id);
	if (daily_quest_reward->RewardScore() <= cur_score )
	{
		if(daily_quest_reward->UnlockLevel() == 0)
		{
			int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG);
			return (curTakeFlag & (1 << id )) == 0;
		}
		else
		{
			if(static_cast<uint32>(GetLevel()) < daily_quest_reward->UnlockLevel())
				return false;
			else
			{
				int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG);
				return (curTakeFlag & (1 << id )) == 0;
			}
		}
	}
	else
	{
		return false;
	}
}

bool Player::TakeDailyQuestReward(uint32 id)
{
	uint32 cur_score = GetDailyQuestScore();
	const DailyQuestReward* daily_quest_reward = sDailyQuestTableMgr.GetDailyQuestReward(id);
	if (daily_quest_reward->RewardScore() <= cur_score)
	{
		int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG );
		curTakeFlag = curTakeFlag | (1 << id);
		SetValueInt(pb::PLAYER_FIELD_TAKE_DAILY_QUEST_REWARD_FLAG,curTakeFlag);
		return true;
	}
	return false;
}

void Player::UpdateAllDailyQuestInfo( DailyQuestType type,uint32 value, const uint32 city_id )
{
	const uint32 quest_id = sDailyQuestTableMgr.GetQuestIdFromQuestType(type,city_id);
	if(quest_id == 0) return;
	m_DailyQuest->UpdateDailyQuestData(*this,quest_id,value);
	DailyQuestDb::SendUpdateInfoToDb(*this,quest_id,m_DailyQuest->GetDailyQuestStruct(quest_id));
	m_DailyQuest->SendUpdateInfoToClient(*this,quest_id);
}

void Player::resetDailyQuest()
{
	DailyQuestStructMap& daily_quest_struct_map = m_DailyQuest->GetMutableDailyQuestStructMap();
	for(DailyQuestStructMap::iterator Iter = daily_quest_struct_map.begin();Iter != daily_quest_struct_map.end();++Iter)
	{
		if(Iter->second.finish_times != 0 || Iter->second.is_finished != 0)
		{
			Iter->second.finish_times = 0;
			Iter->second.is_finished = 0;
			DailyQuestDb::SendUpdateInfoToDb(*this,Iter->first);
		}
	}
	
}
void Player::UnlockCharacter( std::set< uint32>& idSet )
{
	uint32 autoUnlockCharId = GetSecondAutoUnlockCharacterId();
	for(std::set< uint32>::iterator iterId = idSet.begin(); iterId != idSet.end(); ++iterId)
	{
		uint32 charItemId = (*iterId);
		const LogicItemProto* logicProto = sItemMgr.Find(charItemId);
		if ( logicProto)
		{
			if ( logicProto->IsCard())
			{
				if (logicProto->value02 == 0 || logicProto->value02 == GetCountryId())
				{
					uint32 char_proId = logicProto->ObjectValue();
					if (m_characterStorage->HasCharacterProto(char_proId ))
						continue;
					if (char_proId > 0)
					{
						Character * character = PlayerCharacterLogic::CreateCharacterForPlayer(this, char_proId);
						if( character != NULL )
						{
							AddEventLog(EVENT_PALYER_ITEM_SYS,
								4,
								GetLevel(),
								char_proId,
								1,
								IR_STAGE_LEVEL_LOOT_GET);
							{
								CheckTecById(character,FIGHT_ENFORCE1);
								CheckTecById(character,FIGHT_ENFORCE4);
								CheckTecById(character,FIGHT_ENFORCE5);
								CheckTecById(character,ARMY_ENFORCE1);
								CheckTecById(character,ARMY_ENFORCE3);
							}
							
							if (char_proId == autoUnlockCharId)
							{
								CharacterIds ids;
								const CharacterIds& curBattleArr = m_characterStorage->GetBattleArray();
								for (CharacterIds::const_iterator it = curBattleArr.begin(); it != curBattleArr.end(); ++it)
								{
									uint32 charaId = *it;
									if (charaId != 0)
									{
										ids.push_back(charaId);
									}
								}
								ids.push_back(character->GetID());
								ItemCharacterLogic::SetBattleArray( this, ids);
							}
						}
					}
				}
			}
		}
	}
}

uint32 Player::GetFirstAutoUnlockCharacterId()
{
	uint32 curCountry = GetCountryId();
	uint32 char_proId = 0;
	switch(curCountry)
	{
	case pb::COUNTRY_WORM:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_FIRST_CHAR);
		}
		break;
	case pb::COUNTRY_MAN:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_FIRST_CHAR_TERRAN);
		}
		break;
	case pb::COUNTRY_GOD:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_FIRST_CHAR_PROTOSS);
		}
		break;
	}
	return char_proId;
}

uint32 Player::GetSecondAutoUnlockCharacterId()
{
	uint32 curCountry = GetCountryId();
	uint32 char_proId = 0;
	switch(curCountry)
	{
	case pb::COUNTRY_WORM:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_SECOND_CHAR_ZERG);
		}
		break;
	case pb::COUNTRY_MAN:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_SECOND_CHAR_TERRAN);
		}
		break;
	case pb::COUNTRY_GOD:
		{
			char_proId = GET_BASE_DEF_UINT( BD_NEW_PLAYER_CREATE_SECOND_CHAR_PROTOSS);
		}
		break;
	}
	return char_proId;
}

void Player::UpdateAllGrowUpQuestInfo( pb::GrowUpQuestType type,uint32 value )
{
	uint32 group_id = sGrowUpQuestTableMgr.GetGroupIdFromType(static_cast<uint32>(type));
	if(group_id == 0) return;
	uint32 open_time = sGrowUpQuestTableMgr.GetOpenTimeFromGroupId(group_id);
	if(GetDaysFromRegist() < open_time) return;
	if(GetSecondFromRegist() <= GROWUP_LAST_TIME_LONG)
	{
		const uint32 quest_id = sGrowUpQuestTableMgr.GetQuestIdByGroupIdAndType(group_id,type,value);
		if(quest_id == 0) return;
		m_GrowUpQuest->UpdateGrowUpData(this,type,value);
		m_GrowUpQuest->SendUpdateDataToClient(*this,quest_id);
		UpdateGrowUpQuestGroupInfo(group_id);
	}
}

void Player::UpdateGrowUpQuestGroupInfo( const uint32 group_id )
{
	m_GrowUpQuest->UpdateGroupData(this,group_id);
	m_GrowUpQuest->SendUpdateGroupDataToClient(*this,group_id);	
}

void Player::GiveExploitBox()
{
	uint32 current_exploit_value = 0;
	uint32 box_level = 0;
	size_t count = 0;
	current_exploit_value = GetExploitValue();
	box_level = GetAttrUint(pb::PLAYER_FIELD_EXPLOIT_RANK_BOX_LEVEL);


	while( !box_queue_.empty()&& box_level >= box_queue_.front().exploit_level  && count++ < cycle_times)
	{
		while(!box_queue_.empty() &&box_level > box_queue_.front().exploit_level)
		{
			box_queue_.pop_front();
		}
		if(box_queue_.empty()) break;
		if( current_exploit_value >= box_queue_.front().need_exploit )
		{
			if(box_queue_.front().box_type == 1)
			{
				AddValue(pb::PLAYER_FIELD_EXPLOIT_RANK_NORMAL_BOX,box_queue_.front().reward_box_num);
				SetValueInt(pb::PLAYER_FIELD_TAKE_EXPLOIT_BOX_STEP,box_queue_.front().id);
				box_queue_.pop_front();
			}
			else if(box_queue_.front().box_type == 2 && HaveTechnology(ZHANGONGHEHE))
			{
				AddValue(pb::PLAYER_FIELD_EXPLOIT_RANK_ADVANCE_BOX,box_queue_.front().reward_exbox_num);
				SetValueInt(pb::PLAYER_FIELD_TAKE_EXPLOIT_BOX_STEP,box_queue_.front().id);
				box_queue_.pop_front();
			}
		}
	}
}

void Player::UpdateRankSortToMap( ranksortMap &map_,uint32 lv,uint32 vip_lv,uint64 player_id,uint8 country_id,uint32 rank_sort_value,const string& name,uint32 old_value,uint32 head_icon_id)
{
	if(rank_sort_value == 0) return;
	RankSort old_temp(country_id,old_value,lv,vip_lv,player_id,head_icon_id);
	RankSort temp(country_id,rank_sort_value,lv,vip_lv,player_id,head_icon_id);
	if(map_.size() < MAX_VEC_NUM)
	{
		ranksortMap::iterator iter = map_.find(old_temp);
		if(iter != map_.end())
		{
			map_.erase(iter);//don't use swap because I need this to be ordered.swap function can't do that.
		}
		map_[temp] = name;
	}
	else
	{
		ranksortMap::reverse_iterator iter = map_.rbegin();
		if(iter->first<temp)
		{
			map_.erase(iter.base());
			map_[temp] = name;
		}
	}
}


void Player::ZhaoMuOneMoreChar( const uint32 tid )
{
	const TecTable* tec = sTechnologyTableMgr.GetTable(tid);
	if(!tec) return;
	const uint32 member_count = m_characterStorage->GetBattleMemberCount(this) + tec->Value1();
	m_characterStorage->SetBattleMemberCount(this,member_count);
	m_characterStorage->BattleCharAttrResize(m_characterStorage->GetBattleMemberCount(this));
	SetValueInt(pb::PLAYER_FIELD_CARD_SLOT_COUNT,m_characterStorage->GetBattleMemberCount(this));
}

void Player::ChangeCharAttrByTecnology( const uint32 tid )
{
	const TecTable *tec =  sTechnologyTableMgr.GetTable(tid);
	if(!tec) return;
	CharacterContainer& char_container = m_characterStorage->GetMutableCharacterContainer();
	Character* character = NULL;
	if(tid == ARMY_ENFORCE1 || tid == ARMY_ENFORCE3)
	{
		uint32 cur_row_num = GetCurSolderRowNum();
		SetCurSolderRowNum(cur_row_num + tec->Value1());
	}
	for(CharacterContainer::const_iterator it = char_container.begin();it!=char_container.end();++it)
	{
		character = it->second;
		if(character)
		{
			if(tid == FIGHT_ENFORCE1 ||tid == FIGHT_ENFORCE4 || tid == FIGHT_ENFORCE5)
			{
				character->soldierLv = tec->Value1();
			}
			character->CalculateAttr();
		}
	}
	
}

void Player::CheckTecById(Character* charat, const uint32 tid )
{
	if(HaveTechnology(tid))
	{
		const TecTable *tec =  sTechnologyTableMgr.GetTable(tid);
		if(tid == FIGHT_ENFORCE1 ||tid == FIGHT_ENFORCE4 || tid == FIGHT_ENFORCE5)
			charat->soldierLv = tec->Value1();
		//charat->CalculateAttr();///在招募的时候再刷新属性
	}
}



bool Player::IsTakeDailySignExtraReward( uint32 day )
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_SIGN_EXTRA_REWARD_FLAG);
	return (curTakeFlag & (1 << day )) == 0;
}

void Player::TakeDailySignExtraReward( uint32 day )
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_SIGN_EXTRA_REWARD_FLAG );
	curTakeFlag = curTakeFlag | (1 << day);
	SetValueInt(pb::PLAYER_FIELD_SIGN_EXTRA_REWARD_FLAG,curTakeFlag);
}

uint32 Player::GetDaysFromRegist()//当前时间距离注册的天数
{
	time_t regist_time = static_cast<time_t>(GetRegistTime());
	struct tm struct_tm;
	sOS.LocalTime(struct_tm,regist_time);
	struct_tm.tm_hour =0;
	struct_tm.tm_min = 0;
	struct_tm.tm_sec = 0;
	regist_time = ::mktime(&struct_tm);
	uint32 day = static_cast<uint32>((sOS.GetRealTime() - regist_time)/ONE_DAY_SECOND);
	return day + 1;
}

uint32 Player::GetDailySignDay()
{
	return 	GetAttrUint(pb::PLAYER_FIELD_CONTUINE_SIGNUP_DAY);
}

uint32 Player::GetDailyRetroactiveDay()
{
	 return GetAttrUint(pb::PLAYER_FIELD_RETROACTIVE_DAY);
}

void Player::AddDailySignDay( uint32 value )
{
	uint32 day = GetAttrUint(pb::PLAYER_FIELD_CONTUINE_SIGNUP_DAY);
	day += value;
	SetValueInt(pb::PLAYER_FIELD_CONTUINE_SIGNUP_DAY,day);
}

void Player::AddDailyRetroactiveDay( uint32 value )
{
	uint32 day = GetAttrUint(pb::PLAYER_FIELD_RETROACTIVE_DAY);
	day += value;
	SetValueInt(pb::PLAYER_FIELD_RETROACTIVE_DAY,day);
}

void Player::UpdateDateTime()
{
	pb::GS2C_Update_date sendmsg;
	uint32 day = GetDaysFromRegist();
	sendmsg.set_day(day);
	sendmsg.set_is_signed(0);
	Send(pb::SMSG_UPDATE_DATE_RSP,sendmsg);
}

void Player::SetCurSolderRowNum(uint32 num)
{
	SetValueInt(pb::PLAYER_FIELD_CHAR_ROW_NUM,num);
}

uint32 Player::GetCurSolderRowNum()
{
	return GetAttrUint(pb::PLAYER_FIELD_CHAR_ROW_NUM);
}


void Player::AutoGiveDailySignUpReward()
{
	if(GetSecondFromRegist() <= sSignUpTableMgr.GetDailySignLastLongDay() * ONE_DAY_SECOND)
	{
		return;
	}
	if(GetDailySignDay() == 0) return;
	for(uint32 i = 0;i < GetDailySignDay() ;++i)
	{
		if(IsTakeDailySignExtraReward(i))
		{
			TakeDailySignExtraReward(i);
			SignUp* sign_up = sSignUpTableMgr.GetSignUp(i);
			if(!sign_up) return;
			//额外奖励
			const std::vector<int>& extra_reward = sign_up->GetExtraReward();
			const std::vector<int>& extra_reward_num = sign_up->GetExtraRewardNum();
			size_t count = 0;
			string items;
			for(std::vector<int>::const_iterator iter = extra_reward.begin();iter != extra_reward.end() 
				&& count < extra_reward_num.size();++iter)
			{
				if(*iter == 0 || extra_reward_num[count] == 0 ) continue;
				sReward.Change(*this,*iter,extra_reward_num[count],true);
				count++;
				Utility::PacketItemsToString(items,*iter,extra_reward_num[count]);
			}
#ifdef _MMO_SERVER_

			sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_SIGN_UP,items);
#endif
		}
	}
	SetValueInt(pb::PLAYER_FIELD_CONTUINE_SIGNUP_DAY,0);
	SetValueInt(pb::PLAYER_FIELD_RETROACTIVE_DAY,0);
}	

void Player::AutoGivePassStageReward()
{
	if(IsPassStageGiveGoldActive())
	{
		return;
	}
	if(m_PassStageReward->GetPassStageRewardStructMap().empty())
		return;
	string items;
	const ActivityStageRewardMap& reward_map = sActivityStageRewardTableMgr.GetActivityStageRewardMap();
	for(ActivityStageRewardMap::const_iterator iter = reward_map.begin();iter != reward_map.end();++iter)
	{
		if(m_PassStageReward->CheckStageIsTakeReward(iter->first)) continue;
		if(m_PassStageReward->CanTakeReward(iter->first))
		{
			sReward.Change(*this,iter->second->RewardItemId(),iter->second->RewardItemNum(),true);
			Utility::PacketItemsToString(items,iter->second->RewardItemId(),iter->second->RewardItemNum());
		}
	}
#ifdef _MMO_SERVER_

	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_PASS_STAGE,items);
#endif
	m_PassStageReward->Clear(*this);
}

void Player::AutoGiveGrowUpQuestReward()
{
	if(GetSecondFromRegist() <= GROWUP_LAST_TIME_LONG)
	{
		return;
	}
	if(m_GrowUpQuest->CanSendRewardEmail())
	{
		string items;
		const GrowUpQuestRewardVec& grow_up_quest_proto_reward = sGrowUpQuestTableMgr.GetGrowUpQuestProtoRewardVec();
		for(GrowUpQuestRewardVec::const_iterator iter =grow_up_quest_proto_reward.begin();iter != grow_up_quest_proto_reward.end();++iter)
		{
			if(m_GrowUpQuest->CanTakeReward(pb::REWARD_NORMAL,iter->first))
			{
				sReward.Change(*this,iter->second,true);
				Utility::PacketItemsToString(items,iter->second);
			}
		}

		const GrowUpQuestRewardVec& grow_up_quest_group_reward = sGrowUpQuestTableMgr.GetGrowUpQuestGroupRewardVec();
		for(GrowUpQuestRewardVec::const_iterator iter =grow_up_quest_group_reward.begin();iter != grow_up_quest_group_reward.end();++iter)
		{
			if(m_GrowUpQuest->CanTakeReward(pb::REWARD_GROUP,iter->first))
			{
				sReward.Change(*this,iter->second,true);
				Utility::PacketItemsToString(items,iter->second);
			}
		}
#ifdef _MMO_SERVER_
		sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_GROUP_QUEST,items);
#endif
	}
	m_GrowUpQuest->Clear(*this);
}

void Player::AddExploitValue(uint32 value)
{
	uint32 old_value = GetExploitValue();
	uint32 new_value = old_value + value;
	SetExploitValue(static_cast<int>(new_value));
	if(GetSecondFromRegist() <= GROWUP_LAST_TIME_LONG)
	{
		AddValue(pb::PLAYER_FIELD_GROW_UP_QUEST_EXPLOIT_VALUE,static_cast<int>(value));
	}
	if(TestFlag(pb::PLAYER_FLAG_QUEST_GET_EXPLOIT_ACTIVE))
	{
		AddValue(pb::PLAYER_FIELD_MAIN_QUEST_GET_EXPLOIT,static_cast<int>(value));
	}
	if(TestFlag(pb::PLAYER_FLAG_BRANCH_QUEST_GET_EXPLOIT_ACTIVE))
	{
		AddValue(pb::PLAYER_FIELD_BRANCH_QUEST_EXPLOIT,static_cast<int>(value));///分支任务功勋
	}
}
void Player::AddTrickValue(uint32 value){
	uint32 old_value = GetTrickValue();
	uint32 new_value = old_value + value;
	SetTrickValue(static_cast<int>(new_value));
}
void Player::AddOccupyValue(uint32 value,const uint32 city_id /* = 0*/){
	uint32 old_value = GetOccupyValue();
	uint32 new_value = old_value + value;
	SetOccupyValue(static_cast<int>(new_value));
#ifdef _MMO_SERVER_
	if(IsActivityStillActive(pb::ACTIVITY_COUNTRY_RANK))
	{
		AddCountryOccupyCityNum(value);
		sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_COUNTRY_RANK,GetGuid());
	}
	if(IsActivityStillActive(pb::ACTIVITY_OCCUPATION_RANK))
	{
		AddValue(pb::PLAYER_FIELD_ACTIVITY_OCCUPY_CITY_NUM,value);
		sActivityMgr.MarkTakeInActivity(pb::ACTIVITY_OCCUPATION_RANK,GetGuid());
	}
	if(city_id == 0) return;
// 	if(!sWorldFightAchievementMgr.IsInvalidCityId(city_id))
// 	{
// 		AddValue(pb::PLAYER_FIELD_OCCUPY_CITY_NUM,value);//此时，攻打的城市也得加
// 		AddValue(pb::PLAYER_FIELD_ATTACK_CITY_NUM,value);
// 	}
#endif
}

void Player::AddXiLianValue( int value )
{
	uint32 old_value = GetAttrUint(pb::PLAYER_FIELD_XILIAN_COUNT);
	uint32 new_value = old_value + value;
	SetValueInt(pb::PLAYER_FIELD_XILIAN_COUNT,static_cast<int>(new_value));
}
// void Player::AddAttackCityValue(int32 value,const uint32 city_id)
// {
// // #ifdef _MMO_SERVER_
// // 	if(!sWorldFightAchievementMgr.IsInvalidCityId(city_id))
// // 	{
// // 		AddValue(pb::PLAYER_FIELD_ATTACK_CITY_NUM,value);
// // 	}
// // #endif
// }

void Player::AddActivityStarBoxExp( const uint32 exp )
{
	if(IsStarBoxStillActive())
	{
		AddValue(pb::PLAYER_FIELD_HORNOR,exp);
	}
}
void Player::OnDestoryItemLog( pb::ITEM_REASON reason, int entry, int count )
{
	OnDestoryItem(reason,entry,count);
}

uint32 Player::GetActivityStarBoxExp()
{
	return GetAttrUint(pb::PLAYER_FIELD_HORNOR);
}


bool Player::IsActivityStillActive(const uint32  type)
{
#ifdef _MMO_SERVER_
	return sActivityMgr.IsActivityActive(type);
#else
	return false;
#endif
}


void Player::TryDeductStarBoxExp(int exp )
{
	if(IsStarBoxStillActive())
	{
		if(exp > 0)
		{
			AddValue(pb::PLAYER_FIELD_HORNOR,-exp);
		}
		else
		{
			AddValue(pb::PLAYER_FIELD_HORNOR,exp);
		}
	}
}

bool Player::IsPassStageGiveGoldActive()
{
	return GetSecondFromRegist() <= FINISH_STAGE_AND_GIVE_GOLD_LONG;
}

bool Player::IsStarBoxStillActive()
{
	return IsActivityStillActive(pb::ACTIVITY_STAR_BOX);
}

time_t Player::GetSecondFromRegist()
{
	time_t regist_time = static_cast<time_t>(GetRegistTime());
	return sOS.GetRealTime() - regist_time;
}

void Player::UpdatePlayerNameToMap(ranksortMap &map_,uint32 lv,uint32 vip_lv,uint64 player_id,uint8 country_id,uint32 rank_sort_value,const string& new_name,uint32 head_icon_id)
{
	if(rank_sort_value == 0) return;
	RankSort temp(country_id,rank_sort_value,lv,vip_lv,player_id,head_icon_id);
	ranksortMap::iterator iter = map_.find(temp);
	if(iter!= map_.end())
	{
		if(iter->second != new_name)
			iter->second = new_name;
	}
}
void Player::UpdatePlayerLvToMap(ranksortMap& map_,uint32 new_lv,uint32 vip_lv,uint64 player_id,uint8 country_id,uint32 rank_sort_value,uint32 old_level,const string& name,uint32 head_icon_id)
{
	if(rank_sort_value == 0) return;
	RankSort temp(country_id,rank_sort_value,new_lv,vip_lv,player_id,head_icon_id);
	RankSort old_temp(country_id,rank_sort_value,old_level,vip_lv,player_id,head_icon_id);
	ranksortMap::iterator iter = map_.find(old_temp);
	if(iter!= map_.end())
	{
		map_.erase(iter);
		map_[temp] = name;
	}
}
bool Player::IsTakeOpenFogReward( uint32 num )
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_TAKE_FOG_REWARD_FLAG);
	return (curTakeFlag & (1 << num )) == 0;
}

void Player::TakeOpenFogReward( uint32 num )
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_TAKE_FOG_REWARD_FLAG );
	curTakeFlag = curTakeFlag | (1 << num);
	SetValueInt(pb::PLAYER_FIELD_TAKE_FOG_REWARD_FLAG,curTakeFlag);
}

void Player::AutoGiveOpenFogReward()
{
	//TODO:CHECK activity IS AVAIL!!!!
#ifdef _MMO_SERVER_
	if(IsOpenFogIsActive()) return;
	if(GetAttrUint(pb::PLAYER_FIELD_OPEN_FOG_COUNT) == 0) return;
	IntPair pair_reward;
	string items;
	const OpenFogMap& open_fog_map = sMapTableMgr.GetOpenFogMap();
	for(OpenFogMap::const_iterator iter = open_fog_map.begin();iter != open_fog_map.end();++iter)
	{
		if(GetAttrUint(pb::PLAYER_FIELD_OPEN_FOG_COUNT) < iter->second->OpenNum())
		{
			continue;
		}
		else
		{
			if(IsTakeOpenFogReward(iter->first))
			{
				TakeOpenFogReward(iter->first);
				if(m_worldMapLogic->TakeFogReward(iter->first,&pair_reward))
				{
					sReward.Change(*this,pair_reward.first,pair_reward.second,true);
					//Utility::PacketItemsToString(items,pair_reward);
				}
			}
		}
	}
	SetValueInt(pb::PLAYER_FIELD_OPEN_FOG_COUNT,0);
	//sSysEventMgr.SendRankBoxMail(GetGuid(),17,items);

#endif
}

bool Player::IsOpenFogIsActive()
{
#ifdef _MMO_SERVER_
	if(sOS.GetRealTime() <= GetEnterWorldTime() + OPEN_FOG_LAST_LONG)
	{
		return true;
	}
#endif
	return false;
}

void Player::AddActivityXiLianValue( int value )
{
	if(IsActivityStillActive(pb::ACTIVITY_XILIAN))
	{
		AddValue(pb::PLAYER_FIELD_ACTIVITY_XILIAN_COUNT,value);
		AddValue(pb::PLAYER_FIELD_XILIAN_TEN_TIMES,value);
	}
}

uint32 Player::GetActivityXiLianValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_XILIAN_COUNT);
}

bool Player::IsTakeXilianReward( uint32 id )
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_TAKE_XILIAN_REWARD_FLAG);
	return (curTakeFlag & (1 << id )) == 0;
}

void Player::TakeXilianReward( uint32 id )
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_TAKE_XILIAN_REWARD_FLAG );
	curTakeFlag = curTakeFlag | (1 << id);
	SetValueInt(pb::PLAYER_FIELD_TAKE_XILIAN_REWARD_FLAG,curTakeFlag);
}

bool Player::IsXilianActive()
{
	return IsActivityStillActive(pb::ACTIVITY_XILIAN);
}	

void Player::AutoGiveXilianReward()
{
#ifdef _MMO_SERVER_
	if(IsXilianActive())
	{
		return;
	}
	if(GetActivityXiLianValue() == 0)
		return;
	std::string items;
	const ActivityXilianMap& xilian_map = sActivityXilianMgr.GetActivityXilianMap();
	for(ActivityXilianMap::const_iterator iter = xilian_map.begin();iter!= xilian_map.end();++iter)
	{

		if(GetActivityXiLianValue() < iter->second->NeedTimes())
		{
			continue;
		}
		else
		{
			if(IsTakeXilianReward(iter->first))
			{
				TakeXilianReward(iter->first);
				const IntPairVec& pair_vec = iter->second->GetRewardList();
				sReward.Change(*this,pair_vec,true);
				Utility::PacketItemsToString(items,pair_vec);
			}
		}
	}
	SetValueInt(pb::PLAYER_FIELD_ACTIVITY_XILIAN_COUNT,0);
	SetValueInt(pb::PLAYER_FIELD_TAKE_XILIAN_REWARD_FLAG,0);

	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_XILIAN,items);
#endif
}

void Player::UseCrossHammer()
{
	if(cross_hammer_num > 0)
		cross_hammer_num--;
}

int Player::GetCrossDoubleKill()
{
	if(cross_kill_troops)	//有击杀令效果
	{
		if(sOS.TimeSeconds() >= cross_kill_troops_endtime)	//超时，则取消击杀令效果
		{
			cross_kill_troops = false;
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
		return 1;
}

void Player::AddCrossDoubleKill()
{
	uint64 nowtime = sOS.TimeSeconds();
	if(cross_kill_troops_endtime < nowtime)
	{
		cross_kill_troops_endtime = nowtime;
	}

	cross_kill_troops_endtime += 600;
	cross_kill_troops = true;

}

uint64 Player::GetCrossKillEndTime()
{
	return cross_kill_troops_endtime;
}

void Player::AutoActivityStarBoxReward()
{
#ifdef _MMO_SERVER_
	if(IsStarBoxStillActive())
	{
		return;
	}
	uint32 cur_box_level = GetActivityStarBoxLevel();
	uint32 cur_score = GetActivityStarBoxExp();
	if(cur_box_level == 1 && cur_score == 0)
		return;
	
	uint32 id = sActivityStarBoxMgr.GetId(this);
	if(id == 0) return;
	const ActivityStarBox* ac_box = sActivityStarBoxMgr.GetActivityStarBox(id);
	if(!ac_box) 
	{
		return;
	}
	std::string items;
	ActivityStarBoxStructMap& star_box_map = m_ActivityStarBoxLog->GetActivityStarBoxStructMap();
	if(cur_score >= ac_box->NeedScore())
	{
		ActivityStarBoxStruct & ref = star_box_map[id];
		if(ref.is_take_reward == 0)
		{
			const IntPairVec& vec = ac_box->GetRewardList();
			if(ac_box->NextLvNeedWaitTime() != 0)
			{
				for(int32 i = 0;i < ref.npc_left_reward_times;++i)
				{
					sReward.Change(*this,vec,true);
				}
			}
			else
			{
				for(uint32 i = 0;i < ac_box->RewardTimes();++i)
				{
					sReward.Change(*this,vec,true);
				}
			}
			Utility::PacketItemsToString(items,vec);

			sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_ACT_STAR_BOX,items);
		}
	}
	SetValueInt(pb::PLAYER_FIELD_HORNOR,0);
	SetValueInt(pb::PLAYER_FIELD_ACTIVITY_STAR_BOX_LEVEL,1);
	m_ActivityStarBoxLog->Clear();
	
#endif
}


void Player::SetRegistTime( time_t registTime )
{
	SetValueUint64(pb::PLAYER_FIELD_REGIST_TIME,registTime);
}

bool Player::IsTakeIronCollectReward(uint32 id)
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_IRON_COLLECT_REWARD_FLAG);
	return (curTakeFlag & (1 << id )) == 0;
}

void Player::TakeIronCollectReward(uint32 id)
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_IRON_COLLECT_REWARD_FLAG );
	curTakeFlag = curTakeFlag | (1 << id);
	SetValueInt(pb::PLAYER_FIELD_IRON_COLLECT_REWARD_FLAG,curTakeFlag);
}

void Player::AddIronCollectTimesAndTen(uint32 value)
{
	if(IsActivityStillActive(pb::ACTIVITY_IRON_COLLECT))
	{
		AddValue(pb::PLAYER_FIELD_IRON_COLLECT_TIMES,value);
		AddValue(pb::PLAYER_FIELD_IRON_COLLECT_TEN,value);
	}
}

void Player::IronCollectTimesTenMinusValue(int value)
{
	AddValue(pb::PLAYER_FIELD_IRON_COLLECT_TEN,-value);
}

uint32 Player::GetIronCollectTimes() const
{
	return GetAttrUint(pb::PLAYER_FIELD_IRON_COLLECT_TIMES);
}

uint32 Player::GetIronCollectTimesTen() const
{
	return GetAttrUint(pb::PLAYER_FIELD_IRON_COLLECT_TEN);
}

void Player::ResetIronCollectTimes()
{
	SetValueInt(pb::PLAYER_FIELD_IRON_COLLECT_TIMES,0);
	SetValueInt(pb::PLAYER_FIELD_IRON_COLLECT_REWARD_FLAG,0);
	SetValueInt(pb::PLAYER_FIELD_IRON_COLLECT_TEN,0);
}

void Player::AutoGiveIronCollectReward()
{
#ifdef _MMO_SERVER_
	if(IsActivityStillActive(pb::ACTIVITY_IRON_COLLECT))
	{
		return;
	}
	if(GetIronCollectTimes() == 0) return;
	std::string items;
	const ActivityObsidianCollectMap& iron_collect_map = sMarketTableMgr.GetActivityObsidianCollectMap();
	for(ActivityObsidianCollectMap::const_iterator iter = iron_collect_map.begin();iter != iron_collect_map.end();++iter)
	{
		if(GetIronCollectTimes() < iter->second->need_collect_times)
		{
			continue;
		}
		else
		{
			if(IsTakeIronCollectReward(iter->first))
			{
				TakeIronCollectReward(iter->first);
				IntPairVec vec;
				Utility::SplitStr2(iter->second->reward_list,vec);
				sReward.Change(*this,vec,true);
				Utility::PacketItemsToString(items,vec);
			}
		}
	}
	ResetIronCollectTimes();

	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_ACT_IRON_COLLECT,items);
#endif
}


void Player::bzero_min_zhong()
{
	SetValueInt(pb::PLAYER_FIELD_PEOPLE_LOYAL,0);
}

bool Player::IsTakeVisitFamousReward(uint32 id)
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_VISIT_FAMOUS);
	return (curTakeFlag & (1 << id )) == 0;
}

void Player::TakeVisitFamousReward(uint32 id)
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_VISIT_FAMOUS );
	curTakeFlag = curTakeFlag | (1 << id);
	SetValueInt(pb::PLAYER_FIELD_VISIT_FAMOUS,curTakeFlag);
}
bool Player::CheckHasName( const std::string& strName )
{
#ifdef _MMO_SERVER_
	PlayerSession* pSession = dynamic_cast<PlayerSession*>( m_pSession);
	if (pSession)
	{
		return pSession->CheckHasName(strName);
	}
#endif
	return false;
}

bool Player::IsAllCharInCapital()
{
	bool ret = true;
	
	uint32 cityid = sMapTableMgr.GetCapitalCity(GetCountryId());
	Character* character = NULL;
	CharacterContainer& charlst = m_characterStorage->GetMutableCharacterContainer();
	for(CharacterContainer::const_iterator charone = charlst.begin(); charone != charlst.end(); ++charone)
	{
		character = charone->second;
		if(character)
		{
			if(character->GetCurCity() != cityid)
				ret = false;
		}
	}

	return ret;
}
int Player::GetAllCharMaxHp()
{
	if (m_characterStorage)
	{
		return m_characterStorage->GetAllCharMaxHp();
	}
	return 0;
}

bool Player::MoveAllCharToCapital()
{
	bool ret = true;
	uint32 cityid = sMapTableMgr.GetCapitalCity(GetCountryId());
	Character* character = NULL;
	CharacterContainer& charlst = m_characterStorage->GetMutableCharacterContainer();
	for(CharacterContainer::const_iterator charone = charlst.begin(); charone != charlst.end(); ++charone)
	{
		character = charone->second;
		if(character)
		{
			if(!character->FlyTo(cityid))
				ret = false;
		}
	}
	return ret;
}
bool Player::CheckHasCombatChar()
{
	if (m_characterStorage)
	{
		return m_characterStorage->CheckHasCombatChar();
	}
	return false;
}

void Player::SendZhengWuInfo()
{
#ifdef _MMO_SERVER_
	m_GovAffairsLog->SendZhengWuInfo(this);
#endif
}
bool Player::IsBuyItemInLimitedShop(uint64 id)
{
	int curTakeFlag = GetAttrInt(pb::PLAYER_FIELD_LIMITED_SHOP_FLAG);
	return (curTakeFlag & (1 << id )) == 0;
}
void Player::BuyItemInLimitedShop(uint64 id)
{
	int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_LIMITED_SHOP_FLAG );
	curTakeFlag = curTakeFlag | (1 << id);
	SetValueInt(pb::PLAYER_FIELD_LIMITED_SHOP_FLAG,curTakeFlag);
}
void Player::ResetBuyLog()
{
#ifdef _MMO_SERVER_
	if(IsActivityStillActive(pb::ACTIVITY_LIMTED_TIME_SHOP))
	{
		return;
	}
	SetValueInt(pb::PLAYER_FIELD_LIMITED_SHOP_FLAG,0);
#endif
}
void Player::SendUpdateCelebrityEvent()
{
	if(IsActivityStillActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		if( m_worldMapLogic->m_CelebrityCityEventLst.second.endTime == 0 || m_worldMapLogic->m_CelebrityCityEventLst.second.endTime <= sOS.TimeSeconds())
		{
			if(m_worldMapLogic->m_CelebrityCityEventLst.first > 0) return;
			m_worldMapLogic->UpdateCelebrityCityEvent(this);
		}
		else
		{////防止服务器宕机
#ifdef _MMO_SERVER_
			sAutoUpdateGame.AddEvent(boost::bind(&PlayerMapLogic::UpdateCelebrityCityEvent, m_worldMapLogic.get(),this),(U32)::abs(int(m_worldMapLogic->m_CelebrityCityEventLst.second.endTime - sOS.TimeSeconds())));
#endif
		}
	}
}

void Player::AutoGiveCelebrityReward()
{
#ifdef _MMO_SERVER_
	if(IsActivityStillActive(pb::ACTIVITY_MAKE_FRIENDS))
	{
		return;
	}
	uint32 curRenown = GetCurrency(eRenown);
	if(curRenown == 0) return;
	std::string items;
	const ActivityVisitFamousRewardMap& visit_famous_map = sActivityCelebrityMgr.getActivityVisitFamousRewardMap();
	for(ActivityVisitFamousRewardMap::const_iterator iter = visit_famous_map.begin();iter != visit_famous_map.end();++iter)
	{
		if(curRenown < iter->second->NeedRenown())
		{
			continue;
		}
		else
		{
			if(IsTakeVisitFamousReward(iter->first))
			{
				TakeVisitFamousReward(iter->first);
				IntPairVec vec;
				Utility::SplitStr2(iter->second->Reward(),vec);
				sReward.Change(*this,vec,true);
				Utility::PacketItemsToString(items,vec);
			}
		}
	}
	ResetCelebrityCurrency();

	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_ACT_CELEBRITY,items);
#endif
}

void Player::ResetCelebrityCurrency()
{
	SetValueInt(pb::PLAYER_FIELD_RENOWN,0);
	SetValueInt(pb::PLAYER_FIELD_STARCRYSTAL,0);
	SetValueInt(pb::PLAYER_FIELD_BOARD,0);
	SetValueInt(pb::PLAYER_FIELD_GENE_CODE,0);
	SetValueInt(pb::PLAYER_FIELD_VISIT_FAMOUS,0);
}

bool Player::IsTakeReward(const uint32 index,uint32 id)
{
	int curTakeFlag = GetAttrInt(index);
	return (curTakeFlag & (1 << id )) != 0;
}
void Player::TakeReward(const uint32 index,uint32 id)
{
	int curTakeFlag =  GetAttrInt(index);
	curTakeFlag = curTakeFlag | (1 << id);
	SetValueInt(index,curTakeFlag);
}
uint32 Player::getHeroRewardBoxNum()
{
	return GetAttrUint(pb::PLAYER_FIELD_HERO_REWARD_BOX);
}
uint32 Player::getCurrentActivityGold()
{
	return GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_GOLD_NUM);
}
void Player::resetGeneralRewardData()
{
	if(IsActivityStillActive(pb::ACTIVITY_HERO_REWARD)) return;
	if(GetAttrUint(pb::PLAYER_FIELD_HERO_REWARD_BOX) == 0) return;
	SetValueInt(pb::PLAYER_FIELD_HERO_REWARD_BOX,0);
	SetValueInt(pb::PLAYER_FIELD_BEEF,0);
	SetValueInt(pb::PLAYER_FIELD_WINE,0);
	SetValueInt(pb::PLAYER_FIELD_CURR_REWARD_CHAR_ID,0);
#ifdef _MMO_SERVER_
	m_ActivityHeroRewardLog->Clear();
#endif
}
void Player::AutoGiveRechargeRebateAward()
{
#ifdef _MMO_SERVER_
	if(IsActivityStillActive(pb::ACTIVITY_MONEY_TO_GOLD)) return;
	uint32 current_gold = getCurrentActivityGold();
	if(current_gold == 0) return;
	string items;
	const ActivityRechargeRebateMap& rebate_map = sActivityRechargeRebateMgr.getActivityRechargeRebateMap();
	for(ActivityRechargeRebateMap::const_iterator iter = rebate_map.begin();iter != rebate_map.end();++iter)
	{
		if(!IsTakeReward(pb::PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG,iter->first))
		{
			if(current_gold < iter->second->NeedCharge()) continue;
			TakeReward(pb::PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG,iter->first);
			sReward.Change(*this,Reward::Coin_Gold,iter->second->Reward(),true);
			Utility::PacketItemsToString(items,Reward::Coin_Gold,iter->second->Reward());
		}
	}
	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_ACT_RECHARGE_REBATE,items);
	SetValueInt(pb::PLAYER_FIELD_ACTIVITY_GOLD_NUM,0);
	SetValueInt(pb::PLAYER_FIELD_ACTIVITY_GOLD_TAKE_FLAG,0);
#endif
}

bool Player::IsTakeWorldFightReward(const uint32 index,uint32 id ,const uint32 type)
{
#ifdef _MMO_SERVER_
	int curTakeFlag = GetAttrInt(index);
	if(type == CITY_ATTACK_CITY)
	{
		return (curTakeFlag  & (1 << id )) != 0;
	}
	else 
	{
		return ((curTakeFlag >> FLAG_SHIFT ) & (1 << id)) != 0;
	}
#endif
	return false;
}
void Player::TakeWorldFightReward(const uint32 index,uint32 id ,const uint32 type)
{
#ifdef _MMO_SERVER_
	int curTakeFlag =  GetAttrInt(index);
	if(type == CITY_ATTACK_CITY)
	{
		curTakeFlag = curTakeFlag | (1 << id);
	}
	else
	{
		curTakeFlag = curTakeFlag  | (1 << (id + FLAG_SHIFT));
	}
	SetValueInt(index,curTakeFlag);
#endif
}

uint32 Player::GetMainQuestExploitValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_MAIN_QUEST_GET_EXPLOIT);
}

void Player::AddContinueOccupyCityNum(int value)
{
	int cur_hour = sOS.TimeHour();
	if( cur_hour >= 8 && cur_hour <= 23)
	{
		if(sActMgr.IsNowActivity())
		{
			return;
		}
#ifdef _MMO_SERVER_
		Country* country = sWorldMap.GetCountry(m_worldMapLogic->GetCountryId());
		if(!country || country->IsOpenCrossWar())
		{
			return;
		}
		if(GetLevel() < GET_BASE_DEF_INT(pb::BD_SERIAL_COUNTRY_LEVELCONTROL)) return;
		if(!m_ContinueOccupyCityLog->IsInContinueOccupyCity())
		{
			float probality = sContinueOccupyCityTableMgr.getProbalityByCityNum(GetAttrInt(pb::PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY));
			if(System::Rand(0,10) <= (int)(10 * probality) - 1)//active
			{
				AddValue(pb::PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY,value);
				m_ContinueOccupyCityLog->ActiveConitinOccupyCity(*this,(time_t)sOS.TimeSeconds());
			}
		}
		else
		{
			AddValue(pb::PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY,value);
			m_ContinueOccupyCityLog->UpdateCityNum(*this,value);
		}
#endif
	}
}

void Player::ResetContinueOccupyCityNum()//23:10 reset
{
	SetValueInt(pb::PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY,0);
}

void Player::AddCountryOccupyCityNum(uint32 value)
{
#ifdef _MMO_SERVER_
	if(GetCountryId() == pb::COUNTRY_GOD)
	{
		activity_occupy_city_god += value;
		ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_GOD,activity_occupy_city_god);
	}
	else if(GetCountryId() == pb::COUNTRY_MAN)
	{
		activity_occupy_city_human += value;
		ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_MAN,activity_occupy_city_human);
	}
	else if(GetCountryId() == pb::COUNTRY_WORM)
	{
		activity_occupy_city_worm += value;
		ActivityOccupyCityDB::SendUpdateInfoToDb(pb::COUNTRY_WORM,activity_occupy_city_worm);
	}
#endif
}

void GetRankFromValue(std::vector<int>& vec)
{
	uint32 max_value = MAX(activity_occupy_city_god,MAX(activity_occupy_city_human,activity_occupy_city_worm));
	if(max_value == 0) return;
	if(max_value == activity_occupy_city_god)
	{
		vec.push_back(pb::COUNTRY_GOD);
		uint32 greater_value = MAX(activity_occupy_city_human,activity_occupy_city_worm);
		if(greater_value == activity_occupy_city_worm)
		{
			vec.push_back(pb::COUNTRY_WORM);
			vec.push_back(pb::COUNTRY_MAN);
		}
		else
		{
			vec.push_back(pb::COUNTRY_MAN);
			vec.push_back(pb::COUNTRY_WORM);
		}
	}
	else if(max_value == activity_occupy_city_human)
	{
		vec.push_back(pb::COUNTRY_MAN);
		uint32 greater_value = MAX(activity_occupy_city_god,activity_occupy_city_worm);
		if(greater_value == activity_occupy_city_worm)
		{
			vec.push_back(pb::COUNTRY_WORM);
			vec.push_back(pb::COUNTRY_GOD);
		}
		else
		{
			vec.push_back(pb::COUNTRY_GOD);
			vec.push_back(pb::COUNTRY_WORM);
		}
	}
	else if(max_value == activity_occupy_city_worm)
	{
		vec.push_back(pb::COUNTRY_WORM);
		uint32 greater_value = MAX(activity_occupy_city_god,activity_occupy_city_human);
		if(greater_value == activity_occupy_city_human)
		{
			vec.push_back(pb::COUNTRY_MAN);
			vec.push_back(pb::COUNTRY_GOD);
		}
		else
		{
			vec.push_back(pb::COUNTRY_GOD);
			vec.push_back(pb::COUNTRY_MAN);
		}
	}
}

void Player::AutoGiveCountryOccupyCityAward(std::vector<int>& vec)
{
#ifdef _MMO_SERVER_
	//if(IsActivityStillActive(pb::ACTIVITY_COUNTRY_RANK)) return;
	if(vec.empty() || vec.size() != NR_PLAYER_COUNTRY) return;
	const IntPairVec* reward = NULL;
	uint32 country_id = GetCountryId();
	if(country_id == vec[0])
	{
		reward = sActivityRankTableMgr.getRewardByCountryRank(pb::COUNTRY_WORM);
	}
	else if(country_id == vec[1])
	{
		reward = sActivityRankTableMgr.getRewardByCountryRank(pb::COUNTRY_MAN);
	}
	else if(country_id == vec[2])
	{
		reward = sActivityRankTableMgr.getRewardByCountryRank(pb::COUNTRY_GOD);
	}
	string items;
	if(reward)
	{
		sReward.Change(*this,*reward,true);
		Utility::PacketItemsToString(items,*reward);
	}

	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_COUNTRY_OCCUPY_RANK,items);
#endif
}

uint32 Player::getBranchQuestExploitValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_BRANCH_QUEST_EXPLOIT);
}

uint32 Player::getMilitaryPower()
{
	return GetAttrUint(pb::PLAYER_FIELD_MILITARY_POWER_VALUE);
}

void Player::UpdatePlayerVipLvToMap(ranksortMap& map_,uint32 lv,uint32 new_vip_level,uint64 player_id,uint8 country_id,uint32 rank_sort_value,uint32 old_vip_level,const string& name,uint32 head_icon_id)
{
	if(rank_sort_value == 0) return;
	RankSort temp(country_id,rank_sort_value,lv,new_vip_level,player_id,head_icon_id);
	RankSort old_temp(country_id,rank_sort_value,lv,old_vip_level,player_id,head_icon_id);
	ranksortMap::iterator iter = map_.find(old_temp);
	if(iter!= map_.end())
	{
		map_.erase(iter);
		map_[temp] = name;
	}
}

void Player::setMilitaryPower()
{
	uint32 total_military_power = m_characterStorage->GetMilitaryPower() + m_SeigeForceLog->GetSiegeMilitaryPower();
	SetValueInt(pb::PLAYER_FIELD_MILITARY_POWER_VALUE,total_military_power);
}

void Player::UpdatePlayerHeadIconIdToMap(ranksortMap& map_,uint32 lv,uint32 vip_level,uint64 player_id,uint8 country_id,uint32 rank_sort_value,uint32 new_head_icon_id,uint32 old_head_icon_id,const string& name)
{
	if(rank_sort_value == 0) return;
	RankSort temp(country_id,rank_sort_value,lv,vip_level,player_id,new_head_icon_id);
	RankSort old_temp(country_id,rank_sort_value,lv,vip_level,player_id,old_head_icon_id);
	ranksortMap::iterator iter = map_.find(old_temp);
	if(iter!= map_.end())
	{
		map_.erase(iter);
		map_[temp] = name;
	}
}

void Player::When24HoursPasted(const uint64 act_time)
{
	const uint64 timenow = sOS.TimeSeconds();
	string items;uint32 congratulate_count = 0;
	CongratulateStructMap & congratulate_struct_map = m_CongratulateLog->getCongratulateStructMap();
	CongratulateStructMap::const_iterator iter = congratulate_struct_map.find(act_time);
	if(iter != congratulate_struct_map.end())
	{
		if((timenow - act_time) >= ONE_DAY_SECOND)
		{
			if(iter->second.is_achieve_)
			{
				const IntPairVec* reward = sCongratulateMgr.GetCongratulateReward(iter->second.table_id_);
				if(reward)
				{
					congratulate_count += iter->second.been_congratulated_count_;
					sReward.Change(*this,*reward);
					for(IntPairVec::const_iterator iter1 = reward->begin();iter1 != reward->end();++iter1)
					{
						uint32 count = iter1->second;
						Utility::PacketItemsToString(items,iter1->first,count);
					}
					//m_CongratulateLog->SetTakeReward(iter->first);
					CongratulateLogDB::SendInfoToSelfCongratulateDb(*this,*m_CongratulateLog,act_time,pb::DATA_DEL);
				}
			}
			congratulate_struct_map.erase(iter++);
		}
	}
#ifdef _MMO_SERVER_
	if(iter != congratulate_struct_map.end())
	{
		sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursPasted, this,iter->first),(U32)(ONE_DAY_SECOND - (timenow - iter->first)));
	}
	sSysEventMgr.SendJustTipsMail(GetGuid(),EVENT_TYPE_CONGRATULATE,items,congratulate_count);
#endif
}

void Player::SendCongratulateBeenConMsg(const pb::C2GS_Congratulate_Accept& msg)
{
	m_CongratulateLog->LoadFrom(msg);
	CongratulateLogDB::SendInfoToCongratulateBeenDb(*this,msg);
}

void Player::UpdateWannaBeStrongerInfo()
{
	m_WannaBeStrongerLog->UpdateWannaBeStrongerData(*this);
}

void Player::Set24HoursPastedFunc(const uint64 timeNow)
{
#ifdef _MMO_SERVER_
		sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursPasted, this,timeNow),ONE_DAY_SECOND);
#endif
}

void Player::When24HoursToPlayerPasted(const uint64 act_time)
{
	m_CongratulateLog->ClearCongratulateBeenStructMap(*this,act_time);
}

void Player::Set24HoursToPlayerPastedFunc(const uint64 timeNow)
{
#ifdef _MMO_SERVER_
		sAutoUpdateGame.AddEvent(boost::bind(&Player::When24HoursToPlayerPasted, this,timeNow),ONE_DAY_SECOND);
#endif
}

void Player::SendUpdatePerformanceInfo()
{
#ifdef _MMO_SERVER_
	pb::SG2L_Update_Account_Login refLogin ;
	refLogin.set_player_id(GetGuid());
	refLogin.set_account_id(AccountId());
	refLogin.set_level(GetLevel());
	refLogin.set_name(CharName());
	refLogin.set_headicon(GetHeadIconId());
	refLogin.set_viplevel(GetVipLevel());
	refLogin.set_login_time(GetLastLogin());
	pb::G2LBroadcastHeader header ;
	header.set_type( pb::UPDATE_GAME_SERVER_PERFORMANCE_INFO ) ;
	pb::SG2C2L_UpdatePerformanceInfo body ;
	body.set_region_id( sGameSvr.GetRegionId() ) ;
	body.set_server_id( sGameSvr.GetServerId() ) ;
	body.set_player_count( sGameSvr.GetLogicalTaker()->GetSessionCount() ) ;
	body.set_logical_update_ms( sGameSvr.GetLogicalTaker()->GetUpdateUseTimeMS() ) ;
	refLogin.set_region_id(sGameSvr.GetRegionId());
	refLogin.set_server_id(sGameSvr.GetServerId());
	NetPack packet( pb::SG2L_BROADCAST, header.ByteSize() + body.ByteSize() + refLogin.ByteSize() + 6 ) ;
	packet << header ;
	packet << body ;
	packet << refLogin ;
	sGameSvr.BroadcastPacketToLoginServer(packet);
#endif
}

