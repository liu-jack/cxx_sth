#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "MapLogic.pb.h"
#include "PlayerMapLogic.h"
#include "monster/MonsterCity.h"
#include "country/country.h"
#include "Login.pb.h"
#include "Country.pb.h"
#include "World.pb.h"
#include "WorldMap.h"
#include "Table/MapTableMgr.h"
#include "character/Character.h"
#include "character/CharacterStorage.h"
#ifdef _MMO_SERVER_
#include "../Story/PlayerStoryLog.h"
#endif
#include "GameServer.pb.h"
#include "Fuben.pb.h"
#include "Activity.pb.h"
#include "../reward/reward.h"
#include "CrossLogic/CountyDB.h"
#include "game_data/DbStringMgr.h"
#include "chat/ChatSpeaker.h"
#include "def/ConstDef.h"
#include "BaseDefine.pb.h"
#include "BaseDefineMgr.h"
using namespace pb;

OpHandler(CMSG_ENTER_MAP)
{
#ifdef _MMO_SERVER_
    C2GS_Enter_Map req; pack >> req;

    uint32 mapId = req.map_tid();

    if (mapId == 0)
        sWorldMap.AddToMap(*this);
    else
        m_StoryLog->EnterStory(this,mapId);
#endif
	NUll_Ret retmsg;
	Send(SMSG_NULL_RET,retmsg);
}
OpHandler(CMSG_LEAVE_MAP)
{
#ifdef _MMO_SERVER_
    C2GS_Leave_Map req; pack >> req;

    uint32 mapId = req.map_tid();

    if (mapId == 0)
        sWorldMap.DelFromMap(*this);
    else
        m_StoryLog->ExitStory(this);
#endif
	NUll_Ret retmsg;
	Send(SMSG_NULL_RET,retmsg);
}
OpHandler(CMSG_TAKE_SUPPLY_REWARD)////军资奖励
{
	pb::GS2C_Take_Army_Resource_Result msg;
    m_worldMapLogic->TakeArmyResource(*this,msg);
	Send(SMSG_TAKE_ARMY_AWARDS_RESULT, msg);
}
OpHandler(CMSG_REQUEST_SUPPLY_REWARD)
{
    GS2C_MapLogic_ArmyResource msg;
    m_worldMapLogic->SaveTo(*this,msg);
    Send(SMSG_SUPPLY_REWARD, msg);
}
OpHandler(CMSG_REQUEST_ALL_EVENT)
{
    GS2C_All_CITY_EVENT msg;
    m_worldMapLogic->SaveTo(msg);
    Send(SMSG_All_CITY_EVENT, msg);
}

//领取宝箱
OpHandler(CMSG_TAKE_EVENT_AWARD)
{
    C2GS_TAKE_EVENT_AWARD msg; pack >> msg;
	GS2C_Take_Award_Ret sendmsg;
	IntPairVec reward;
    m_worldMapLogic->ClientHandleCityEvent(*this, msg.city_id(), msg.event_id(), msg.award_index(),sendmsg,reward);
	Send(SMSG_TAKE_EVENT_AWARD_RET,sendmsg);
	sReward.Change(*this,reward);
}
//蛮族
OpHandler(CMSG_MONSTER_CITY_INFO)
{
	GS2C_Monster_City_All_Info sendmsg;
	m_worldMapLogic->SendMonsterAllInfo(*this,*sendmsg.mutable_all_info());
	sendmsg.set_cdendtime(m_worldMapLogic->m_monsterDonateTime);
	Send(pb::SMSG_MONSTER_CITY_INFO_RSP,sendmsg);
}
OpHandler(CMSG_MONSTER_CITY_DONATE)
{
    C2GS_Monster_City_Donate msg; pack >> msg;
	GS2C_Monster_City_Donate sendmsg;
	if(m_worldMapLogic->MonsterDonate(*this, msg.dest_country_id(),sendmsg))
	{
		Send(SMSG_MONSTER_CITY_DONATE,sendmsg);
		AddXP(GET_BASE_DEF_UINT(pb::BD_INVADE_WILD_GET_PLAYER_EXP));
	}
	else
	{
		Send(SMSG_MONSTER_CITY_DONATE,sendmsg);
	}   
}
OpHandler(CMSG_MONSTER_CITY_CLEAR_DONATE_CD)
{
    m_worldMapLogic->ClearMonsterDonateCD(*this);
    NetPack msgBack(SMSG_MONSTER_CITY_CLEAR_DONATE_CD);
    Send(msgBack);
}
OpHandler(CMSG_MONSTER_CITY_START_INVADE)
{
    C2GS_Monster_City_Start_Invade msg; pack >> msg;
	GS2C_Monster_City_Invade_Rsp sendmsg;
    if (Country* myCountry = m_worldMapLogic->GetWorldMapCountry())
    {
		if (Country* dstCountry = sWorldMap.GetCountry(msg.dest_country_id()))
		{
			if (myCountry->m_MonsterCity.StartInvade(*this,*dstCountry,sendmsg,myCountry->GetLevel()))
			{
				pb::C2GS_Chat chat_msg;
				char temp[1024] = {0},buf[1024] = {0},country_name[1024] = {0},my_country_name[1024] = {0};
				uint32 my_country_id = GetCountryId();
				if(my_country_id == pb::COUNTRY_GOD)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_GOD_COUNTRY_NAME,my_country_name,LANG_CN);
				}	
				else if(my_country_id == pb::COUNTRY_WORM)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_WORM_COUNTRY_NAME,my_country_name,LANG_CN);
				}
				else if(my_country_id == pb::COUNTRY_MAN)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_MAN_COUNTRY_NAME,my_country_name,LANG_CN);
				}
				if(msg.dest_country_id() == pb::COUNTRY_GOD)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_GOD_COUNTRY_NAME,country_name,LANG_CN);
				}	
				else if(msg.dest_country_id() == pb::COUNTRY_WORM)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_WORM_COUNTRY_NAME,country_name,LANG_CN);
				}
				else if(msg.dest_country_id() == pb::COUNTRY_MAN)
				{
					CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_MAN_COUNTRY_NAME,country_name,LANG_CN);
				}
				CDbStringMgr<String_Ui>::GetUtf8String(STRING_UI_MONSTER_INVADE,temp,LANG_CN);
				SPRINTF(buf,temp,my_country_name,country_name);
				chat_msg.set_type(pb::CHAT_SYSTEM);
				chat_msg.set_chat_info(buf);
				m_chatSpeaker->Say(this,chat_msg);
				CountryDB::SendBaseToDb(*myCountry);
			}
		}
    }
	Send(SMSG_MONSTER_CITY_START_INVADE,sendmsg);
}
OpHandler(CMSG_OPEN_MONSTER_SHILIAN)
{
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		country->OpenMonsterTask(*this);
	}
}
OpHandler(CMSG_LEVEL_UP_COUNTRY)
{
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		if(GetPlaceOfficeId() == 1)		//皇帝
			country->LeveUp();
	}
}
OpHandler(CMSG_TAKE_MONSTER_REWARD)
{
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		country->TakeMonsterTaskReward(*this);
	}
}
OpHandler(CMSG_MONSTER_SHILIAN_RANK_INFO)
{
	if (Country* country = sWorldMap.GetCountry(GetCountryId()))
	{
		GS2C_Country_Task_Player msg;
		country->SaveTo(msg, *this);
		Send(SMSG_MONSTER_SHILIAN_RANK_INFO, msg);
	}
}
OpHandler(CMSG_REQUEST_FOG_INFO)
{
	C2GS_REQUEST_FOG_INFO msg; pack >> msg;
	m_worldMapLogic->dealRequireFogInfo(*this, msg);
}
OpHandler(CMSG_ENTER_FOG_STAGE)
{
	C2GS_Enter_Fog msg; pack >> msg;

	const uint32 clientStageID = msg.fog_id();

	//收集武将上阵数据
	vector<uint32> heroIds;
	for (int i = 0; i < msg.hero_id_size(); ++i)
	{
		heroIds.push_back(msg.hero_id(i));
	}

	m_worldMapLogic->EnterFog(*this, clientStageID, heroIds);
}


//屯田消息
OpHandler(CMSG_START_TUNTIAN)
{
	C2GS_StartTunTian msg; pack >> msg;
	GS2C_StartTunTian sendmsg;
	sendmsg.set_istuntiansuc(false);

	const MapCity* table = sMapTableMgr.GetMapCity(msg.cityid());
	if(table)
	{
		if(table->IsTonden())
		{
			Character* pHero = m_characterStorage->MutableCharacter(msg.heroid());
			if(pHero)
			{
				if(m_worldMapLogic->TondenBegin(*this, *pHero))
				{
					sendmsg.set_istuntiansuc(true);
					sendmsg.set_starttime(sOS.TimeSeconds());
					sendmsg.set_heroid(msg.heroid());
				}
			}
		}
	}
	Send(SMSG_START_TUNTIAN,sendmsg);
}

//屯田区捐赠
OpHandler(CMSG_DONATE_CITY)
{
	GS2C_TunTian_Donate_Ret retMsg;
	m_worldMapLogic->TondenDonateMoney(*this,retMsg);
	Send(SMSG_TUNTIAN_DONATE_RET, retMsg);
}

//清理屯田cd或获得屯田信息
OpHandler(CMSG_TUNTIAN_CLEAR)
{
	C2GS_Clear_Fresh_TunTian msg; pack >> msg;

	GS2C_Clear_Tuntian_CD_Ret retMsg;
	m_worldMapLogic->ClearCDorFresh(*this,msg.isclear(),retMsg);
	Send(SMSG_TUNTIAN_CLEAR,retMsg);

}

//开始练兵
OpHandler(CMSG_TRAIN_SOLDIER)
{
	C2GS_Train_Soldier msg; pack >> msg;
	GS2C_Train_Info sendMsg;

	m_worldMapLogic->FillTrainInfo(*this,sendMsg,msg.train_index());
	Send(SMSG_TRAIN_RET,sendMsg);
}

//结束练兵
OpHandler(CMSG_TRAIN_END)
{

	GS2C_Train_Info sendMsg;

	m_worldMapLogic->TrainFinish(this,sendMsg);
	m_worldMapLogic->FillTrainInfo(*this,sendMsg,0);
	Send(SMSG_TRAIN_RET,sendMsg);
}

//立即完成练兵
OpHandler(CMSG_TRAIN_INSTANT)
{
	GS2C_Train_Info sendMsg;

	m_worldMapLogic->TrainInstant(this,sendMsg);
	m_worldMapLogic->FillTrainInfo(*this,sendMsg,0);
	Send(SMSG_TRAIN_RET,sendMsg);
}

//揭开迷雾
OpHandler(CMSG_TAKE_FOG_REWARD)
{
	C2GS_Fog_Reward cmsg;
	pack >> cmsg;
	if(LIKELY(IsOpenFogIsActive()))
	{
		GS2C_Fog_Reward msg;
		IntPair pair_reward;
		if(LIKELY(IsTakeOpenFogReward(cmsg.id())))
		{
			TakeOpenFogReward(cmsg.id());
			if(LIKELY(m_worldMapLogic->TakeFogReward(cmsg.id(),&pair_reward)))
			{
				pb::StReward* reward = msg.mutable_rewards();
				reward->set_type(pair_reward.first);
				reward->set_value(pair_reward.second);
				msg.set_result(pb::RR_T_RESULT_SUCCESS);
				sReward.Change(*this,pair_reward);
				msg.set_id(cmsg.id());
				Send(SMSG_TAKE_FOG_REWARD_RSP,msg);
				return;
			}
			else
			{
				msg.set_result(pb::RR_T_RESULT_OTHER_REASON);
				Send(SMSG_TAKE_FOG_REWARD_RSP,msg);
				return;
			}
		}
		else
		{
			msg.set_result(pb::RR_T_RESULT_HAS_TAKE);
			Send(SMSG_TAKE_FOG_REWARD_RSP,msg);
			return;
		}
	}
}

OpHandler(CMSG_OPEN_FOG_INFO)
{

}

OpHandler(CMSG_CELEBRITY_TAKE_REWARD_OR_USE_ITEM)
{
	C2GS_UseCurrencyToTakeReward msg; pack >> msg;
	GS2C_UseCurrencyToTakeRewardRsp sendmsg;
	IntPairVec reward;
	m_worldMapLogic->ClientHandleCelebrityCityEvent(this, msg.city_id(), msg.event_id(), msg.award_idx(),sendmsg,reward);
	Send(SMSG_CELEBRITY_TAKE_REWARD_OR_USE_ITEM_RSP,sendmsg);
	sReward.Change(*this,reward);
}