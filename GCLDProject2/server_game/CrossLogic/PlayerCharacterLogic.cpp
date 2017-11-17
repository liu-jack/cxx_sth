#include <boost/typeof/typeof.hpp>

#include "PlayerCharacterLogic.h"

#include "server_client/DbProxyServerClient.h"  // sDbProxyClient

#include "Loot/LootList.h"
#include "Loot/LootManager.h"
#include "../Trigger/trigger.h"
#include "character/CharacterStorage.h"
#include "character/Character.h"
#include "character/CharacterProto.h"
#include "character/CharacterManager.h"

#include "quest/behavior/PlayerBehaviorCounter.h"
#include "../object/PlayerStringGroup.h"
#include "CrossLogic/PlayerItemLogic.h"
#include "CrossLogic/ItemCharacterLogic.h"
#include "../Building/BuildingLogMgr.h"
#include "../Building/BuildingDefine.h"
#include "object/Player.h"
#include "object/PlayerDataManager.h"

#include "item/ItemArray.h"
#include "item/Item.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "CommomDef.pb.h"

#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"

#include "../Technology/PlayerTechnology.h"
#include "../Technology/Table/TechnologyTableMgr.h"

using namespace pb;



namespace
{
    typedef std::set<int> IdSet;

    int GetTrainingCurrCount(uint32 randomType)
    {
        if (randomType == 1)
            return GET_BASE_DEF_UINT( BD_CHAR_UPGRADE_RANDOM_1_CURR_COUNT);
        if (randomType == 2)
            return GET_BASE_DEF_UINT( BD_CHAR_UPGRADE_RANDOM_2_CURR_COUNT);
        if (randomType == 3)
            return GET_BASE_DEF_UINT( BD_CHAR_UPGRADE_RANDOM_3_CURR_COUNT);
        return 0;
    }

    bool CheckQualityUpMaterial( IdSet &meterialSet, Character* UpCharacter, CharacterStorage* charStorage )
    {
        for ( IdSet::iterator it = meterialSet.begin() ; it != meterialSet.end(); ++it)
        {
            uint32 matCharId = *it;
            if ( matCharId == UpCharacter->GetID()) // 材料卡片不能是自己
                return false;

            const Character* matChar = charStorage->GetCharacter( matCharId);
            if ( !matChar) // 材料卡片找不到
                return false;

            if ( !charStorage->CanBeDestroy( matCharId)     // 材料卡片不能删除
                || matChar->ProtoID() != UpCharacter->ProtoID())
                return false;      
        }
        return true;
    }
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::DecomposeCharacter( Player *player, uint32 charId, LootList& outLootList )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* theChar = charStorage->MutableCharacter( charId);
    if ( !theChar)
        return ErrInvalidCharacterId;

    ErrCode err_code = RebirthCharacter( player, theChar, outLootList);
    if ( err_code != ErrCommonSuccess)
        return err_code;

    //////////////////////////////////////////////////////////////////////////

    outLootList.AddMoney( GET_BASE_DEF_UINT(BD_DECOMPOSE_CHAR_CURRENCY_TYPE1), GET_BASE_DEF_UINT(BD_DECOMPOSE_CHAR_CURRENCY_COUNT1) );

    outLootList.AddMoney( GET_BASE_DEF_UINT(BD_DECOMPOSE_CHAR_CURRENCY_TYPE2), GET_BASE_DEF_UINT(BD_DECOMPOSE_CHAR_CURRENCY_COUNT2) ); 
    
    PlayerCharacterLogic::DestroyCharacterForPlayer( player, charId);

    return ErrCommonSuccess;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::RebirthCharacter( Player *player, uint32 charId, LootList& outLootList )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* theChar = charStorage->MutableCharacter( charId);
    if ( !theChar)
        return ErrInvalidCharacterId;

    if ( !player->EnoughGold(GET_BASE_DEF_UINT(BD_REBIRTH_CHAR_REQUIRE_GOLD)))
        return ErrNotEnoughCurr;

    ErrCode result = RebirthCharacter( player, theChar, outLootList);
    if ( result ==  ErrCommonSuccess)
    {
        player->TryDeductCurrency( pb::IR_CYCLE_COST, eSysGold, GET_BASE_DEF_UINT(BD_REBIRTH_CHAR_REQUIRE_GOLD));
    } 

    return result;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::RebirthCharacter( Player *player, Character* character, LootList& outLootList )
{
    // 升级 金币返还 
    uint32 coinReinforce = 0;
    uint32 coinQuality = 0;
    uint32 coinLevel = 0;
    uint32 coinSkill = 0;
    uint32 coinTraining = 0;
    ItemID2Count itemid2count;


    coinLevel = uint32( character->Proto().GetCharacterLvUpCoin(1, character->GetLevel())
        * GET_BASE_DEF_FLOAT( BD_REBIRTH_CHAR_LV_COIN_PROPORTION)); 

    coinQuality = uint32( character->Proto().GetCharacterQualityUpCoin(1, character->GetQuality())
        * GET_BASE_DEF_FLOAT( BD_REBIRTH_CHAR_QUALITY_COIN_PROPORTION));

    coinReinforce = uint32 ( character->Proto().GetCharacterReinforceUpCoin(1, character->GetReinforce())
        * GET_BASE_DEF_FLOAT( BD_REBIRTH_CHAR_REINFORCE_COIN_PROPORTION));

    uint32 coinEachSkill = 0;
    for ( uint32 i = 0 ; i <= DB_CharSetSkills::MAX_SKILL_INDEX; ++i)
    {
        const DB_CharSetSkills* skill_info = character->Proto().GetSkillInfo(i);
        uint32 skill_level = character->GetSkillLevel(i);
        if (skill_info != NULL && skill_level > 1 )
        {
            if ( sCharacterMgr.TryGetSkillLevelUpCoin( skill_info->skill_type, 1, skill_level, coinEachSkill))
            {
                coinSkill += coinEachSkill;
            }
        }
    }
    coinSkill = uint32 ( coinSkill * GET_BASE_DEF_FLOAT(BD_REBIRTH_CHAR_SKILL_COIN_PROPORTION));


    uint32 sumXp = uint32(character->GetSumXp() * GET_BASE_DEF_FLOAT(BD_REBIRTH_CHAR_LV_EXP_PROPORTION));
    sCharacterMgr.GetRebirthLevelSumItem( sumXp, itemid2count);
    sCharacterMgr.GetRebitrhQualitySumItem(character->ProtoID(),character->GetQuality(), itemid2count);



    uint32 sumCoin = coinReinforce + coinQuality + coinLevel + coinSkill + coinTraining;
    //////////////////////////////////////////////////////////////////////////
    character->Rebirth();
    outLootList.AddItem( itemid2count);
    outLootList.AddMoney( eCoin, sumCoin );

    return ErrCommonSuccess;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::ConfirmTrainingCharacter( Player* player, uint32 charId )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* upCharacter = charStorage->MutableCharacter( charId);
    if ( !upCharacter)
        return ErrInvalidCharacterId;

    if ( upCharacter->IsRandomAttrAllZero())
        return ErrHaveNotTraining;

    if (!upCharacter->ConfirmTraining())
        return pb::ErrCommonFail;

    if ( charStorage->IsInBatterArray( upCharacter->GetID()))
    {
        ItemCharacterLogic::ReCalculateBattleAttr( player);
    }

    return ErrCommonSuccess;
}



PlayerCharacterLogic::ErrCode PlayerCharacterLogic::UpgradeCharacterSkill( Player* player, uint32 charId, uint32 skillIndex )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* upCharacter = charStorage->MutableCharacter( charId);
    if ( !upCharacter)
        return ErrInvalidCharacterId;

    if ( !upCharacter->IsSkillActive(skillIndex) )
        return ErrCommonFail;

    uint32 skill_level = upCharacter->GetSkillLevel(skillIndex);
    uint32 coinNeed = 0;
    ItemID2Count itemid2count;

    const DB_CharSetSkills* skillInfo = upCharacter->Proto().GetSkillInfo(skillIndex);
    if ( skillInfo == NULL)
        return ErrCommonFail;

    if ( !sCharacterMgr.TryGetSkillLevelUpCoin(skillInfo->skill_type, skill_level, skill_level +1, coinNeed))
        return ErrCommonFail;

    if ( !player->EnoughCurrency(eCoin, coinNeed))
        return ErrNotEnoughCoin;

    if ( !bag->EnoughItem(itemid2count))
        return ErrCommonFail;

    //////////////////////////////////////////////////////////////////////////
    int priviousLevel = upCharacter->GetSkillLevel(skillIndex);
    if ( !upCharacter->TryAddSkillLevel( skillIndex))
        return ErrCommonFail;

    bool result = player->DestroyItem( pb::IR_CHARACTER_UPGRADE_SKILL_COST, itemid2count);
    MMO_ASSERT( result == true);

    result = player->TryDeductCurrency( pb::IR_CHARACTER_UPGRADE_SKILL_COST, eCoin, coinNeed);
    MMO_ASSERT( result == true);
    if ( charStorage->IsInBatterArray( upCharacter->GetID()))
    {
        player->m_questLogger->UpdateQuestCategory(player, QC1_CHAR, QC2_CHAR_SKILL_LEVEL_REACH);
        ItemCharacterLogic::ReCalculateBattleAttr( player);
    }

    return ErrCommonSuccess;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::UpgradeCharacterReinforce( Player* player, uint32 charId )
{
    if ( !sPlayerDataMgr.IsUnlockCharReinforce(player->GetLevel()))
        return pb::ErrCommonFail;

    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* upCharacter = charStorage->MutableCharacter( charId);
    if ( !upCharacter)
        return ErrInvalidCharacterId;

 
    //////////////////////////////////////////////////////////////////////////

    if ( charStorage->IsInBatterArray( upCharacter->GetID()))
    {
        player->m_questLogger->UpdateQuestCategory(player, QC1_CHAR, QC2_CHAR_REINFORCE_REACH);
        ItemCharacterLogic::ReCalculateBattleAttr( player);
    }



    return ErrCommonSuccess;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::InlayRuneToCharacter( Player* player, uint32 charId , uint32 slot )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* upCharacter = charStorage->MutableCharacter( charId);
    if ( !upCharacter)
        return ErrInvalidCharacterId;

    return ErrCommonSuccess;
}

//PlayerCharacterLogic::ErrCode PlayerCharacterLogic::UpgradeCharacterQuality( Player* player, pb::C2GS_UpgradeQuality& msg )
//{
//    CharacterStorage* charStorage = player->m_characterStorage.get();
//    if ( charStorage == NULL )
//        return pb::ErrCommonFail;
//
//    ItemArray * bag = player->m_bag.get();
//    if ( bag == NULL )
//        return pb::ErrCommonFail;
//
//    uint32 charId = msg.char_id();
//    Character* UpCharacter = charStorage->MutableCharacter( charId);
//    if ( !UpCharacter)
//        return ErrInvalidCharacterId;
//
//    const Uint32List& materialCharList = msg.material_char_ids();
//
//    IdSet meterialSet;
//    for ( int i = 0 ; i < materialCharList.values_size(); ++i)
//    {
//        meterialSet.insert( materialCharList.values(i));
//    }
//
//
//    if ( !CheckQualityUpMaterial(meterialSet, UpCharacter, charStorage))
//        return ErrCharUpQualityMaterialNotMatch;
//
//
//    uint32 need_coin = UpCharacter->Proto().GetCharacterQualityUpCoin(UpCharacter->GetQuality());
//    if ( !player->EnoughCurrency( eCoin, need_coin) )
//        return ErrNotEnoughCoin;
//
//    //////////////////////////////////////////////////////////////////////////
//
//    bool res = false;
//    res = player->TryDeductCurrency(
//        pb::IR_CHARACTER_UPGRADE_QUALITY_COST,
//        eCoin,
//        need_coin
//        );
//    MMO_ASSERT( res == true);
//
//    for ( IdSet::iterator it = meterialSet.begin() ; it != meterialSet.end(); ++it)
//    {
//        res = PlayerCharacterLogic::DestroyCharacterForPlayer( player, *it);
//        MMO_ASSERT( res == true);
//    }
//
//    if ( charStorage->IsInBatterArray( UpCharacter->GetID()))
//    {
//        player->m_questLogger->UpdateQuestCategory(player, QC1_CHAR, QC2_CHAR_QUALITY_REACH);
//        ItemCharacterLogic::ReCalculateBattleAttr( player);
//    }
//
//
//    return ErrCommonSuccess;
//}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::UseUpgradeLvItemToCharacter( Player* player, pb::C2GS_UseItemsToTarget& msg )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return pb::ErrCommonFail;

    ItemArray * bag = player->m_bag.get();
    if ( bag == NULL )
        return pb::ErrCommonFail;

    Character* character = charStorage->MutableCharacter( msg.target_id());
    if ( !character)
        return ErrInvalidCharacterId;

	uint32 card_type = msg.card_type();
	uint32 nr_cards = msg.nr_cards();
	if ( !bag->EnoughItem(card_type,nr_cards))
		return ErrNotEnoughItem;
	const uint32 xp_value = sCharacterMgr.getUpgradeLevelXpByItemId(card_type);
	const uint32 sum_xp = xp_value * nr_cards;
	character->TryAddXP(sum_xp);
    bool res = false;
    res = player->DestroyItem( pb::IR_CHARACTER_UPGRADE_LV_COST, card_type,nr_cards);
	MMO_ASSERT( res == true);
    if ( charStorage->IsInBatterArray( character->GetID()))
    {
        player->m_questLogger->UpdateQuestCategory(player, QC1_CHAR, QC2_CHAR_LEVEL_REACH);
        ItemCharacterLogic::ReCalculateBattleAttr( player);
    }
    return ErrCommonSuccess;
}

Character* PlayerCharacterLogic::CreateCharacterForPlayer( Player* player, uint32 protoId )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return NULL;

    Character* ch = charStorage->CreateCharacter(player, protoId);
    if (ch)
    {
        pb::GS2C_CharacterCreate msg;
        ch->SaveTo( msg);
        player->Send( pb::SMSG_CHARACTER_CREATE, msg );
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_CREATE_CHAR, msg );
    }
    return ch;
}

bool PlayerCharacterLogic::DestroyCharacterForPlayer( Player* player, uint32 charId )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return false;

    bool result = charStorage->DestroyCharacter(charId);
    if ( result)
    {
        pb::Uint32List msg;
        msg.add_values( charId);
        player->Send(pb::SMSG_DESTROY_CHARACTER, msg);
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_DELETE_CHARS, msg );
    }
    return result;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::BuyPlayerHeadIcon( Player* player, uint32 iconId, pb::GS2C_PlayerStringGroup& msg )
{
	
	if (player->m_playStringGroup->ContainId(pb::PLAYER_FIELD_HEAD_ICON_IDS,iconId))
	{
		PlayerString* PlayStr =	player->m_playStringGroup->GetPlayerString(pb::PLAYER_FIELD_HEAD_ICON_IDS);
		if (PlayStr)
		{
			PlayStr->SaveTo(*msg.add_player_strings());
			return ErrCommonSuccess;
		}
		return ErrCommonSuccess;
	}

	const DB_HeadIconUnlock* headIconReq = sPlayerDataMgr.GetHeadIconUnLock(iconId);
	if (headIconReq)
	{
		if(headIconReq->unlock_condition != 0)
		{
			if(!sTrigger.Check(player,headIconReq->unlock_condition))
			{
				return ErrNotUnlock;
			}
		}
		if (headIconReq->is_need_buy != 0)
		{
			if(!player->EnoughCurrency(headIconReq->cost_type,headIconReq->cost_num))
			{
				return ErrNotEnoughCurr;
			}
		}
	}
	else
	{
		return ErrAlreadyExist;
	}

	

	if(player->m_playStringGroup->AddId(player,pb::PLAYER_FIELD_HEAD_ICON_IDS,iconId))
	{
		if(headIconReq->is_need_buy != 0)
		{
			if (!player->TryDeductCurrency(pb::IR_PLAYER_BUY_HEAD_ICON_COST,headIconReq->cost_type,headIconReq->cost_num))
			{
				player->m_playStringGroup->ReMoveId(player,pb::PLAYER_FIELD_HEAD_ICON_IDS,iconId);
				return ErrNotEnoughCurr;
			}
		}
		PlayerString* PlayStr =	player->m_playStringGroup->GetPlayerString(pb::PLAYER_FIELD_HEAD_ICON_IDS);
		if (PlayStr)
		{
			PlayStr->SaveTo(*msg.add_player_strings());
			return ErrCommonSuccess;
		}
	}
	
	return pb::ErrCommonFail;
}

void PlayerCharacterLogic::CheckPlayerNewFreeUnlockInfo( Player* player)
{
	GS2C_PlayerStringGroup msg;
	ErrCode err_code = pb::ErrCommonFail;
	std::set< uint32 > newFreeUnlock;

	const PlayerDataMgr::IconId2UnLockReq& curIconMap = sPlayerDataMgr.GetHeadIconMap();
	PlayerDataMgr::IconId2UnLockReq::const_iterator it = curIconMap.begin();
	for(; it != curIconMap.end(); ++it)
	{
		const DB_HeadIconUnlock* headIconReq =  it->second;
		if(sTrigger.Check(player,headIconReq->unlock_condition) && (headIconReq->is_need_buy == 0))
		{
			newFreeUnlock.insert(headIconReq->role_image_id);
		}
	}
	if ((int)newFreeUnlock.size() > 0)
	{
		bool isAdd = false;
		for (std::set< uint32 >::iterator iter = newFreeUnlock.begin(); iter != newFreeUnlock.end(); ++iter)
		{
			uint32 iconId = (*iter);
			if(player->m_playStringGroup->AddId(player,pb::PLAYER_FIELD_HEAD_ICON_IDS,iconId))
			{
				isAdd = true;
			}
		}

		if (isAdd)
		{
			PlayerString* curPlayStr = player->m_playStringGroup->GetPlayerString(pb::PLAYER_FIELD_HEAD_ICON_IDS);
			if (curPlayStr)
			{
				curPlayStr->SaveTo(*msg.add_player_strings());
				err_code = ErrCommonSuccess;
			}
		}
	}
	if ( err_code == ErrCommonSuccess)
	{
		player->Send( pb::SMSG_UPDATE_PLAYER_ATTACH_INFO, msg);
	}
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::SwitchPlayerHeadIcon( Player* player, uint32 iconId)
{
	CheckPlayerNewFreeUnlockInfo(player);
	if (player->m_playStringGroup->ContainId(pb::PLAYER_FIELD_HEAD_ICON_IDS,iconId))
	{
		player->SetHeadIconId(iconId);
		return ErrCommonSuccess;
	}

	return pb::ErrCommonFail;
}

PlayerCharacterLogic::ErrCode PlayerCharacterLogic::OnKeyRecoverHp( Player* player , uint32 charId )
{
	if (Character* hero = player->m_characterStorage->MutableCharacter(charId))
	{
		if(hero->isInPrison())	return pb::ErrHeroInDungeon;		//被捕获状态

		uint32 curNeedHp =  hero->NeedHp();
		uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);
		if (curNeedHp > 0 && hpSpeed > 0)
		{
			uint32 needTime = (curNeedHp / hpSpeed) * 60 + (curNeedHp % hpSpeed > 0 ? 60 : 0);

			uint32 perOderForSec = GET_BASE_DEF_UINT( BD_REDUCE_DRAFT_TIME);
			uint32 orderNum = needTime/perOderForSec  + (needTime % perOderForSec > 0 ? 1 : 0);	
			
			const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(hero->soldierLv);
			MMO_ASSERT(pTable);
			if (pTable == NULL)
				return ErrCommonFail;
			float fNeedFood = pTable->NeedFood();
			int nAllNeedFood = (int)(curNeedHp * fNeedFood *(hero->GetTotalReduceCostHP()/100.0)+ 0.5);
			int curAllFood = player->GetCurrency(eFood);
			if(curAllFood >= nAllNeedFood)
			{
				uint32 tmpnum = player->GetCurrency(eTroopsOder);	//征兵令的数量
				uint32 coinnum = player->GetCurrency(eSysGold) + player->GetCurrency(eCashGold);	//金币数量
				uint32 needcoin = MAX(0,(int)orderNum-(int)tmpnum);
				uint32 needorder = MIN((int)orderNum,(int)tmpnum);
				if(tmpnum + coinnum >= orderNum)
				{
					if(hero->RecoverFullSoldier())
					{
						player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eTroopsOder,needorder);
						player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eSysGold,needcoin);	
						player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eFood,nAllNeedFood);
						player->m_characterStorage->SetModifiedID(hero->GetID());
						return ErrCommonSuccess;
					}
				}
				else
				{
					return ErrNotEnoughToopsOder;
				}
			}
			else
				return ErrNotEnoughFood;
		}
		else if(curNeedHp == 0)
			hero->RecoverFullSoldier();
	}
	return pb::ErrCommonFail;
}

bool PlayerCharacterLogic::TryToRecoverHp(Player* player, Character* character, uint32 hpSpeed, bool isUseCoin)
{

	uint32 curNeedHp =  character->NeedHp();
	if (curNeedHp > 0)
	{
		if(hpSpeed <= 0) return false;
		if(character->isInPrison())	return false;		//被捕获状态

		uint32 needTime = (curNeedHp / hpSpeed) * 60 + (curNeedHp % hpSpeed > 0 ? 60 : 0);

		uint32 perOderForSec = GET_BASE_DEF_UINT( BD_REDUCE_DRAFT_TIME);
		uint32 orderNum = needTime/perOderForSec  + (needTime % perOderForSec > 0 ? 1 : 0);		//需要征兵令数量
		const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(character->soldierLv);
		if(pTable)
		{
			float fNeedFood = pTable->NeedFood();
			int nAllNeedFood = (int)(curNeedHp * fNeedFood *(character->GetTotalReduceCostHP()/100.0)+ 0.5);
			int curAllFood = player->GetCurrency(eFood);
			if(curAllFood >= nAllNeedFood)	//粮食足够
			{
				uint32 tmpnum = player->GetCurrency(eTroopsOder);	//征兵令的数量
				if(!isUseCoin)	//不使用金币
				{
					if(tmpnum >= orderNum)	//征兵令足够
					{
						if(character->RecoverFullSoldier())
						{
							player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eTroopsOder,orderNum);
							player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eFood,nAllNeedFood);
							player->m_characterStorage->SetModifiedID(character->GetID());
							return true;
						}
						else
							return false;
					}
					else
						return false;
				}
				else	//使用金币
				{
					uint32 coinnum = player->GetCurrency(eSysGold) + player->GetCurrency(eCashGold);	//金币数量
					uint32 needcoin = MAX(0,(int)orderNum-(int)tmpnum);
					uint32 needorder = MIN((int)orderNum,(int)tmpnum);
					if(tmpnum + coinnum >= orderNum)	//金币与征兵令总量足够
					{
						if(character->RecoverFullSoldier())
						{
							player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eTroopsOder,needorder);
							player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eSysGold,needcoin);	
							player->TryDeductCurrency(IR_CHARACTER_INCREASE_HP_COST,eFood,nAllNeedFood);
							player->m_characterStorage->SetModifiedID(character->GetID());
							return true;
						}
						else
							return false;
					}
					else
						return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return true;
}

void PlayerCharacterLogic::InitCreatPlayerData( Player* player )
{
	const ItemID2Count& curItemMap = sPlayerDataMgr.GetInitItemMap();
	player->CreateItem(IR_PLAYER_CREAT_INIT_GET,curItemMap);
}

void PlayerCharacterLogic::SendCharacterStorageModify( Player* player )
{
	//LLOG("[1002] SendCharacterStorageModify 1");
    if ( player == NULL)
        return;

    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return;

    if ( !charStorage->HasModified())
        return;
	//LLOG("[1002] SendCharacterStorageModify 2");
    BOOST_AUTO( it , charStorage->ModIdBegin());
    BOOST_AUTO( itEnd , charStorage->ModIdEnd());
    for ( ; it!= itEnd; ++it)
    {
		//LLOG("[1002] SendCharacterStorageModify 3");
        int id = *it;
        Character* charact = charStorage->MutableCharacter(id);            
        if (charact && charact->HasUpdateInfo())
        {
            pb::GS2C_CharacterUpdate msg;
            charact->WriteUpdateInfoTo( msg);
#ifdef _MMO_SERVER_
            sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_CHAR, msg );
#endif

            player->Send( pb::SMSG_CHARACTER_UPDATE, msg);

            charact->SetUnmodified();
        }
    }
	LLOG("[1002] SendCharacterStorageModify 4");
    charStorage->SetUnmodified();
}

void PlayerCharacterLogic::updateCharacterInfoEx(Player* player,uint32 charId)
{
	if (Character* hero = player->m_characterStorage->MutableCharacter(charId))
	{
		pb::GS2C_CharacterUpdate msg;
		hero->WriteUpdateInfoTo( msg );
#ifdef _MMO_SERVER_
		sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_CHAR, msg );
#endif
		hero->SetUnmodified();
	}
}

void PlayerCharacterLogic::GenBattleArrayProtoStr( Player& player, std::string& out_str )
{
	CharacterStorage* charStor = player.m_characterStorage.get();
	const CharacterIds& ids = charStor->GetBattleArray();
	for (CharacterIds::const_iterator it = ids.begin();
		it != ids.end(); ++it)
	{

		if ( *it == 0)
			continue;
		const Character* ch = charStor->GetCharacter(*it);
		if ( ch == NULL)
			continue;

		char tmp[32] = {};
		snprintf( tmp, 32, "%d", ch->ProtoID());
		if ( !out_str.empty())
		{
			out_str.append(" ");
		}
		out_str.append( tmp);
	}
}

int32 PlayerCharacterLogic::GenBattleArrayConbatPower( Player& player )
{
	int32 power = 0;
	CharacterStorage* charStor = player.m_characterStorage.get();
	const CharacterIds& ids = charStor->GetBattleArray();
	for (CharacterIds::const_iterator it = ids.begin();
		it != ids.end(); ++it)
	{

		if ( *it == 0)
			continue;
		const Character* ch = charStor->GetCharacter(*it);
		if ( ch == NULL)
			continue;

		power += ch->GetAttack();
	}
	return power;
}
