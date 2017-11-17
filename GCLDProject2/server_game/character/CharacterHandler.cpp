#include "object/Player.h"

#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "def/ObjectDefines.h"
#include "def/TypeDef.h"
#include "def/MmoAssert.h"

#include "utility/Utility.h"
#include "DbTool.h"

#include "CharacterStorage.h"

#include "CrossLogic/ItemCharacterLogic.h"
#include "CrossLogic/PlayerCharacterLogic.h"


#include "Loot/LootList.h" 

#include "Opcode.pb.h"
#include "Character.pb.h"
#include "World.pb.h"
#include "Enum.pb.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "Combat.pb.h"
#include "ServerOpcode.pb.h"
#include "GameServer.pb.h"
#ifdef _MMO_SERVER_
#include "fuben/FubenLog.h"
#endif
using namespace pb;



namespace PlayerHandle
{
    void SendOpcodeWithErrCode( Player* player, int opcode, pb::CxGS_ERROR_CODE err_code)
    {
        NetPack packet( opcode, sizeof(pb::CxGS_ERROR_CODE) + 2);
        packet << err_code;
        player->Send(packet);
    }
}  

OpHandler( CMSG_ALL_CHARACTER_INFO)
{
    if ( !m_characterStorage.get())
        return;

    GS2C_CharacterStorage msg ;

    m_characterStorage->SaveTo(msg);
    Send( SMSG_ALL_CHARACTER_INFO, msg);
}

OpHandler( CMSG_SET_BATTLE_CHARACTER)
{
	if(IsCanCrossWarState())
	{
		if(IsRegistCrossServer())	//如果是国战并且已经在国战中注册了，则向国战确认
		{
			GS2C_ChangeBattleCharacter askNullMsg;
			pack >> askNullMsg;
			SendCrossServerMsg(pb::SG2R_Check_Can_Change_Hero,askNullMsg,PACKET_TYPE_SERVER_GAME,GetGuid());

			NUll_Ret retmsg;
			Send(SMSG_NULL_RET,retmsg);
			return;
		}
	}

    if ( !m_characterStorage.get())
        return;
	const TecTable* tec = NULL;
    GS2C_ChangeBattleCharacter msg;
    pack >> msg;

	Character* forCharctor = m_characterStorage->MutableCharacter(msg.card_id());
	if (msg.is_zhaomu())
	{
		if (forCharctor)
		{
			bool isEnoughtCoin = false;
			int costZhaomu = forCharctor->GetZhaoMuNeedCost();
			if (EnoughCurrency(eCoin,costZhaomu))
			{
				isEnoughtCoin = TryDeductCurrency(IR_PLAYER_ZHAOMU_CHARACTER_COST,eCoin,costZhaomu);
			}

			if (!isEnoughtCoin)
			{
				SendErrorCode(ErrNotEnoughCoin);
				return;
			}
		}
		else
		{
			SendErrorCode(ErrNotUnlock);
			return;
		}
	}

    std::vector< CharacterId> characterGuids;
    MsgTool::SaveMsgToVec( msg.battle_array(), characterGuids);

    ItemCharacterLogic::SetBattleArray( this, characterGuids);
#ifdef _MMO_SERVER_
	m_fubenLog->AddHeroToVector(characterGuids);
#endif
}

OpHandler(CMSG_GET_BATTLE_CHARACTER)
{
	GS2C_BattleCharacter msg;
	m_characterStorage->SaveBattleArrayToMsg( msg);
	Send(SMSG_SET_BATTLE_CHARACTER, msg);
}

OpHandler( CMSG_USE_UPGRADE_LV_ITEM_TO_CHARACTER)
{
    C2GS_UseItemsToTarget msg; 
    pack >> msg;

    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::UseUpgradeLvItemToCharacter(this, msg);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_USE_UPGRADE_LV_ITEM_TO_CHARACTER_RESP, err_code);
}

OpHandler( CMSG_UPGRADE_CHARACTER_QUALITY)
{
    /*C2GS_UpgradeQuality msg;
    pack >> msg;

    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::UpgradeCharacterQuality( this, msg);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_UPGRADE_CHARACTER_QUALITY_RESP, err_code);*/
}


OpHandler( CMSG_INLAY_RUNE_TO_CHARACTER)
{
    uint32 charId = 0, slot = 0;
    pack >> charId >> slot;

    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::InlayRuneToCharacter( this, charId, slot);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_INSAY_RUNE_TO_CHARACTER_RESP, err_code);

}

OpHandler( CMSG_UPGRADE_CHARACTER_REINFORCE)
{
    uint32 charId = 0;
    pack >> charId;

    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::UpgradeCharacterReinforce( this, charId);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_UPGRADE_CHARACTER_REINFORCE_RESP, err_code);
}


OpHandler( CMSG_REBIRTH_CHAR)
{
    uint32 charId = 0;
    pack >> charId;
                                                                                          
    LootList lootList;
    pb::GS2C_LootList msg;
    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::RebirthCharacter( this, charId, lootList);
    if ( lootList.PickAllMoneyAndItems(this, msg, pb::IR_CYCLE_GET))
    {
        Send( pb::SMSG_ITEM_GET_LIKE_LOOT, msg);
    }

    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_REBIRTH_CHAR_RESP, err_code);

}

OpHandler( CMSG_DECOMPOSE_CHAR)
{
    pb::Uint32List charIdList;
    pack >> charIdList;

    LootList lootList;
    pb::GS2C_LootList msg;

    pb::CxGS_ERROR_CODE last_err = ErrCommonSuccess;
    for ( int i = 0; i < charIdList.values_size(); ++i)
    {
        pb::CxGS_ERROR_CODE err = (CxGS_ERROR_CODE)PlayerCharacterLogic::DecomposeCharacter(this, charIdList.values(i), lootList);
        if ( err != ErrCommonSuccess)
        {
            last_err = err;
        }
    }
    if ( lootList.PickAllMoneyAndItems(this, msg, pb::IR_CYCLE_GET))
    {
        Send( pb::SMSG_ITEM_GET_LIKE_LOOT, msg);
    }
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_DECOMPOSE_CHAR_RESP, last_err);
}


OpHandler( CMSG_TRAINING_CHARACTER)
{
   /* uint32 charId = 0, randomType = 0, addtion = 0;
    pack >> charId >> randomType >> addtion;


    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::TrainingCharacter( this, charId, randomType, addtion);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_TRAINING_CHARACTER_RESP, err_code);*/
}

OpHandler( CMSG_TRAINING_CHARACTER_CONFIRM)
{
	uint32 charId = 0;
	pack >> charId;

	CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::ConfirmTrainingCharacter( this, charId);

	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_TRAINING_CHARACTER_CONFIRM_RESP, err_code);
}

OpHandler( CMSG_UPGRADE_CHARACTER_SKILL)
{
    uint32 charId = 0, skillIndex = 0;
    pack >> charId >> skillIndex;

    CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::UpgradeCharacterSkill( this, charId, skillIndex);
    PlayerHandle::SendOpcodeWithErrCode(this, SMSG_UPGRADE_CHARACTER_SKILL_RESP, err_code);
}
OpHandler(CMSG_HERO_MOVE)
{
	LLOG("[1002] CMSG_HERO_MOVE");
    C2GS_Hero_Move msg; pack >> msg;
	//GS2C_Hero_Move sendmsg;

    if (Character* hero = m_characterStorage->MutableCharacter(msg.char_id()))
    {
		if(hero->isInPrison()) return;		//地牢中不能移動
        hero->MoveTo(msg.city_id());
		//Send(pb::SMSG_HERO_MOVE);

		//NUll_Ret retmsg;
		//Send(SMSG_NULL_RET,retmsg);
    }
}

OpHandler(CMSG_STOP_HERO_MOVE)
{
	C2GS_HERO_STOP_MOVE msg; pack >> msg;

	if(Character* hero = m_characterStorage->MutableCharacter(msg.char_id()))
	{
		hero->initPath();
	}

	NUll_Ret retmsg;
	Send(SMSG_NULL_RET,retmsg);
}

OpHandler(CMSG_CONSCRIPTION)
{
    uint32 charId = 0;
	CxGS_ERROR_CODE err_code = ErrCommonFail;
	CxGS_ERROR_CODE tmp_err_code = ErrCommonFail;
    //pack >> charId;

	C2GS_Onekey_Recover_HP msg; pack >> msg;
	uint32 charsize = msg.heroid_size();
	for(uint32 i=0; i < charsize; i++)
	{
		tmp_err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::OnKeyRecoverHp( this, msg.heroid(i));
		if(tmp_err_code == ErrCommonSuccess)
			err_code = ErrCommonSuccess;
	}
	if (err_code != ErrCommonSuccess)
		err_code = tmp_err_code;

	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_CONSCRIPTION_REP, err_code);
}

OpHandler( CMSG_BUY_HEADICON)
{
	uint32 iconId = 0;
	pack >> iconId;

	GS2C_PlayerStringGroup UpdateMsg;
	CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::BuyPlayerHeadIcon( this, iconId,UpdateMsg);
	if ( err_code != ErrCommonSuccess)
	{
		PlayerHandle::SendOpcodeWithErrCode(this, SMSG_BUY_HEADICON_RSP, err_code);
	}
	else
	{
		Send( pb::SMSG_UPDATE_PLAYER_ATTACH_INFO, UpdateMsg);
	}
	
}

OpHandler( CMSG_CHECK_PLAYER_ATTACH_INFO)
{
	PlayerCharacterLogic::CheckPlayerNewFreeUnlockInfo( this);
}

OpHandler( CMSG_SWITCH_HEADICON)
{
	uint32 iconId = 0;
	pack >> iconId;

	CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)PlayerCharacterLogic::SwitchPlayerHeadIcon( this, iconId);
	if ( err_code != ErrCommonSuccess)
	{
		PlayerHandle::SendOpcodeWithErrCode(this, SMSG_BUY_HEADICON_RSP, err_code);
	}
}

OpHandler( CMSG_QUIK_EXCHANGE_CHARACTER_EQUIPS)
{
	uint32 charId = 0; 
	uint32 toCharId = 0; 
	pack >> charId >> toCharId;

	CxGS_ERROR_CODE err_code = (CxGS_ERROR_CODE)ItemCharacterLogic::QuikExchangeCharacterEquips( this, charId,toCharId);

	PlayerHandle::SendOpcodeWithErrCode(this, SMSG_QUIK_EXCHANGE_CHARACTER_EQUIPS, err_code);
}

// OpHandler(CMSG_AUTO_ATTACK)//自动国战
// {
// 	//C2GS_Request_Auto_Attack req;pack >> req;
// 	
// }
// 
// OpHandler(CMSG_DONATE_CITY)//屯田区捐赠
// {
// }
// OpHandler(CMSG_TRAIN_SOLDIER)//练兵
// {
// }
// OpHandler(CMSG_BRIBE_CITY) //收买蛮族城市
// {}
// 
// OpHandler(CMSG_INCITE_CITY)//发动蛮族城市
// {
// }


OpHandler(CMSG_REQUEST_DINNER)//宴会请求
{

	pb::GS2C_Dinner_Resp msg;
	const CharacterIds & Ids = m_characterStorage->GetBattleArray();
	std::vector<CharacterId>::const_iterator Iter = Ids.begin();
	if(GetAttrUint(pb::PLAYER_FIELD_DINNER_NUMBER) == 0)
	{
		msg.set_ret(1);//次数为0//不成功
	}
	else
	{
		msg.set_ret(0);//成功
		DescreaseDinnerNum(-1);//次数减一//消耗
		for(;Iter != Ids.end() ;++Iter)
		{
			Character* character = m_characterStorage->MutableCharacter(*Iter);
			if(character)
			{
				if(!character->IsFullHp() && character->CharIsIdle())
				{
					if(character->RecoverFullSoldier())//回复
					{
						m_characterStorage->SetModifiedID(character->GetID());
					}
				}
			}
		}
		UpdateAllDailyQuestInfo(DINNER_CHARACTER,1,0);//每日任务
	}
	Send(pb::SMSG_REQUEST_DINNER_RESP,msg);
}

OpHandler(CMSG_REQUEST_DINNER_DATA)//请求宴会数据
{
	pb::GS2C_Dinner_Data msg;
	uint32 dinner_number = GetAttrUint(pb::PLAYER_FIELD_DINNER_NUMBER);
	msg.set_dinner_num(dinner_number);
	int time_now = sOS.TimeHour();
	int next_time = 0;
	switch(time_now){
		case 0:
		case 1:
		case 2:
		case 3:
			next_time = 4;
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			next_time  = 8;
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			next_time = 12;
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			next_time = 16;
			break;
		case 16:
		case 17:
		case 18:
		case 19:
			next_time = 20;
			break;
		case 20:
		case 21:
		case 22:
		case 23:
			next_time = 0;
			break;
		default:
			break;
			
	}
	msg.set_next_time(next_time);
	Send(pb::SMSG_REQUEST_DINNER_DATA_RESP,msg);
}

OpHandler(CMSG_PLAYER_VALUE_UPDATE)
{
	pb::ObjectFullValue sendmsg;
	SaveValue(sendmsg);
	Send(pb::SMSG_PLAYER_VALUE_UPDATE_RET,sendmsg);
}

OpHandler(CMSG_CHARACTER_UPDATE)
{
	const CharacterIds & Ids = m_characterStorage->GetBattleArray();
	std::vector<CharacterId>::const_iterator Iter = Ids.begin();

	for(;Iter != Ids.end() ;++Iter)
	{
		int id = *Iter;
		Character* charact = m_characterStorage->MutableCharacter(id);            
		if (charact)
		{
			pb::GS2C_CharacterUpdate sendmsg;
			sendmsg.set_id(id);
			charact->SaveTo(sendmsg);
			Send( pb::SMSG_CHARACTER_UPDATE, sendmsg);
		}
	}
}