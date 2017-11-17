#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Combat.pb.h"
#include "combat_define.h"
#include "CombatGroup.h"
#include "CombatObj.h"
#include "../character/CharacterStorage.h"
#include "../item/ItemArray.h"
#include "../reward/reward.h"
#ifdef _MMO_SERVER_
#include "../fuben/FubenLog.h"
#include "Palace.pb.h"
#include "../Palace/PalaceMgr.h"
#endif
#include "Technology/PlayerTechnology.h"
#include "GameServer.pb.h"
#include "Activity/Personal/PersonalTask.h"
#include "quest/QuestLogger.h"
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossDestroy/CrossDestroy.h"
#endif

using namespace pb;
using namespace Combat;

OpHandler(CMSG_COMBAT_BEGIN)	//准备	（观看）
{
    C2GS_Combat_Begin req; pack >> req;

	//printf("GS2C_Combat_Begin(%d) -- \n", req.group_id());

    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))	//找到战斗组
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(group->m_isCombatEnd)	//战斗结束了，屏蔽所有消息
		{
			NLOG("### COMBAT IS END ###");

			GS2C_Combat_Try sendMsg;
			sendMsg.set_result(false);
			sendMsg.set_combat_type(group->m_eCombat);
			Send(SMSG_COMBAT_TRY,sendMsg);

			return;
		}

		//观看不会发送攻击队列
		if(req.attack_lst_size()>0 && !group->m_isReady)		
		{
			NLOG("start combat");
			group->StartCombatAndChange(req);//开始准备阶段有可能改变武将出站顺序，如果中途进来不会改变
			//因为npc可能是空血或者玩家武将是空血，需要先删除空血的武将
			group->ReleaseHeros();

			if(group->m_group_attack.size() == 0 || group->m_group_defence.size() == 0)
			{
				//ASSERT(0);
				GS2C_Combat_Try sendMsg;
				sendMsg.set_result(false);
				sendMsg.set_combat_type(group->m_eCombat);
				Send(SMSG_COMBAT_TRY,sendMsg);

				return;
			}

			group->UpdateHerosInCombat();	//参战武将的状态，前6排isFighting,后面isInCombat  计算所有武将的技能释放次数
			group->m_isReady = true;
			if(group->m_eCombat == Combat::CT_Fuben)
				group->m_FubenPlayer = GetGuid();
		}
		else
		{
			NLOG("watch combat");
			if(group->m_isReady == false) 
			{
				NLOG("### IMPORTANT BUG ###");
				return;		// 不允许未开战的时候观战
			}
		}

		group->Watch(GetGuid());	//添加观看玩家列表，如果已经存在，不处理

		//战斗信息
        GS2C_Combat_Begin msg;
        group->SaveTo(msg);
		msg.set_isautocombat(IsAutoCombat());
        Send(SMSG_COMBAT_BEGIN, msg);
		//printf("GS2C_Combat_Begin(%d) -- attack soldiers:%d  defence soldiers:%d \n", req.group_id(), (int)group->m_group_attack.front()->soldiers.size(), (int)group->m_group_defence.front()->soldiers.size());
    
		//观战会将玩家或者npc选择的战术发过去
		if(req.attack_lst_size() == 0)
		{
			GS2C_Combat_Select_Tactic_Single attmsg;
			GS2C_Combat_Select_Tactic_Single defmsg;
			int count = 0;
			if(!group->SaveTo(attmsg,defmsg,count))
				return;
			
			attmsg.set_group_id(req.group_id());
			defmsg.set_group_id(req.group_id());

			if(count == 1 || count == 3)
				Send(SMSG_COMBAT_SELECT_TACTIC_SINGLE,attmsg);

			if(count == 2 || count == 3)
				Send(SMSG_COMBAT_SELECT_TACTIC_SINGLE,defmsg);
		}
	}
	else
	{
		GS2C_Combat_Try sendMsg;
		sendMsg.set_result(false);
		//sendMsg.set_combat_type(group->m_eCombat);
		Send(SMSG_COMBAT_TRY,sendMsg);
	}
}

OpHandler(CMSG_COMBAT_REQUEST_NEXT_LST)
{
	C2GS_Combat_Next_Lst req; pack >> req;

	LLOG("CMSG_COMBAT_REQUEST_NEXT_LST index = %d,isatt = %d",req.indexincombat(),req.is_attacker());

	GS2C_Combat_Next_Lst msg;
	if(CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		if( !group->m_isReady || group->m_isCombatEnd || group->m_isQuickCombat ) 
		{
			NLOG("### IMPORTANT BUG ###");
			return;		// 不允许未开战的时候观战
		}

		group->SaveTo(msg, req.indexincombat(), req.is_attacker());
		msg.set_group_id(req.group_id());
		Send(SMSG_COMBAT_REQUEST_NEXT_LST, msg);
	}
}

OpHandler(CMSG_COMBAT_ANI_FINISHED)
{
	C2GS_Combat_Ani_Finished req; pack >> req;

	NLOG("########## get message CMSG_COMBAT_ANI_FINISHED #########");
	
	if(CombatGroup* group = CombatGroup::FindGroup(req.groupid()))
	{
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(req.roundcount() == group->m_CurrentRound && !group->m_isCombatEnd)
			group->AniFinished();
	}
	else
		NLOG("#### not have group  groupid = %d ####",req.groupid());


}

OpHandler(CMSG_COMBAT_SELECT_TACTIC)		//选择技能战术
{
    C2GS_Combat_Select_Tactic req; pack >> req;

	LLOG("###########get a message  CMSG_COMBAT_SELECT_TACTIC #########");
    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		LLOG("{%d} rev count=%lld, cur count=%lld",group->m_unique_id,req.roundcount(),group->m_CurrentRound);
		if(req.roundcount() == group->m_CurrentRound && !group->m_isCombatEnd)
		{
			if (Combat::CombatObj* obj = group->FindObj(GetGuid(), req.hero_id(), req.is_attacker()))
			{
				LLOG("{%d}#### handle select #### type=%d",group->m_unique_id,req.type());
				if(obj->isTacticLegal((TacticEnum)req.type()))
					obj->SelectTactic((TacticEnum)req.type());
				else
				{
					obj->SelectTactic(obj->RandEnableTactic());
					LLOG("tactic err orgTac=%d, realTac=%d",req.type(),obj->tactic);
				}
				pb::GS2C_Combat_Select_Tactic_Single msg;
				msg.set_type((pb::Msg_ECombatTactic)obj->tactic);
				msg.set_is_attacker(req.is_attacker());
				msg.set_attackernum(group->m_group_attack.size());
				msg.set_defendernum(group->m_group_defence.size());
				msg.set_roundcount(group->m_CurrentRound);
				msg.set_group_id(req.group_id());
				if(!group->m_isQuickCombat)
					group->Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
			}
			else
			{
				LLOG("#### not handle select ####");
			}
		}
		else
		{
			LLOG("#### round count not match ####");
		}
    }
	else
	{
		LLOG("#### not have group  groupid = %d ####",req.group_id());
	}
}
OpHandler(CMSG_COMBAT_UNWATCH)
{
    C2GS_Combat_Unwatch req; pack >> req;

#ifdef _MMO_SERVER_
	if(req.has_is_in_cross())
	{
		if(req.is_in_cross())
			SendCrossServerNetPack(pack);
	}
#endif

    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
        group->UnWatch(GetGuid());
    }

	NUll_Ret retmsg;
	Send(SMSG_NULL_RET,retmsg);

	//SetInCrossWar(false);
	//NUll_Ret retmsg;
	//Send(SMSG_NULL_RET,retmsg);

}

//诱敌
OpHandler(CMSG_COMBAT_ATTRACT_ENEMY)
{
	C2GS_Combat_Attract_Enemy req; pack >> req;
	pb::GS2C_Combat_Attract_Enemy msg;
    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(group->m_fight_calc)
			msg.set_is_success(false);
		else
			group->AttractEnemy(*this, req.is_attacker(),msg);
    }
	else
		msg.set_is_success(false);
	msg.set_group_id(req.group_id());
	Send(pb::SMSG_COMBAT_ATTRACT_ENEMY, msg);
	if(msg.is_success())
	{
		UpdateAllDailyQuestInfo(ATTACK_ENEMY,1,0);//每日任务 
	}
}

//突击
OpHandler(CMSG_COMBAT_RUSH)
{
    C2GS_Combat_Rush req; pack >> req;

	pb::GS2C_Rush_Result msg;

    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(group->m_fight_calc)	//在计算中
		{
			msg.set_result(pb::SERVER_BUSY);
		}
		else
		{
			if (Character* pHero = m_characterStorage->MutableCharacter(req.hero_id()))
			{
				group->Rush(*pHero, req.city_id(), req.is_attacker(),msg);
			}
			else
				msg.set_result(pb::HERO_ERR);
		}
    }
	else
		msg.set_result(pb::GROUP_ID_ERR);
	msg.set_group_id(req.group_id());
	Send(SMSG_COMBAT_RUSH,msg);
	if(msg.result() == pb::SUCCESS)
	{
		UpdateAllDailyQuestInfo(TUJING,1,0);//每日任务  突进
	}
}

//撤退
OpHandler(CMSG_COMBAT_RUNAWAY)
{
    C2GS_Combat_Runaway req; pack >> req;

	pb::GS2C_Combat_Runaway msg;
    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(group->m_fight_calc)	//在计算中
		{
			msg.set_result(pb::SERVER_BUSY);
		}
		else
		{
			if (Character* pHero = m_characterStorage->MutableCharacter(req.hero_id()))
			{
				group->RunAway(*this, *pHero, req.city_id(), req.is_attacker(),msg);
			}
			else
				msg.set_result(pb::HERO_ERR);
		}
	}
	else
		msg.set_result(pb::GROUP_ID_ERR);
	msg.set_group_id(req.group_id());
	Send(SMSG_COMBAT_RUNAWAY,msg);
}

//增兵
OpHandler(CMSG_COMBAT_ADD_SOLDIER)
{
    C2GS_Combat_Add_Soldier req; pack >> req;

	pb::GS2C_Combat_Add_Soldier msg;

    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		//5000800 免费增兵次数 5000100 系统赠送金币
		int tmpnum = GetCurrency(eTroopsNum);
		int coinnum = GetCurrency(eSysGold)/20;
		int silvernum = GetCurrency(eCoin);
		uint32 addexp = 0;
		uint32 sumexp = 0;
		int toAddShaders = (int)req.times();
		if((tmpnum + coinnum) >= toAddShaders)
		{
			for (int i = 0; i < toAddShaders; ++i)
			{
				bool addret = group->AddSoldier(*this, req.is_attacker(), req.hero_id(), addexp, req.curcityid());
				sumexp += addexp;
				if(addret)
					sPersonTask.addProgress(ADD_SOLIDER_IN_COUNTRY,1,*this);
#ifdef _SERVER_CROSS_
				sCrossDestroy.addProgress(ADD_SOLIDER_IN_COUNTRY,1,*this);
#endif
			}

			if (toAddShaders > 0)
			{
				AddEventLog( pb::EVENT_PLAYER_CALL_PHANTOM,
					2,
					GetLevel(),
					req.curcityid(),
					toAddShaders
					);
			}
	
			msg.set_addexp(sumexp);
			msg.set_result(pb::SUCCESS);
		}
		else
			msg.set_result(pb::NO_ENOUGH_COSTS);
    }
	else
		msg.set_result(pb::GROUP_ID_ERR);
	msg.set_group_id(req.group_id());
	Send(SMSG_COMBAT_ADD_SOLDIER,msg);
	if(msg.result() == pb::SUCCESS)
	{
		UpdateAllDailyQuestInfo(ADD_SOLIDER,1,0);//每日任务
	}
}

//征召
OpHandler(CMSG_COMBAT_CALL_UP)
{
	if(!HaveTechnology(GUOZHANORDER)) return;

    C2GS_Combat_Call_Up req; pack >> req;
	GS2C_Combat_Call_Up msg;

    CombatGroup::CallUp(*this, req.city_id(), msg);

	Send(SMSG_COMBAT_CALL_UP,msg);
}

OpHandler(CMSG_COMBAT_CALL_REQUEST)
{
	C2GS_Combat_Call_Request req; pack >> req;
	GS2C_Combat_Call_Request msg;

	CombatGroup::CallRequest(*this,req,req.destcityid(),msg);

	Send(SMSG_COMBAT_CALL_REQUEST,msg);
}

//加速
OpHandler(CMSG_COMBAT_SPEED_UP)
{
    C2GS_Combat_Speed_Up req; pack >> req;

    if (CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
    {
		if(group->m_isQuickCombat) return;	//快速结束战斗中

        if (group->SpeedUp(*this, req.city_id()))
        {
            NetPack pack(SMSG_COMBAT_SPEED_UP);
            Send(pack);
        }
    }
}

//远征添加幻影
OpHandler(CMSG_COMBAT_ADD_HERO)
{
#ifdef _MMO_SERVER_
	C2GS_Combat_Crusade_Add_Hero req; pack >> req;

	GS2C_Combat_Crusade_Add_Hero retmsg;

	if(CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		if(!group->m_isReady)
		{
			if(m_fubenLog->AddShadowInCrusade(*this,req.hero_id(),retmsg))
			{
				group->AddHeros(*this,req.hero_id());
				Send(SMSG_COMBAT_ADD_HERO,retmsg);
			}
		}
	}
#endif
}
//远征回复召幻影次数
OpHandler(CMSG_CRUSADE_BACK)
{
#ifdef _MMO_SERVER_
	pb::C2GS_Combat_Crusade_Recover_Times req;pack >> req;
	m_fubenLog->RecoverTimes(*this,req);
	NUll_Ret retmsg;
	Send(SMSG_NULL_RET,retmsg);
#endif
}


OpHandler(CMSG_COMBAT_TRY)
{
#ifdef _MMO_SERVER_
	C2GS_Combat_Try req; pack >> req;

	GS2C_Combat_Try sendMsg;
	bool ret = false;;
	int errcode = 0;

	switch(req.combat_type())
	{
	case Combat::CT_Country:
		if(CombatGroup* group = CombatGroup::FindGroup(req.assist_arg()))
		{
			if(group->m_eCombat == Combat::CT_Country)
			{
				if(!group->m_isCombatEnd)
					ret = true;
			}
		}
		break;
	case Combat::CT_Fuben:
		if(CombatGroup* group = CombatGroup::FindGroup(req.assist_arg()))
		{
			if(group->m_eCombat == Combat::CT_Fuben)
			{
				if(!group->m_isCombatEnd)
					ret = true;
			}
		}
		break;
	case Combat::CT_Fog:
		ret = true;
		break;
	case Combat::CT_Ore:
		ret = true;
		break;
	case Combat::CT_EventStory:
		ret = true;
		break;
	case Combat::CT_Crusade:
		ret = true;
		break;
	case Combat::CT_Official:
		ret = sPalaceMgr(GetCountryId())->IsOfficalCanFight(*this,req.assist_arg(),errcode);
		break;
	case Combat::CT_Monster:
		ret = true;
		break;
	case Combat::CT_Activity_Tournament:
		ret = true;
		break;
	}
	sendMsg.set_result((uint32)ret);
	sendMsg.set_combat_type(req.combat_type());
	sendMsg.set_assist_arg(req.assist_arg());
	sendMsg.set_errcode((pb::Msg_return_common)errcode);
	Send(SMSG_COMBAT_TRY,sendMsg);
#endif
}

OpHandler(CMSG_AUTO_COMBAT)
{
	if(!HaveTechnology(AUTOMITIC_TATIC)) return;
	if(!m_questLogger->GetQuestFinished(GET_BASE_DEF_INT( pb::BD_TEACH_AUTOFIGHT_LIMITQUEST)))
		return;

	C2GS_Auto_Combat req; pack >> req;

	SetIsAutoCombat(req.useauto());

	GS2C_Auto_Combat sendMsg;
	sendMsg.set_useauto(req.useauto());
	sendMsg.set_issetsuccess(true);
	Send(SMSG_AUTO_COMBAT, sendMsg);
}

OpHandler(CMSG_QUICK_COMBAT)
{
	C2GS_Quick_Combat req; pack >> req;

	LLOG("{%d} quick combat",req.group_id());

	GS2C_Quick_Combat sendmsg;
	sendmsg.set_ret(false);
	sendmsg.set_group_id(req.group_id());
	if(CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		LLOG("1");
		if(group->m_isQuickCombat) return;	//快速结束战斗中

		LLOG("2");

		if(!group->m_isCombatEnd)
		{
			LLOG("3");
			if(group->m_eCombat == Combat::CT_Fuben || group->m_eCombat == Combat::CT_Fog || group->m_eCombat == CT_Crusade)	//pve才允許
			{
				LLOG("4");
				int usetimes = GetAttrInt(pb::PLAYER_FIELD_USE_FREE_QUICK_COMBAT_TIMES);

				if(usetimes < (int)GET_BASE_DEF_UINT(pb::BD_FREE_QUICK_FIGHT_TIMES))	//免费次数使用免费次数
				{
					LLOG("5");
					SetValueInt(pb::PLAYER_FIELD_USE_FREE_QUICK_COMBAT_TIMES, usetimes+1);

					sendmsg.set_ret(true);

					if(group->m_leftFlashSec)
						group->AniFinished();
					group->m_leftOperateSec = 0;
					group->m_combatEndLast = 0;
					group->m_isQuickCombat = true;
				}
				else if(EnoughCurrency(eSysGold,GET_BASE_DEF_INT(pb::BD_QUICK_FIGHT_COST_DIAMOND)))
				{
					LLOG("6");
					TryDeductCurrency(IR_QUICK_FIGHT_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_QUICK_FIGHT_COST_DIAMOND));
					
					sendmsg.set_ret(true);

					if(group->m_leftFlashSec)
						group->AniFinished();
					group->m_leftOperateSec = 0;
					group->m_combatEndLast = 0;
					group->m_isQuickCombat = true;
				}
				else
					sendmsg.set_ret(false);
			}
		}

		Send(SMSG_QUICK_COMBAT, sendmsg);

	}
	else
		Send(SMSG_QUICK_COMBAT, sendmsg);

}

OpHandler(CMSG_GET_HERO_STATUS)
{
	C2GS_Combat_Hero_Status req; pack >> req;

	Character* charone = m_characterStorage->MutableCharacter(req.hero_tid());
	if(charone)
	{
		pb::GS2C_Combat_Hero_Status msg;
		msg.set_hero_tid(charone->GetID());
		msg.set_is_in_combat(charone->isInCombat);
		msg.set_is_fighting(charone->isFighting);
		if(charone->isInCombat)
		{
			if(CombatGroup* group = CombatGroup::FindGroup(charone->nowCombatGroupID))
			{
				msg.set_group_id(charone->nowCombatGroupID);
				msg.set_combat_type(group->m_eCombat);
				msg.set_is_in_attract(group->m_isAttractCombat);
			}
		}
		msg.set_cityid(charone->GetCurCity());
		msg.set_isfakechar(charone->IsFakeChar());
		Send(SMSG_SEND_HERO_STATUS,msg);
	}
	else
	{
		if(Character* charone = m_characterStorage->MutableFakeCharacter(req.hero_tid()))
		{
			pb::GS2C_Combat_Hero_Status msg;
			msg.set_hero_tid(charone->GetID());
			msg.set_is_in_combat(charone->isInCombat);
			msg.set_is_fighting(charone->isFighting);
			if(charone->isInCombat)
			{
				if(CombatGroup* group = CombatGroup::FindGroup(charone->nowCombatGroupID))
				{
					msg.set_group_id(charone->nowCombatGroupID);
					msg.set_combat_type(group->m_eCombat);
					msg.set_is_in_attract(group->m_isAttractCombat);
				}
			}
			msg.set_cityid(charone->GetCurCity());
			msg.set_isfakechar(charone->IsFakeChar());
			Send(SMSG_SEND_HERO_STATUS,msg);
		}
	}
}

OpHandler(CMSG_COMBAT_REQUEST_TERR)
{
	C2GS_REQUEST_TERR req; pack >> req;
	GS2C_REQUEST_TERR_RET msg;
	if(CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		msg.set_isgroupexist(1);
		msg.set_terraintyp(group->m_terrainType);
	}
	else
		msg.set_isgroupexist(0);
	Send(SMSG_COMBAT_REQUEST_TERR_RET,msg);
}

OpHandler(CMSG_GET_MY_SHADOW_NUM)
{
	C2GS_GetMyShadowNum req; pack >> req;
	GS2C_GetMyShadowNum msg;
	uint32 count = 0;
	msg.set_shadownum(0);
	if(CombatGroup* group = CombatGroup::FindGroup(req.group_id()))
	{
		if(req.isattack())
		{
			msg.set_shadownum(group->GetShadowNum(true,GetGuid()));
		}
		else
		{
			msg.set_shadownum(group->GetShadowNum(false,GetGuid()));
		}
	}
	Send(SMSG_GET_MY_SHADOW_NUM,msg);
}